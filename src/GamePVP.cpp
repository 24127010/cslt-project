#include "GamePVP.h"
#include <iostream>

PVPMode::PVPMode(sf::RenderWindow& win)
    : BaseGameMode(win, "assets/images/player_2.png", "assets/images/background.jpg", "assets/sounds/pvp.ogg")
{
    // Sử dụng trực tiếp biến player và bot từ lớp cha để đảm bảo đồng bộ với thanh máu
    player.getSprite().setPosition(sf::Vector2f(100.f, 400.f));
    bot.getSprite().setPosition(sf::Vector2f(600.f, 400.f));
    bot.setImagePrefix("player_2"); 
    gameEnded = false;
}

void PVPMode::aiControlBot(float dt) {}

void PVPMode::runGame() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) return; 
        }

        if (!gameEnded) {
            // Player 1: WASD, F (đấm), G (đá), Q (Parry)
            player.handleInput(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, 
                               sf::Keyboard::F, sf::Keyboard::G, sf::Keyboard::Q, true);

            // Player 2: Mũi tên, Num1, Num2, Num3 (Parry)
            bot.handleInput(sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, 
                            sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Keyboard::Numpad3, false);

            // CHỈ GỌI update ĐỂ XỬ LÝ SÁT THƯƠNG TẠI ĐÂY
            update(dt);
        }
        render();
    }
}

void PVPMode::update(float dt) {
    player.update(dt);
    bot.update(dt);

    if (!gameEnded) {
        bool va_cham = player.getBounds().intersects(bot.getBounds());

        if (va_cham) {
            // PLAYER 1 ĐÁNH: 
            // Điều kiện isCurrentlyAttacking() chỉ đúng khi phím đấm/đá đã được nhấn và gọi qua setAction
            if (player.isCurrentlyAttacking() && player.getAttackClock().getElapsedTime().asSeconds() > 0.1f) {
                if (!bot.isParrying()) {
                    bot.takeDamage(15); // Tăng sát thương cho dễ thấy
                    bot.setHit();
                } else {
                    player.setHit(); // P1 bị khựng nếu P2 đang hóa vàng
                }
                player.getAttackClock().restart();
            }

            // PLAYER 2 ĐÁNH:
            if (bot.isCurrentlyAttacking() && bot.getAttackClock().getElapsedTime().asSeconds() > 0.1f) {
                if (!player.isParrying()) {
                    player.takeDamage(15);
                    player.setHit();
                } else {
                    bot.setHit();
                }
                bot.getAttackClock().restart();
            }
        }
    }

    // Cập nhật thanh máu (Nhân 2 hoặc giữ nguyên tùy độ dài thanh máu của bạn)
    playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
    botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

    if (player.isDead()) {
        gameOverText.setString("PLAYER 2 WINS");
        gameEnded = true;
    } else if (bot.isDead()) {
        gameOverText.setString("PLAYER 1 WINS");
        gameEnded = true;
    }
}

void PVPMode::render() {
    window.clear();
    window.draw(background);
    window.draw(playerHealthBar);
    window.draw(botHealthBar);
    window.draw(vsText);
    window.draw(exitText);
    player.render(window);
    bot.render(window);
    if (gameEnded) window.draw(gameOverText);
    window.display();
}