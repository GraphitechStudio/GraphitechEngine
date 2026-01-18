#include "Server.hpp"
#include <iostream>
#include <algorithm>

namespace Graphitech {
    namespace Network {

        Server::Server(uint16_t port) : m_running(false) {
            if (m_socket.Bind(port)) {
                m_socket.SetNonBlocking(true);
                m_running = true;
                
                std::cout << "[Network] Server bound to port " << port << std::endl;
                
                m_networkThread = std::thread(&Server::Poll, this);
            } else {
                std::cerr << "[Network] Failed to bind server socket" << std::endl;
            }
        }

        Server::~Server() {
            m_running = false;
            if (m_networkThread.joinable()) {
                m_networkThread.join();
            }
            m_socket.Close();
        }

        void Server::Poll() {
            std::vector<uint8_t> buffer(4096);
            Endpoint sender;

            while (m_running) {
                CheckTimeouts();

                ssize_t len = m_socket.Receive(buffer.data(), buffer.size(), sender);
                if (len > 0) {
                    HandlePacket(buffer, (size_t)len, sender);
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                }
            }
        }

        void Server::HandlePacket(const std::vector<uint8_t>& buffer, size_t size, const Endpoint& sender) {
            if (size < sizeof(PacketHeader)) return;

            const PacketHeader* hdr = reinterpret_cast<const PacketHeader*>(buffer.data());
            
            if (ntohl(hdr->magic) != MAGIC_NUMBER) return;

            std::lock_guard<std::mutex> lock(m_mutex);

            uint32_t clientId = 0;
            std::string key = sender.ToString();

            if (m_clients.find(key) == m_clients.end()) {
                if (hdr->type == static_cast<uint8_t>(PacketType::CLIENT_CONNECT)) {
                    clientId = GetOrCreateClient(sender);
                    m_eventQueue.push_back({ServerEvent::Connect, clientId, 0});
                    
                    std::vector<uint8_t> resp(sizeof(PacketHeader) + sizeof(uint32_t));
                    PacketHeader* respHdr = reinterpret_cast<PacketHeader*>(resp.data());
                    respHdr->magic = htonl(MAGIC_NUMBER);
                    respHdr->version = PROTOCOL_VERSION;
                    respHdr->type = static_cast<uint8_t>(PacketType::SERVER_ACCEPT);
                    respHdr->sequence = 0;

                    uint32_t netId = htonl(clientId);
                    std::memcpy(resp.data() + sizeof(PacketHeader), &netId, sizeof(uint32_t));
                    
                    m_socket.Send(resp.data(), resp.size(), sender);
                }
            } else {
                ClientInfo& info = m_clients[key];
                info.lastActive = std::chrono::steady_clock::now();
                clientId = info.id;

                if (hdr->type == static_cast<uint8_t>(PacketType::INPUT)) {
                    if (size >= sizeof(PacketHeader) + 1) {
                        uint8_t inputData = buffer[sizeof(PacketHeader)];
                        m_eventQueue.push_back({ServerEvent::Input, clientId, inputData});
                    }
                }
                else if (hdr->type == static_cast<uint8_t>(PacketType::PING)) {
                    // Send PONG
                    std::vector<uint8_t> pong(sizeof(PacketHeader));
                    PacketHeader* ph = reinterpret_cast<PacketHeader*>(pong.data());
                    ph->magic = htonl(MAGIC_NUMBER);
                    ph->version = PROTOCOL_VERSION;
                    ph->type = static_cast<uint8_t>(PacketType::PONG);
                    ph->sequence = 0;
                    m_socket.Send(pong.data(), pong.size(), sender);
                }
            }
        }

        uint32_t Server::GetOrCreateClient(const Endpoint& endpoint) {
            std::string key = endpoint.ToString();
            if (m_clients.find(key) != m_clients.end()) {
                return m_clients[key].id;
            }

            uint32_t id = m_nextClientId++;
            m_clients[key] = {id, endpoint, std::chrono::steady_clock::now()};
            m_idToKey[id] = key;
            return id;
        }

        bool Server::PopEvent(ServerEvent& event) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_eventQueue.empty()) return false;
            event = m_eventQueue.front();
            m_eventQueue.pop_front();
            return true;
        }

        void Server::SendToClient(uint32_t clientId, const std::vector<uint8_t>& data) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_idToKey.find(clientId) != m_idToKey.end()) {
                std::string key = m_idToKey[clientId];
                m_socket.Send(data.data(), data.size(), m_clients[key].endpoint);
            }
        }

        void Server::Broadcast(const std::vector<uint8_t>& data) {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (const auto& pair : m_clients) {
                m_socket.Send(data.data(), data.size(), pair.second.endpoint);
            }
        }

        void Server::CheckTimeouts(float timeoutSeconds) {
            std::lock_guard<std::mutex> lock(m_mutex);

            auto now = std::chrono::steady_clock::now();
            auto it = m_clients.begin();
            while (it != m_clients.end()) {
                std::chrono::duration<float> diff = now - it->second.lastActive;
                if (diff.count() > timeoutSeconds) {
                    uint32_t id = it->second.id;
                    std::cout << "[Network] Client " << id << " timed out." << std::endl;
                    m_eventQueue.push_back({ServerEvent::Disconnect, id, 0});
                    
                    m_idToKey.erase(id);
                    it = m_clients.erase(it);
                } else {
                    ++it;
                }
            }
        }

    }
}
