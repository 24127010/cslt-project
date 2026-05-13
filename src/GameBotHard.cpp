#include "GameBotHard.h"
#include <cmath>
#include <cstdlib>

HardMode::HardMode(sf::RenderWindow& win)
    : BaseGameMode(win, "assets/images/hardbot.png", "assets/images/hardbg.jpg", "assets/sounds/hardmode.ogg")
{
    bot.setImagePrefix("hardbot"); // Thêm hàm setImagePrefix để đổi prefix
}

void HardMode::aiControlBot(float dt) {
    sf::Vector2f dir = player.getPosition() - bot.getPosition();
    float distanceX = std::abs(dir.x);

    if (distanceX > 60) {
        // Áp sát player
        bot.getSprite().move((dir.x > 0 ? 1 : -1) * 200.f * dt, 0);  
    } 
    else {
        // Random hành động
        int action = rand() % 100;
        if (action < 40) { 
            bot.attack();   // Tấn công nhiều hơn
        } else if (action < 60) {
            bot.jump();     // Thi thoảng nhảy né
        } else {
            bot.setAction(Player::ActionType::Idle);
        }
    }
}

void HardMode::update(float dt) {
    player.update(dt);
    bot.update(dt);
    aiControlBot(dt);

    // Xử lý va chạm & đòn đánh
    if (player.getBounds().intersects(bot.getBounds())) {
        if (player.isCurrentlyAttacking()) {
            bot.takeDamage(10);
        }
        if (bot.isCurrentlyAttacking()) {
            player.takeDamage(10);
        }
    }
}

void HardMode::render() {
    window.clear(sf::Color::Black);
    window.draw(background);
    player.render(window);
    bot.render(window);
    window.display();
}
