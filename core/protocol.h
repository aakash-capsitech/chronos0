// #pragma once
// #include <cstdint>

// // This ensures the compiler doesn't add "padding" bytes 
// // so the data looks the same on every computer.
// #pragma pack(push, 1)

// enum class PacketType : uint8_t {
//     HEARTBEAT,
//     INPUT,
//     STATE_UPDATE
// };

// struct PacketHeader {
//     uint32_t magic; //A unique ID to ignore random internet noise
//     PacketType type;
// };

// #pragma pack(pop)

// const uint32_t CHRONOS_MAGIC = 0x4348524F; // "CHRO" in hex



#pragma once
#include <cstdint>

#pragma pack(push, 1) // Ensures no "ghost padding" between variables

enum class PacketType : uint8_t {
    CLIENT_INPUT = 1,
    SERVER_ACK   = 2
};

// This is what the Client sends to the Server
struct InputPacket {
    uint32_t magic = 0x4348524F; 
    uint8_t  type  = (uint8_t)PacketType::CLIENT_INPUT;
    uint32_t tickNumber;  // Every 16ms, this increments
    uint8_t  inputMask;   // Bit 0:W, 1:A, 2:S, 3:D
};

// This is what the Server sends back to say "I got it!"
struct AckPacket {
    uint32_t magic = 0x4348524F;
    uint8_t  type  = (uint8_t)PacketType::SERVER_ACK;
    uint32_t lastProcessedTick;
};

#pragma pack(pop)

// Helper to handle bitmasking
namespace InputBit {
    const uint8_t W = 1 << 0; // 0001
    const uint8_t A = 1 << 1; // 0010
    const uint8_t S = 1 << 2; // 0100
    const uint8_t D = 1 << 3; // 1000
}