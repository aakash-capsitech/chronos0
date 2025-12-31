#include "socket.h"
#include <cstdlib> // for rand()
#include <iostream>

#ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")
#endif

UDPSocket::UDPSocket() {
    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData)
    #endif
        //create the socket: AF_INET (IPv4), SOCK_DGRAM (UDP)
        handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

UDPSocket::~UDPSocket() {
    #ifdef _WIN32
        closesocket(handle);
        WSACleanup();
    #else
        close(handle);
    #endif
}

bool UDPSocket::Bind(unsigned short port) {
    socketAddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
    address.sin_port = htons(port);

    if (bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
        return false;
    }
    return true;
}

bool UDPSocket::SetNonBlocking() {
    #ifdef _WIN32
        DWORD nonBlocking = 1;
        return ioctlsocket(handle, FIONBIO, &nonBlocking) == 0;
    #else
        int flags = fcntl(handle, F_GETFL, 0);
        return fcntl(handle, F_SETFl, flags | 0_NONBLOCK) == 0;
    #endif
}

// int UDPSocket::SendTo(const void* data,int size, sockaddr_in& address) {
//     return sendto(handle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
// }

// Set this to 0.1 for 10% loss, 0.5 for 50% loss
const float ARTIFICIAL_LOSS_CHANCE = 0.1f; 

int UDPSocket::SendTo(const void* data, int size, sockaddr_in& address) {
    // Check if we should "drop" this packet
    float randomRoll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    if (randomRoll < ARTIFICIAL_LOSS_CHANCE) {
        // We return the size to trick the caller into thinking it sent,
        // but we never actually call the OS 'sendto' function.
        // std::cout << "[Simulator] Packet Dropped!" << std::endl;
        return size; 
    }

    return sendto(handle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
}

int UDPSocket::ReceiveFrom(void* data, int size, sockaddr_in& address) {
    socklen_t fromLen = sizeof(address);
    return recvfrom(handle, (char*)data, size, 0, (sockaddr*)&address, &fromLen);
}