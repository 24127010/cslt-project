#include "DifficultyMenu.h"
#include <iostream>

DifficultyMenu::DifficultyMenu(float width, float height) {
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
        std::cerr << "Không thể tải font!\n";

    if (!backgroundTexture.loadFromFile("assets/images/difficultymenu_background.jpg"))
        std::cerr << "Không thể tải ảnh nền!\n";
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u bgSize = backgroundTexture.getSize();
    backgroundSprite.setScale(800.f / bgSize.x, 600.f / bgSize.y);

    // ✅ Chỉ còn EASY, MEDIUM, HARD
    std::vector<std::string> labels = { "EASY", "MEDIUM", "HARD" };
    for (int i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(36);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setPosition(width / 2.f - 100, height / 2.f + i * 60 - 50);
        options.push_back(text);
    }
    selectedIndex = 0;

    // setup exitText
    exitText.setFont(font);
    exitText.setString("Exit: O");
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(10.f, height - 30.f);  // góc trái dưới
}

void DifficultyMenu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    for (auto& text : options) window.draw(text);
    window.draw(exitText);
}

void DifficultyMenu::moveUp() {
    if (selectedIndex > 0) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void DifficultyMenu::moveDown() {
    if (selectedIndex < options.size() - 1) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

int DifficultyMenu::getSelectedIndex() const { return selectedIndex; }

BotDifficulty DifficultyMenu::getDifficulty() const {
    switch (selectedIndex) {
        case 0: return BotDifficulty::Easy;
        case 1: return BotDifficulty::Medium;
        case 2: return BotDifficulty::Hard;
        default: return BotDifficulty::Easy;
    }
}

void DifficultyMenu::handleSelection(sf::RenderWindow& window) {
    switch (selectedIndex) {
        case 0: { // EASY
            EasyMode game(window);
            game.runGame();
            break;
        }
        case 1: { // MEDIUM
            MediumMode game(window);
            game.runGame();
            break;
        }
        case 2: { // HARD
            HardMode game(window);
            game.runGame();
            break;
        }
    }
}
