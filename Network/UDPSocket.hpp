#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <optional>
#include <cstring>

namespace Graphitech {
    namespace Network {

        struct Endpoint {
            std::string address;
            uint16_t port;
            sockaddr_in raw;

            Endpoint() = default;
            Endpoint(const std::string& addr, uint16_t p);
            Endpoint(const sockaddr_in& addr);
            std::string ToString() const;
        };

        class UDPSocket {
        public:
            UDPSocket();
            ~UDPSocket();

            bool Bind(uint16_t port);
            void Close();

            bool Send(const void* data, size_t size, const Endpoint& destination);
            
            ssize_t Receive(void* buffer, size_t size, Endpoint& sender);

            void SetNonBlocking(bool nonBlocking);

            bool IsValid() const { return m_socket >= 0; }

        private:
            int m_socket;
        };

    }
}

#endif // UDPSOCKET_HPP
