#include <SDL.h>
#include <iostream>
#include "socket.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Chronos Client", SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    UDPSocket clientSocket;
    clientSocket.SetNonBlocking();

    // Define Server Address (Localhost)
    socketAddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = htonl(x7F000001); // 127.0.0.1

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Send heartbeat to server
        const char* msg = "PING";
        clientSocket.SendTo(msg, 4, serverAddr);

        SDL_Delay(16); // Aim for ~60fps
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}