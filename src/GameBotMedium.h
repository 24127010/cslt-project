// GameBotMedium.h
#pragma once
#include "BaseGameMode.h"

class MediumMode : public BaseGameMode {
public:
    MediumMode(sf::RenderWindow& win);

    void aiControlBot(float dt) override;
    void update(float dt) override;   // ? implement
    void render() override;           // ? implement
}; 