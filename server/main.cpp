#include <iostream>
#include <chrono>
#include <thread>
#include "simulation.h"

int main() {
    PlayerState serverState { 320.0f, 240.0f, 150.0f };
    InputS dummyInput { false, false, false, false };

    auto lastTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;

    std::cout << "Server Simulation Running..." << std::endl;

    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double frameTime = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        accumulator += frameTime;

        while (accumulator >= MS_PER_TICK) {
            UpdateSimulation(serverState, dummyInput);
            accumulator -= MS_PER_TICK;
        }

        static int tickCounter = 0;
        if (++tickCounter % 60 == 0) {
            std::cout << "Tick: " << tickCounter << " | Player X: " << serverState.x << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}