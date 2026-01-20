#ifndef SERVER_HPP
#define SERVER_HPP


#include "UDPSocket.hpp"
#include "TCPSocket.hpp"
#include "Protocol.hpp"
#include <unordered_map>
#include <deque>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

namespace Graphitech {
    namespace Network {

        struct ServerEvent {
            enum Type { Connect, Disconnect, Input };
            Type type;
            uint32_t clientId;
            uint8_t data;
        };

        struct ClientInfo {
            uint32_t id;
            Endpoint udpEndpoint;
            TCPSocket tcpSocket; // Move-only
            std::chrono::steady_clock::time_point lastActive;
            bool udpConnected = false;

            ClientInfo() : id(0) {}
            ClientInfo(uint32_t _id, TCPSocket&& _socket) 
                : id(_id), tcpSocket(std::move(_socket)), lastActive(std::chrono::steady_clock::now()) {}
            
            // Allow move
            ClientInfo(ClientInfo&& other) noexcept = default;
            ClientInfo& operator=(ClientInfo&& other) noexcept = default;

            // Delete copy
            ClientInfo(const ClientInfo&) = delete;
            ClientInfo& operator=(const ClientInfo&) = delete;
        };

        class Server {
        public:
            Server(uint16_t port);
            ~Server();

            bool PopEvent(ServerEvent& event);

            // Defaults to TCP (Reliable)
            void SendToClient(uint32_t clientId, const std::vector<uint8_t>& data, bool reliable = true);

            // Defaults to UDP (Unreliable) for updates, TCP for events
            void Broadcast(const std::vector<uint8_t>& data, bool reliable = true);

            void CheckTimeouts(float timeoutSeconds = 10.0f);

            bool IsRunning() const { return m_running; }

        private:
            void Poll();
            
            // UDP Handler
            void HandleUDPPacket(const std::vector<uint8_t>& buffer, size_t size, const Endpoint& sender);
            
            // TCP Handler
            void HandleTCPPacket(uint32_t clientId, const std::vector<uint8_t>& buffer);

            UDPSocket m_udpSocket;
            TCPSocket m_tcpListener;
            
            std::atomic<bool> m_running;
            
            std::thread m_networkThread;
            std::mutex m_mutex;

            // Map ClientID -> ClientInfo
            std::unordered_map<uint32_t, ClientInfo> m_clients;
            
            // Map UDP Endpoint String -> ClientID
            std::unordered_map<std::string, uint32_t> m_udpToId;
            
            uint32_t m_nextClientId = 1;

            std::deque<ServerEvent> m_eventQueue;
        };

    }
}

#endif // SERVER_HPP