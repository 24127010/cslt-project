#include "Menu.h"
#include <iostream>

Menu::Menu(float width, float height) {
    // Load font
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Không thể tải font cho menu!\n";
    }

    // Load background image
    if (!bgTexture.loadFromFile("assets/images/menu_background.jpg")) {
        std::cerr << "Không thể tải menu_bg.jpg\n";
    }

    bgSprite.setTexture(bgTexture);
    sf::Vector2u bgSize = bgTexture.getSize();
    bgSprite.setScale(
        width / static_cast<float>(bgSize.x),
        height / static_cast<float>(bgSize.y)
    );

    std::vector<std::string> labels = {
        "PLAY",         // sau này chơi với bot
        "PLAYER vs PLAYER",
        "EXIT"
    };

    for (int i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(36);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setOutlineColor(sf::Color::Black);         // Viền đen
        text.setOutlineThickness(2.f);                  // Độ dày viền
        text.setPosition(width / 2.f - 100, height / 2.f + i * 60 - 50);
        options.push_back(text);
    }

    selectedIndex = 0;
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(bgSprite);  // Vẽ background trước
    for (auto& text : options)
        window.draw(text);  // Vẽ chữ đè lên
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (selectedIndex < options.size() - 1) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

int Menu::getSelectedIndex() const {
    return selectedIndex;
}
