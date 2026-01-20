#include "Server.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

namespace Graphitech {
    namespace Network {

        Server::Server(uint16_t port) : m_running(false) {
            bool udpOk = m_udpSocket.Bind(port);
            bool tcpOk = m_tcpListener.Bind(port);

            if (udpOk && tcpOk) {
                m_udpSocket.SetNonBlocking(true);
                m_tcpListener.Listen();
                m_tcpListener.SetNonBlocking(true);

                m_running = true;
                std::cout << "[Network] Server listening on port " << port << " (TCP+UDP)" << std::endl;
                
                m_networkThread = std::thread(&Server::Poll, this);
            } else {
                std::cerr << "[Network] Failed to bind server ports" << std::endl;
            }
        }

        Server::~Server() {
            m_running = false;
            if (m_networkThread.joinable()) {
                m_networkThread.join();
            }
            m_udpSocket.Close();
            m_tcpListener.Close();
        }

        void Server::Poll() {
            std::vector<uint8_t> buffer(8192);
            Endpoint sender;

            while (m_running) {
                CheckTimeouts();

                // 1. Accept new TCP clients
                TCPSocket newClient;
                if (m_tcpListener.Accept(newClient)) {
                    newClient.SetNonBlocking(true);
                    
                    std::lock_guard<std::mutex> lock(m_mutex);
                    uint32_t id = m_nextClientId++;
                    
                    // Send Accept packet immediately
                    std::vector<uint8_t> resp(sizeof(PacketHeader) + sizeof(uint32_t));
                    PacketHeader* respHdr = reinterpret_cast<PacketHeader*>(resp.data());
                    respHdr->magic = htonl(MAGIC_NUMBER);
                    respHdr->version = PROTOCOL_VERSION;
                    respHdr->type = static_cast<uint8_t>(PacketType::SERVER_ACCEPT);
                    respHdr->sequence = 0;
                    uint32_t netId = htonl(id);
                    std::memcpy(resp.data() + sizeof(PacketHeader), &netId, sizeof(uint32_t));
                    
                    newClient.Send(resp.data(), resp.size());
                    
                    std::cout << "[Network] Client " << id << " connected via TCP" << std::endl;
                    
                    m_clients.emplace(id, ClientInfo(id, std::move(newClient)));
                    m_eventQueue.push_back({ServerEvent::Connect, id, 0});
                }

                // 2. Poll TCP Clients
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    for (auto it = m_clients.begin(); it != m_clients.end();) {
                        ssize_t len = it->second.tcpSocket.Receive(buffer.data(), buffer.size());
                        if (len > 0) {
                            it->second.lastActive = std::chrono::steady_clock::now();
                            // Process potentially multiple packets in buffer
                            size_t processed = 0;
                            while (processed < (size_t)len) {
                                // Basic checking - in real TCP, we need a persistent buffer per client
                                // Here we assume whole packets for simplicity or handle what we can
                                if ((size_t)len - processed < sizeof(PacketHeader)) break;
                                
                                const PacketHeader* hdr = reinterpret_cast<const PacketHeader*>(buffer.data() + processed);
                                // Determine size... (Simplified: assume header is enough for check or we know size)
                                // Since logic is "PacketType -> Size", we'd need a parser.
                                // For now, pass the whole chunk to HandleTCPPacket and let it decode one.
                                // NOTE: This is a weak point in "Simple TCP".
                                
                                // To make it robust without full buffering logic for this exercise:
                                // We will just process the first packet found.
                                HandleTCPPacket(it->second.id, std::vector<uint8_t>(buffer.begin(), buffer.begin() + len));
                                break; // Consume all for now
                            }
                            ++it;
                        } else if (len == 0) {
                             // Disconnected
                             std::cout << "[Network] Client " << it->second.id << " TCP disconnected" << std::endl;
                             m_eventQueue.push_back({ServerEvent::Disconnect, it->second.id, 0});
                             // Remove from UDP map
                             for (auto uit = m_udpToId.begin(); uit != m_udpToId.end();) {
                                 if (uit->second == it->second.id) uit = m_udpToId.erase(uit);
                                 else ++uit;
                             }
                             it = m_clients.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }

                // 3. Poll UDP
                ssize_t len = m_udpSocket.Receive(buffer.data(), buffer.size(), sender);
                if (len > 0) {
                    HandleUDPPacket(buffer, (size_t)len, sender);
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                }
            }
        }

        void Server::HandleTCPPacket(uint32_t clientId, const std::vector<uint8_t>& buffer) {
             if (buffer.size() < sizeof(PacketHeader)) return;
             const PacketHeader* hdr = reinterpret_cast<const PacketHeader*>(buffer.data());
             if (ntohl(hdr->magic) != MAGIC_NUMBER) return;

             // TCP handles Control packets if any come from client (e.g. maybe Chat later, or RequestSpawn)
             // Currently Client->Server is mostly INPUT (UDP).
             // But valid TCP packets from Client could be: PING (RTT check on TCP?), LEAVE.
        }

        void Server::HandleUDPPacket(const std::vector<uint8_t>& buffer, size_t size, const Endpoint& sender) {
            if (size < sizeof(PacketHeader)) return;
            const PacketHeader* hdr = reinterpret_cast<const PacketHeader*>(buffer.data());
            if (ntohl(hdr->magic) != MAGIC_NUMBER) return;

            std::lock_guard<std::mutex> lock(m_mutex);
            std::string key = sender.ToString();

            if (m_udpToId.find(key) != m_udpToId.end()) {
                // Known UDP client
                uint32_t id = m_udpToId[key];
                if (m_clients.find(id) != m_clients.end()) {
                    m_clients[id].lastActive = std::chrono::steady_clock::now();
                    
                    if (hdr->type == static_cast<uint8_t>(PacketType::INPUT)) {
                        if (size >= sizeof(PacketHeader) + 1) {
                            uint8_t inputData = buffer[sizeof(PacketHeader)];
                            m_eventQueue.push_back({ServerEvent::Input, id, inputData});
                        }
                    }
                    else if (hdr->type == static_cast<uint8_t>(PacketType::PING)) {
                         // Pong on UDP
                        std::vector<uint8_t> pong(sizeof(PacketHeader));
                        PacketHeader* ph = reinterpret_cast<PacketHeader*>(pong.data());
                        ph->magic = htonl(MAGIC_NUMBER);
                        ph->version = PROTOCOL_VERSION;
                        ph->type = static_cast<uint8_t>(PacketType::PONG);
                        ph->sequence = 0;
                        m_udpSocket.Send(pong.data(), pong.size(), sender);
                    }
                }
            } else {
                // Unknown UDP sender - Check for HANDSHAKE
                if (hdr->type == static_cast<uint8_t>(PacketType::UDP_HANDSHAKE)) {
                    if (size >= sizeof(PacketHeader) + sizeof(uint32_t)) {
                        uint32_t claimedId;
                        std::memcpy(&claimedId, buffer.data() + sizeof(PacketHeader), sizeof(uint32_t));
                        claimedId = ntohl(claimedId);

                        if (m_clients.find(claimedId) != m_clients.end()) {
                            std::cout << "[Network] Client " << claimedId << " UDP Handshake successful from " << key << std::endl;
                            m_udpToId[key] = claimedId;
                            m_clients[claimedId].udpEndpoint = sender;
                            m_clients[claimedId].udpConnected = true;
                        }
                    }
                }
            }
        }

        bool Server::PopEvent(ServerEvent& event) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_eventQueue.empty()) return false;
            event = m_eventQueue.front();
            m_eventQueue.pop_front();
            return true;
        }

