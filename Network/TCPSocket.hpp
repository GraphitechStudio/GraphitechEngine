#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

namespace Graphitech {
    namespace Network {

        class TCPSocket {
        public:
            TCPSocket();
            ~TCPSocket();

            // Move semantics
            TCPSocket(TCPSocket&& other) noexcept;
            TCPSocket& operator=(TCPSocket&& other) noexcept;

            // Delete copy
            TCPSocket(const TCPSocket&) = delete;
            TCPSocket& operator=(const TCPSocket&) = delete;

            bool Bind(uint16_t port);
            bool Listen();
            bool Accept(TCPSocket& clientSocket);
            bool Connect(const std::string& ip, uint16_t port);

            void Close();
            
            // Returns true if all data was sent
            bool Send(const void* data, size_t size);
            
            // Returns bytes received, 0 on disconnect, -1 on error/wouldblock
            ssize_t Receive(void* buffer, size_t size);

            void SetNonBlocking(bool nonBlocking);
            
            bool IsValid() const { return m_socket >= 0; }
            int GetHandle() const { return m_socket; }

        private:
            // Private constructor for Accept
            TCPSocket(int socket);

            int m_socket;
        };

    }
}

#endif // TCPSOCKET_HPP
