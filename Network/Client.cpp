#include "Client.hpp"
#include <iostream>
#include <cstring>

namespace Graphitech {
    namespace Network {

        Client::Client() : m_state(ClientState::Disconnected), m_clientId(0), m_lastSequenceReceived(0) {
        }

        Client::~Client() {
            Disconnect();
        }

        bool Client::Connect(const std::string& ip, uint16_t port) {
            if (m_state == ClientState::Connected) return true;

            // 1. Setup UDP
            if (!m_udpSocket.Bind(0)) {
                std::cerr << "[Network] Client failed to bind UDP socket" << std::endl;
                return false;
            }
            m_udpSocket.SetNonBlocking(true);

            // 2. Connect TCP
            std::cout << "[Network] Connecting TCP to " << ip << ":" << port << "..." << std::endl;
            if (!m_tcpSocket.Connect(ip, port)) {
                std::cerr << "[Network] TCP Connection failed" << std::endl;
                m_udpSocket.Close();
                return false;
            }
            m_tcpSocket.SetNonBlocking(true);

            m_serverEndpoint = Endpoint(ip, port); // For UDP
            m_state = ClientState::Connecting; // Wait for SERVER_ACCEPT via TCP
            
            m_lastConnectAttempt = std::chrono::steady_clock::now();
            m_lastPacketSent = std::chrono::steady_clock::now();
            
            return true;
        }

        void Client::Disconnect() {
            m_state = ClientState::Disconnected;
            m_tcpSocket.Close();
            m_udpSocket.Close();
        }

        void Client::Poll() {
            if (m_state == ClientState::Disconnected) return;

            auto now = std::chrono::steady_clock::now();

            // PING Logic (UDP)
            if (m_state == ClientState::Connected) {
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastPacketSent).count() >= 1000) {
                    std::vector<uint8_t> packet(sizeof(PacketHeader));
                    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(packet.data());
                    hdr->magic = htonl(MAGIC_NUMBER);
                    hdr->version = PROTOCOL_VERSION;
                    hdr->type = static_cast<uint8_t>(PacketType::PING);
                    hdr->sequence = 0;
                    m_udpSocket.Send(packet.data(), packet.size(), m_serverEndpoint);
                    
                    m_lastPacketSent = now;
                    m_lastPingSent = now;
                    
                    m_stats.packetsSent++;
                    m_stats.bytesSent += packet.size();
                }
            }

            std::vector<uint8_t> buffer(8192);
            Endpoint sender;

            // Poll TCP
            ssize_t tcpLen = m_tcpSocket.Receive(buffer.data(), buffer.size());
            if (tcpLen > 0) {
                // Handle TCP stream (Simplified: Assume packet boundaries for now or single packet)
                m_stats.packetsReceived++;
                m_stats.bytesReceived += tcpLen;
                HandlePacket(std::vector<uint8_t>(buffer.begin(), buffer.begin() + tcpLen), (size_t)tcpLen);
            } else if (tcpLen == 0) {
                // Server closed connection
                std::cerr << "[Network] Server closed TCP connection" << std::endl;
                Disconnect();
                return;
            }

