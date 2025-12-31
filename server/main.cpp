#include <iostream>
#include <vector>
#include "socket.h"
#include "protocol.h"

int main() {
    UDPSocket serverSocket;
    // serverSocket.Bind(8888);

    if (!serverSocket.Bind(8888)) {
        std::cerr << "Failed to bind to port 8888" << std::endl;
        return 1;
    }
    serverSocket.SetNonBlocking();

    std::cout << "Chronos Server Phase 0: Listening on 8888..." << std::endl;

    char buffer[1024];
    sockaddr_in clientAddr;

    while (true) {
        int bytesRead = serverSocket.ReceiveFrom(buffer, sizeof(buffer), clientAddr);
        if (bytesRead >= (int)sizeof(PacketHeader)) {
            PacketHeader* header = (PacketHeader*)buffer;
            if (header->magic == CHRONOS_MAGIC) {
                std::cout << "Valid Packet Received! Type: " << (int)header->type << std::endl;
            }
        }
        // Prevent 100% CPU usage
        // SDL_Delay(1);
        // Small sleep to keep CPU usage low
        #ifdef _WIN32
            Sleep(10);
        #else
            usleep(10000);
        #endif
    }
    return 0;
}