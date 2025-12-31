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

        while (accumulator >= FIXED_DT_MS) {
            UpdateSimulation(myState, myInput, FIXED_DT);
            accumulator -= FIXED_DT_MS;
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