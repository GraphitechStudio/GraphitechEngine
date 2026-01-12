#include "UDPSocket.hpp"
#include <iostream>

namespace Graphitech {
    namespace Network {

        Endpoint::Endpoint(const std::string& addr, uint16_t p) : address(addr), port(p) {
            std::memset(&raw, 0, sizeof(raw));
            raw.sin_family = AF_INET;
            raw.sin_port = htons(port);
            inet_pton(AF_INET, address.c_str(), &raw.sin_addr);
        }

        Endpoint::Endpoint(const sockaddr_in& addr) : raw(addr) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
            address = std::string(ip);
            port = ntohs(addr.sin_port);
        }

        std::string Endpoint::ToString() const {
            return address + ":" + std::to_string(port);
        }

        UDPSocket::UDPSocket() : m_socket(-1) {
            m_socket = ::socket(AF_INET, SOCK_DGRAM, 0);
            if (m_socket < 0) {
                perror("UDPSocket creation failed");
            }
        }

        UDPSocket::~UDPSocket() {
            Close();
        }

        void UDPSocket::Close() {
            if (m_socket >= 0) {
                ::close(m_socket);
                m_socket = -1;
            }
        }

        bool UDPSocket::Bind(uint16_t port) {
            if (m_socket < 0) return false;

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            addr.sin_port = htons(port);

            int reuse = 1;
            setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

            if (::bind(m_socket, (sockaddr*)&addr, sizeof(addr)) < 0) {
                perror("UDPSocket Bind failed");
                return false;
            }
            return true;
        }

        void UDPSocket::SetNonBlocking(bool nonBlocking) {
            if (m_socket < 0) return;
            int flags = fcntl(m_socket, F_GETFL, 0);
            if (nonBlocking) {
                fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
            } else {
                fcntl(m_socket, F_SETFL, flags & ~O_NONBLOCK);
            }
        }

        bool UDPSocket::Send(const void* data, size_t size, const Endpoint& destination) {
            if (m_socket < 0) return false;
            ssize_t sent = ::sendto(m_socket, data, size, 0, (sockaddr*)&destination.raw, sizeof(destination.raw));
            return sent == (ssize_t)size;
        }

        ssize_t UDPSocket::Receive(void* buffer, size_t size, Endpoint& sender) {
            if (m_socket < 0) return -1;
            
            sockaddr_in addr{};
            socklen_t len = sizeof(addr);
            
            ssize_t received = ::recvfrom(m_socket, buffer, size, 0, (sockaddr*)&addr, &len);
            
            if (received > 0) {
                sender = Endpoint(addr);
            }
            return received;
        }

    }
}
