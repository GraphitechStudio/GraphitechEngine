#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP


#include <cstdint>
#include <vector>

namespace Graphitech {
    namespace Network {

        static constexpr uint32_t MAGIC_NUMBER = 0x52545950;
        static constexpr uint8_t PROTOCOL_VERSION = 1;

        static constexpr float WORLD_WIDTH = 1920.0f;
        static constexpr float WORLD_HEIGHT = 1080.0f;

        enum class PacketType : uint8_t {
            CLIENT_CONNECT = 0x01,
            SERVER_ACCEPT = 0x02,
            UDP_HANDSHAKE = 0x05,
            PING = 0x03,
            PONG = 0x04,
            INPUT = 0x10,
            WORLD_UPDATE = 0x20,
            PROJECTILE_SPAWN = 0x30,
            ENTITY_DESTROY = 0x31,
            GAME_RESET = 0x40
        };

        enum Input : uint8_t {
            IN_UP    = 0x01,
            IN_DOWN  = 0x02,
            IN_LEFT  = 0x04,
            IN_RIGHT = 0x08,
            IN_SHOOT = 0x10,
            IN_WEAPON1 = 0x20,
            IN_WEAPON2 = 0x40,
            IN_WEAPON3 = 0x80
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
