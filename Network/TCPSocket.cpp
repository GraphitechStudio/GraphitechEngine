#include "TCPSocket.hpp"
#include <iostream>

namespace Graphitech {
    namespace Network {

        TCPSocket::TCPSocket() : m_socket(-1) {
            m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
        }

        TCPSocket::TCPSocket(int socket) : m_socket(socket) {
        }

        TCPSocket::~TCPSocket() {
            Close();
        }

        TCPSocket::TCPSocket(TCPSocket&& other) noexcept : m_socket(other.m_socket) {
            other.m_socket = -1;
        }

        TCPSocket& TCPSocket::operator=(TCPSocket&& other) noexcept {
            if (this != &other) {
                Close();
                m_socket = other.m_socket;
                other.m_socket = -1;
            }
            return *this;
        }

        void TCPSocket::Close() {
            if (m_socket >= 0) {
                ::close(m_socket);
                m_socket = -1;
            }
        }

        bool TCPSocket::Bind(uint16_t port) {
            if (m_socket < 0) return false;

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            addr.sin_port = htons(port);

            int reuse = 1;
            setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

            if (::bind(m_socket, (sockaddr*)&addr, sizeof(addr)) < 0) {
                perror("TCPSocket Bind failed");
                return false;
            }
            return true;
        }

        bool TCPSocket::Listen() {
            if (m_socket < 0) return false;
            if (::listen(m_socket, SOMAXCONN) < 0) {
                perror("TCPSocket Listen failed");
                return false;
            }
            return true;
        }

        bool TCPSocket::Accept(TCPSocket& clientSocket) {
            if (m_socket < 0) return false;

            sockaddr_in addr{};
            socklen_t len = sizeof(addr);
            int clientFd = ::accept(m_socket, (sockaddr*)&addr, &len);

            if (clientFd < 0) {
                return false;
            }

            clientSocket = TCPSocket(clientFd);
            return true;
        }

        bool TCPSocket::Connect(const std::string& ip, uint16_t port) {
            if (m_socket < 0) return false;

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
                return false;
            }

            if (::connect(m_socket, (sockaddr*)&addr, sizeof(addr)) < 0) {
                // perror("TCPSocket Connect failed");
                return false;
            }
            return true;
        }

        bool TCPSocket::Send(const void* data, size_t size) {
            if (m_socket < 0) return false;
            
            size_t totalSent = 0;
            const char* bytes = static_cast<const char*>(data);

            while (totalSent < size) {
                ssize_t sent = ::send(m_socket, bytes + totalSent, size - totalSent, 0); // MSG_NOSIGNAL on Linux usually to avoid SIGPIPE
                if (sent < 0) {
                    return false;
                }
                totalSent += sent;
            }
            return true;
        }

        ssize_t TCPSocket::Receive(void* buffer, size_t size) {
            if (m_socket < 0) return -1;
            return ::recv(m_socket, buffer, size, 0);
        }

        void TCPSocket::SetNonBlocking(bool nonBlocking) {
            if (m_socket < 0) return;
            int flags = fcntl(m_socket, F_GETFL, 0);
            if (nonBlocking) {
                fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
            } else {
                fcntl(m_socket, F_SETFL, flags & ~O_NONBLOCK);
            }
        }

    }
}
