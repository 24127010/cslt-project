// BaseGameMode.h
#pragma once
#include "IGameMode.h"

class BaseGameMode : public IGameMode {
protected:
    sf::RenderWindow& window;
    Player player;
    Player bot;

    sf::Music bgMusic;
    sf::Texture backgroundTexture;
    sf::RectangleShape background;

    sf::Font font;
    sf::Text vsText, gameOverText, exitText;

    sf::RectangleShape playerHealthBar, botHealthBar;

    sf::Clock clock;
    bool gameEnded;

public:
    BaseGameMode(sf::RenderWindow& win,
                 const std::string& botTexture,
                 const std::string& bgImage,
                 const std::string& musicFile);

    virtual void runGame() override;   // vòng lặp game chung
    virtual void aiControlBot(float dt) = 0; // vẫn để thuần ảo, mỗi mode định nghĩa
    virtual void update(float dt) = 0;   // Đảm bảo khai báo virtual
    virtual void render() = 0;           // Đảm bảo khai báo virtual
};
