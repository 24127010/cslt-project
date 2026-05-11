// GameBotEasy.h
#pragma once
#include "BaseGameMode.h"

class EasyMode : public BaseGameMode {
public:
    EasyMode(sf::RenderWindow& win);  

    void aiControlBot(float dt) override;
    void update(float dt) override;        // Phương thức cập nhật game logic
    void render() override;                // Phương thức vẽ lên màn hình
    
};
