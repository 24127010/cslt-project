#pragma once
#include "BaseGameMode.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

class HardMode : public BaseGameMode {
public:
    HardMode(sf::RenderWindow& window);
	void aiControlBot(float dt) override;
    void update(float dt) override;   // override abstract
    void render() override;           // override abstract
};
