#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"

class GameMode {
protected:
    sf::RenderWindow& gameWindow;
    Player player;
    Player bot;
    sf::RectangleShape background;
    sf::Texture backgroundTexture;
    sf::RectangleShape playerHealthBar;
    sf::RectangleShape botHealthBar;
    sf::Font font;
    sf::Text vsText, exitText, gameOverText;
    sf::Music bgMusic;
    bool gameEnded;
    sf::Clock clock;

    virtual void aiControlBot(float deltaTime) = 0; // abstract AI

public:
    GameMode(sf::RenderWindow& win, const std::string& playerTex,
             const std::string& botTex, const std::string& bgImg,
             const std::string& musicFile);
    virtual ~GameMode() {}
    void runGame(); // loop chung
};

#endif
