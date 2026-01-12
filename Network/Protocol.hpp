#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP


#include <cstdint>
#include <vector>

namespace Graphitech {
    namespace Network {

        static constexpr uint32_t MAGIC_NUMBER = 0x52545950;
        static constexpr uint8_t PROTOCOL_VERSION = 1;

        enum class PacketType : uint8_t {
            CLIENT_CONNECT = 0x01,
            SERVER_ACCEPT = 0x02,
            PING = 0x03,
            INPUT = 0x10,
            WORLD_UPDATE = 0x20
        };

        enum Input : uint8_t {
            IN_UP    = 0x01,
            IN_DOWN  = 0x02,
            IN_LEFT  = 0x04,
            IN_RIGHT = 0x08,
            IN_SHOOT = 0x10
        };

        struct PacketHeader {
            uint32_t magic;
            uint8_t version;
            uint8_t type;
            uint16_t sequence;
        } __attribute__((packed));

    }
}

#endif // PROTOCOL_HPP
