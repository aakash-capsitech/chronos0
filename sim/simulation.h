#pragma once
#include <cstdint>

#define TICKS_PER_SECOND 60
#define MS_PER_TICK 16

struct InputS {
    bool up, down, left, right;
};

struct PlayerState {
    float x, y;
    float speed;
};

inline void UpdateSimulation(PlayerState& state, const InputS& input) {
    const float fixed_dt = 1.0f / TICKS_PER_SECOND;
    float moveDist = state.speed * fixed_dt;

    if (input.left)  state.x -= moveDist;
    if (input.right) state.x += moveDist;
    if (input.up)    state.y -= moveDist; 
    if (input.down)  state.y += moveDist;
}