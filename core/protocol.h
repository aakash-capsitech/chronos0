#pragma once
#include <cstdint>

// This ensures the compiler doesn't add "padding" bytes 
// so the data looks the same on every computer.
#pragma pack(push, 1)

enum class PacketType : uint8_t {
    HEARTBEAT,
    INPUT,
    STATE_UPDATE
};

struct PacketHeader {
    uint32_t magic; //A unique ID to ignore random internet noise
    PacketType type;
};

#pragma pack(pop)

const uint32_t CHRONOS_MAGIC = 0x4348524F; // "CHRO" in hex