            // Poll UDP
            while (true) {
                ssize_t len = m_udpSocket.Receive(buffer.data(), buffer.size(), sender);
                if (len <= 0) break;
                
                m_stats.packetsReceived++;
                m_stats.bytesReceived += len;
                HandlePacket(std::vector<uint8_t>(buffer.begin(), buffer.begin() + len), (size_t)len);
            }
        }

        void Client::HandlePacket(const std::vector<uint8_t>& buffer, size_t size) {
            if (size < sizeof(PacketHeader)) return;

            const PacketHeader* hdr = reinterpret_cast<const PacketHeader*>(buffer.data());
            
            if (ntohl(hdr->magic) != MAGIC_NUMBER) return;

            m_lastPacketReceived = std::chrono::steady_clock::now();

            // Handshake Logic
            if (m_state == ClientState::Connecting) {
                if (hdr->type == static_cast<uint8_t>(PacketType::SERVER_ACCEPT)) {
                    if (size >= sizeof(PacketHeader) + sizeof(uint32_t)) {
                        uint32_t netId;
                        std::memcpy(&netId, buffer.data() + sizeof(PacketHeader), sizeof(uint32_t));
                        m_clientId = ntohl(netId);
                        
                        std::cout << "[Network] Server Accepted via TCP. ID: " << m_clientId << ". Starting UDP Handshake." << std::endl;
                        
                        m_state = ClientState::Connected;
                        SendUDPHandshake();
                    }
                }
                return;
            }

            // Gameplay Logic
            if (hdr->type == static_cast<uint8_t>(PacketType::PONG)) {
                auto now = std::chrono::steady_clock::now();
                float rtt = std::chrono::duration<float, std::milli>(now - m_lastPingSent).count();
                m_stats.rtt = rtt;
                return;
            }

            if (hdr->type == static_cast<uint8_t>(PacketType::WORLD_UPDATE)) {
                bool is_newer = false;
                if (hdr->sequence > m_lastSequenceReceived) {
                    is_newer = true;
                } else if (m_lastSequenceReceived > 60000 && hdr->sequence < 5000) {
                    is_newer = true;
                }

                if (!is_newer && m_state == ClientState::Connected) {
                    return;
                }
                m_lastSequenceReceived = hdr->sequence;
            }

            uint8_t t = hdr->type;
            if (t == static_cast<uint8_t>(PacketType::WORLD_UPDATE) ||
                t == static_cast<uint8_t>(PacketType::PROJECTILE_SPAWN) ||
                t == static_cast<uint8_t>(PacketType::ENTITY_DESTROY)) {
                
                WorldUpdate update;
                update.type = t;
                update.rawData.assign(buffer.begin() + sizeof(PacketHeader), buffer.begin() + size);
                m_worldUpdates.push_back(update);
            }
        }

        void Client::SendUDPHandshake() {
            std::vector<uint8_t> packet(sizeof(PacketHeader) + sizeof(uint32_t));
            PacketHeader* hdr = reinterpret_cast<PacketHeader*>(packet.data());
            hdr->magic = htonl(MAGIC_NUMBER);
            hdr->version = PROTOCOL_VERSION;
            hdr->type = static_cast<uint8_t>(PacketType::UDP_HANDSHAKE);
            hdr->sequence = 0;
            
            uint32_t netId = htonl(m_clientId);
            std::memcpy(packet.data() + sizeof(PacketHeader), &netId, sizeof(uint32_t));
            
            m_udpSocket.Send(packet.data(), packet.size(), m_serverEndpoint);
            m_lastPacketSent = std::chrono::steady_clock::now();
            
            m_stats.packetsSent++;
            m_stats.bytesSent += packet.size();
        }

        void Client::SendInput(uint8_t inputMask) {
            if (m_state != ClientState::Connected) return;

            std::vector<uint8_t> packet(sizeof(PacketHeader) + 1);
            PacketHeader* hdr = reinterpret_cast<PacketHeader*>(packet.data());
            hdr->magic = htonl(MAGIC_NUMBER);
            hdr->version = PROTOCOL_VERSION;
            hdr->type = static_cast<uint8_t>(PacketType::INPUT);
            hdr->sequence = 0;

            packet[sizeof(PacketHeader)] = inputMask;

            // Input goes over UDP
            if (m_udpSocket.Send(packet.data(), packet.size(), m_serverEndpoint)) {
                m_stats.packetsSent++;
                m_stats.bytesSent += packet.size();
            }
            m_lastPacketSent = std::chrono::steady_clock::now();
        }

        bool Client::PopWorldUpdate(WorldUpdate& update) {
            if (m_worldUpdates.empty()) return false;
            update = m_worldUpdates.front();
            m_worldUpdates.pop_front();
            
            if (m_worldUpdates.size() > 5) {
                m_worldUpdates.clear();
            }
            return true;
        }

    }
}