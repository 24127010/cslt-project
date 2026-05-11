#pragma once
#include "BaseGameMode.h"

class PVPMode : public BaseGameMode {
private:
    Player& player1 = player;  // alias cho player
    Player& player2 = bot;     // alias cho bot nhưng gọi là player2
public:
    PVPMode(sf::RenderWindow& win);

    void aiControlBot(float dt) override; // PVP không cần AI
    void runGame() override;               // Override để xử lý input 2 player
    void render() override;
    void update(float dt) override;
};