        void Server::SendToClient(uint32_t clientId, const std::vector<uint8_t>& data, bool reliable) {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_clients.find(clientId);
            if (it == m_clients.end()) return;

            if (reliable) {
                it->second.tcpSocket.Send(data.data(), data.size());
            } else {
                if (it->second.udpConnected) {
                    m_udpSocket.Send(data.data(), data.size(), it->second.udpEndpoint);
                }
            }
        }

        void Server::Broadcast(const std::vector<uint8_t>& data, bool reliable) {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (auto& pair : m_clients) {
                if (reliable) {
                    pair.second.tcpSocket.Send(data.data(), data.size());
                } else {
                    if (pair.second.udpConnected) {
                        m_udpSocket.Send(data.data(), data.size(), pair.second.udpEndpoint);
                    }
                }
            }
        }

        void Server::CheckTimeouts(float timeoutSeconds) {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto now = std::chrono::steady_clock::now();
            
            for (auto it = m_clients.begin(); it != m_clients.end();) {
                std::chrono::duration<float> diff = now - it->second.lastActive;
                if (diff.count() > timeoutSeconds) {
                    uint32_t id = it->second.id;
                    std::cout << "[Network] Client " << id << " timed out." << std::endl;
                    m_eventQueue.push_back({ServerEvent::Disconnect, id, 0});
                    
                    // Cleanup UDP map
                    for (auto uit = m_udpToId.begin(); uit != m_udpToId.end();) {
                        if (uit->second == id) uit = m_udpToId.erase(uit);
                        else ++uit;
                    }
                    it = m_clients.erase(it);
                } else {
                    ++it;
                }
            }
        }

    }
}