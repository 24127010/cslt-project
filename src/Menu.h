#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>

class Menu {
private:
    std::vector<sf::Text> options;
    sf::Font font;
    int selectedIndex;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
};

#endif
