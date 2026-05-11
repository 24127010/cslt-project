// IGameMode.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"

class IGameMode {
public:
    virtual ~IGameMode() {}

    virtual void runGame() = 0;          // chạy loop game
    virtual void aiControlBot(float dt) = 0; // AI riêng của từng mode
};
