// BaseGameMode.cpp
#include "BaseGameMode.h"
#include <iostream>

BaseGameMode::BaseGameMode(sf::RenderWindow& win,
                           const std::string& botTexture,
                           const std::string& bgImage,
                           const std::string& musicFile)
    : window(win),
      player("assets/images/player_1.png", "player_1", sf::Vector2f(100, 400)),
      bot(botTexture, "bot", sf::Vector2f(500, 320)),
      gameEnded(false)
{
    window.setFramerateLimit(60);

    // load nhạc
    if (!bgMusic.openFromFile(musicFile)) {
        std::cerr << "Không thể tải nhạc: " << musicFile << "\n";
    } else {
        bgMusic.setLoop(true);
        bgMusic.setVolume(50);
        bgMusic.play();
    }

    // background
    if (!backgroundTexture.loadFromFile(bgImage)) {
        background.setSize(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(80, 80, 80));
    } else {
        background.setTexture(&backgroundTexture);
        background.setSize(sf::Vector2f(800, 600));
    }

    // font
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Không thể load font arial.ttf\n";
    }
    vsText.setFont(font);
    vsText.setString("VS");
    vsText.setCharacterSize(40);
    vsText.setFillColor(sf::Color::White);
    vsText.setPosition(375, 30);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(250, 300);

    exitText.setFont(font);
    exitText.setString("EXIT: O");
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(10.f, 570.f);

    playerHealthBar.setSize(sf::Vector2f(200, 20));
    playerHealthBar.setFillColor(sf::Color::Red);
    playerHealthBar.setPosition(50, 50);

    botHealthBar.setSize(sf::Vector2f(200, 20));
    botHealthBar.setFillColor(sf::Color::Red);
    botHealthBar.setPosition(550, 50);
}

void BaseGameMode::runGame() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O) {
                bgMusic.stop();
                return; // thoát về menu
            }
            if (!gameEnded) {
                player.handleInput(sf::Keyboard::A, sf::Keyboard::D,
                                   sf::Keyboard::W, sf::Keyboard::J,
                                   sf::Keyboard::K, true);
            }
        }

        if (!gameEnded) {
            player.update(deltaTime);
            aiControlBot(deltaTime); // gọi AI riêng
            bot.update(deltaTime);

            bool isColliding = player.getBounds().intersects(bot.getBounds());

            if (isColliding && player.isCurrentlyAttacking()) {
                bot.takeDamage(8);
                bot.setHit();
            }
            if (isColliding && bot.isCurrentlyAttacking()) {
                player.takeDamage(8);
                player.setHit();
            }

            playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
            botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

            if (player.isDead()) { gameOverText.setString("Bot Wins"); gameEnded = true; }
            else if (bot.isDead()) { gameOverText.setString("Player Wins"); gameEnded = true; }
        }

        

        window.clear();
        window.draw(background);
        window.draw(playerHealthBar);
        window.draw(botHealthBar);
        window.draw(vsText);
        player.render(window);
        bot.render(window);
        if (gameEnded) window.draw(gameOverText);
        window.draw(exitText);
        window.display();
    }
}
