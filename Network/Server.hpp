#ifndef SERVER_HPP
#define SERVER_HPP


#include "UDPSocket.hpp"
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
            Endpoint endpoint;
            std::chrono::steady_clock::time_point lastActive;
        };

        class Server {
        public:
            Server(uint16_t port);
            ~Server();

            bool PopEvent(ServerEvent& event);

            void SendToClient(uint32_t clientId, const std::vector<uint8_t>& data);

            void Broadcast(const std::vector<uint8_t>& data);

            void CheckTimeouts(float timeoutSeconds = 5.0f);

            bool IsRunning() const { return m_running; }

        private:
            void Poll();
            void HandlePacket(const std::vector<uint8_t>& buffer, size_t size, const Endpoint& sender);
            uint32_t GetOrCreateClient(const Endpoint& endpoint);

            UDPSocket m_socket;
            std::atomic<bool> m_running;
            
            std::thread m_networkThread;
            std::mutex m_mutex;

            std::unordered_map<std::string, ClientInfo> m_clients;
            std::unordered_map<uint32_t, std::string> m_idToKey;
            uint32_t m_nextClientId = 1;

            std::deque<ServerEvent> m_eventQueue;
        };

    }
}

#endif // SERVER_HPP
