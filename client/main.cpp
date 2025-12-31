#include <SDL.h>
#include <iostream>
#include "socket.h"
#include "simulation.h"

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Chronos Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    PlayerState myState { 320.0f, 240.0f };
    InputS myInput { false, false, false, false };

    uint32_t prevTime = SDL_GetTicks();
    float accumulator = 0.0f;
    const float FIXED_DT = 0.01666f;
    const float FIXED_DT_MS = 16.66f;

    bool running = true;

    uint32_t clientTick = 0;
    while (running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        const uint8_t* keystate = SDL_GetKeyboardState(NULL);
        myInput.up    = keystate[SDL_SCANCODE_W];
        myInput.down  = keystate[SDL_SCANCODE_S];
        myInput.left  = keystate[SDL_SCANCODE_A];
        myInput.right = keystate[SDL_SCANCODE_D];

        uint32_t currTime = SDL_GetTicks();
        float frameTime = (float)(currTime - prevTime);
        prevTime = currTime;
        
        accumulator += frameTime;

        uint8_t mask = 0;
        if (keystate[SDL_SCANCODE_W]) mask |= InputBit::W;
        if (keystate[SDL_SCANCODE_A]) mask |= InputBit::A;
        if (keystate[SDL_SCANCODE_S]) mask |= InputBit::S;
        if (keystate[SDL_SCANCODE_D]) mask |= InputBit::D;

        // while (accumulator >= FIXED_DT_MS) {
        //     UpdateSimulation(myState, myInput, FIXED_DT);
        //     accumulator -= FIXED_DT_MS;
        // }

        while (accumulator >= FIXED_DT_MS) {
            // Prepare the packet
            InputPacket pkt;
            pkt.tickNumber = clientTick;
            pkt.inputMask = mask;

            // Send to Server
            clientSocket.SendTo(&pkt, sizeof(pkt), serverAddr);

            // Update local "predicted" state (Phase 3 preview)
            InputS simInput = { (bool)(mask & InputBit::W), (bool)(mask & InputBit::S), 
                                (bool)(mask & InputBit::A), (bool)(mask & InputBit::D) };
            UpdateSimulation(myState, simInput);

            clientTick++;
            accumulator -= FIXED_DT_MS;
        }

        // 2. Listen for ACKs from Server
        char buffer[1024];
        sockaddr_in fromAddr;
        if (clientSocket.ReceiveFrom(buffer, sizeof(buffer), fromAddr) > 0) {
            PacketHeader* header = (PacketHeader*)buffer;
            if (header->type == (uint8_t)PacketType::SERVER_ACK) {
                AckPacket* ack = (AckPacket*)buffer;
                // std::cout << "Server confirmed tick: " << ack->lastProcessedTick << std::endl;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect playerRect;
        playerRect.x = static_cast<int>(myState.x);
        playerRect.y = static_cast<int>(myState.y);
        playerRect.w = 32; 
        playerRect.h = 32;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}