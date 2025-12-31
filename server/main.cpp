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

    uint32_t lastProcessedTick = 0;

    while (true) {

        char buffer[1024];
        sockaddr_in clientAddr;
        
        // 1. Receive Input
        int bytes = serverSocket.ReceiveFrom(buffer, sizeof(buffer), clientAddr);
        if (bytes >= sizeof(InputPacket)) {
            InputPacket* pkt = (InputPacket*)buffer;

            // Only process if it's a newer tick (Ignore late/duplicate packets)
            if (pkt->tickNumber > lastProcessedTick) {
                
                // Translate mask back to simulation input
                InputS simInput;
                simInput.up    = pkt->inputMask & InputBit::W;
                simInput.left  = pkt->inputMask & InputBit::A;
                simInput.down  = pkt->inputMask & InputBit::S;
                simInput.right = pkt->inputMask & InputBit::D;

                // Authoritative Update
                UpdateSimulation(serverState, simInput);
                lastProcessedTick = pkt->tickNumber;

                // 2. Send ACK back to Client
                AckPacket ack;
                ack.lastProcessedTick = lastProcessedTick;
                serverSocket.SendTo(&ack, sizeof(ack), clientAddr);
            }
        }

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