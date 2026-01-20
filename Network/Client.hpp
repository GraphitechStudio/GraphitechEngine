#ifndef CLIENT_HPP
#define CLIENT_HPP


#include "UDPSocket.hpp"
#include "TCPSocket.hpp"
#include "Protocol.hpp"
#include <deque>
#include <vector>
#include <functional>
#include <chrono>

namespace Graphitech {
    namespace Network {

        enum class ClientState {
            Disconnected,
            Connecting,
            Connected
        };

        struct WorldUpdate {
            uint8_t type;
            std::vector<uint8_t> rawData;
        };

        struct NetworkStats {
            uint32_t packetsSent = 0;
            uint32_t packetsReceived = 0;
            uint32_t bytesSent = 0;
            uint32_t bytesReceived = 0;
            float rtt = 0.0f; // Round Trip Time in ms
        };

        class Client {
        public:
            Client();
            ~Client();

            bool Connect(const std::string& ip, uint16_t port);
            
            void Disconnect();

            void Poll();

            void SendInput(uint8_t inputMask);

            bool PopWorldUpdate(WorldUpdate& update);

            ClientState GetState() const { return m_state; }
            uint32_t GetClientId() const { return m_clientId; }
            
            const NetworkStats& GetStats() const { return m_stats; }

        private:
            void HandlePacket(const std::vector<uint8_t>& buffer, size_t size);
            void SendUDPHandshake();

            UDPSocket m_udpSocket;
            TCPSocket m_tcpSocket;
            Endpoint m_serverEndpoint;
            
            ClientState m_state;
            uint32_t m_clientId;
            
            NetworkStats m_stats;

            std::deque<WorldUpdate> m_worldUpdates;
            
            std::chrono::steady_clock::time_point m_lastPacketReceived;
            std::chrono::steady_clock::time_point m_lastPacketSent;
            std::chrono::steady_clock::time_point m_lastPingSent;
            std::chrono::steady_clock::time_point m_lastConnectAttempt;
            uint16_t m_lastSequenceReceived;
        };

    }
}

#endif // CLIENT_HPP