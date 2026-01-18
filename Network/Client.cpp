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

            if (!m_socket.Bind(0)) {
                std::cerr << "[Network] Client failed to bind socket" << std::endl;
                return false;
            }
            m_socket.SetNonBlocking(true);

            m_serverEndpoint = Endpoint(ip, port);
            m_state = ClientState::Connecting;
            m_lastConnectAttempt = std::chrono::steady_clock::now();
            m_lastPacketSent = std::chrono::steady_clock::now();
            
            std::cout << "[Network] Connecting to " << ip << ":" << port << "..." << std::endl;
            SendConnectPacket();
            return true;
        }

        void Client::Disconnect() {
            m_state = ClientState::Disconnected;
            m_socket.Close();
        }

        void Client::Poll() {
            if (m_state == ClientState::Disconnected) return;

            auto now = std::chrono::steady_clock::now();

            if (m_state == ClientState::Connecting) {
                if (std::chrono::duration_cast<std::chrono::seconds>(now - m_lastConnectAttempt).count() >= 1) {
                    std::cout << "[Network] Retrying connection..." << std::endl;
                    SendConnectPacket();
                    m_lastConnectAttempt = now;
                }
            }
            else if (m_state == ClientState::Connected) {
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastPacketSent).count() >= 1000) {
                    std::vector<uint8_t> packet(sizeof(PacketHeader));
                    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(packet.data());
                    hdr->magic = htonl(MAGIC_NUMBER);
                    hdr->version = PROTOCOL_VERSION;
                    hdr->type = static_cast<uint8_t>(PacketType::PING);
                    hdr->sequence = 0;
                    m_socket.Send(packet.data(), packet.size(), m_serverEndpoint);
                    
                    m_lastPacketSent = now;
                    m_lastPingSent = now;
                    
                    m_stats.packetsSent++;
                    m_stats.bytesSent += packet.size();
                }
            }

            std::vector<uint8_t> buffer(4096);
            Endpoint sender;

            while (true) {
                ssize_t len = m_socket.Receive(buffer.data(), buffer.size(), sender);
                if (len <= 0) break;
                
                m_stats.packetsReceived++;
                m_stats.bytesReceived += len;
                HandlePacket(buffer, (size_t)len);
            }
        }

        void Client::HandlePacket(const std::vector<uint8_t>& buffer, size_t size) {
            if (size < sizeof(PacketHeader)) return;

            const PacketHeader* hdr = reinterpret_cast<const PacketHeader*>(buffer.data());
            
            if (ntohl(hdr->magic) != MAGIC_NUMBER) return;

            m_lastPacketReceived = std::chrono::steady_clock::now();

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

            if (m_state == ClientState::Connecting) {
                if (hdr->type == static_cast<uint8_t>(PacketType::SERVER_ACCEPT)) {
                    if (size >= sizeof(PacketHeader) + sizeof(uint32_t)) {
                        uint32_t netId;
                        std::memcpy(&netId, buffer.data() + sizeof(PacketHeader), sizeof(uint32_t));
                        m_clientId = ntohl(netId);
                        m_state = ClientState::Connected;
                        std::cout << "[Network] Connected! Assigned ID: " << m_clientId << std::endl;
                    }
                }
            }
            else if (m_state == ClientState::Connected) {
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
        }

        void Client::SendConnectPacket() {
            std::vector<uint8_t> packet(sizeof(PacketHeader));
            PacketHeader* hdr = reinterpret_cast<PacketHeader*>(packet.data());
            hdr->magic = htonl(MAGIC_NUMBER);
            hdr->version = PROTOCOL_VERSION;
            hdr->type = static_cast<uint8_t>(PacketType::CLIENT_CONNECT);
            hdr->sequence = 0;
            
            m_socket.Send(packet.data(), packet.size(), m_serverEndpoint);
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

            if (m_socket.Send(packet.data(), packet.size(), m_serverEndpoint)) {
                // std::cout << "[Client] Sent INPUT " << (int)inputMask << std::endl;
                m_stats.packetsSent++;
                m_stats.bytesSent += packet.size();
            } else {
                std::cerr << "[Client] Failed to send INPUT" << std::endl;
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
