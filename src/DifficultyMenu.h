// DifficultyMenu.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameBotEasy.h"
#include "GameBotMedium.h"
#include "GameBotHard.h"
#include "GamePVP.h"

enum class BotDifficulty { Easy, Medium, Hard };

class DifficultyMenu {
public:
    DifficultyMenu(float width, float height);

    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
    BotDifficulty getDifficulty() const;
    void handleSelection(sf::RenderWindow& window);

private:
    std::vector<sf::Text> options;
    sf::Font font;
    int selectedIndex;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text exitText;
};
