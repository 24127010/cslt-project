#include "GameBotMedium.h"
#include <cmath>
#include <cstdlib>

MediumMode::MediumMode(sf::RenderWindow& win)
    : BaseGameMode(win, "assets/images/mediumbot2.png", "assets/images/mediumbg.jpg", "assets/sounds/mediumMode.ogg")
{
    bot.setImagePrefix("mediumbot2"); // Thêm hàm setImagePrefix để đổi prefix
}

void MediumMode::update(float dt) {
    player.update(dt);
    bot.update(dt);
    aiControlBot(dt);
}

void MediumMode::render() {
    window.draw(background);
    player.render(window);
    bot.render(window);
}


void MediumMode::aiControlBot(float dt) {
    if (bot.getIsHit() || bot.getIsAttacking()) {
        return; // Thoát hàm, chờ trạng thái bị đánh kết thúc
    }
    sf::FloatRect playerBounds = player.getBounds();
    sf::FloatRect botBounds = bot.getBounds();
    float distanceX = playerBounds.left - botBounds.left;
    bool botOnGround = bot.getVelocity().y == 0 && bot.getSprite().getPosition().y + bot.getSprite().getGlobalBounds().height >= 600;

    // Nếu khoảng cách lớn, ưu tiên di chuyển đến gần player
    if (abs(distanceX) > 150) {
        if (distanceX > 0) {
            bot.setAction(Player::ActionType::RunRight);  // Di chuyển về phía player
        } else {
            bot.setAction(Player::ActionType::RunLeft);  // Di chuyển về phía player
        }
    }
    // Nếu khoảng cách trung bình, kết hợp di chuyển và tấn công
    else if (abs(distanceX) > 50) {
        int actionChance = rand() % 100;

        // Kiểm tra nếu bot có cơ hội tấn công (25%)
        if (actionChance < 30 && bot.getAttackClock().getElapsedTime().asSeconds() > 1.0f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        }
        // Nếu bot ở trên mặt đất và có cơ hội nhảy (20%)
        else if (actionChance < 40 && botOnGround) {
            bot.setAction(Player::ActionType::Jump);
        }
        else {
            // Nếu không tấn công, di chuyển về phía player
            if (distanceX > 0) {
                bot.setAction(Player::ActionType::RunRight);
            } else {
                bot.setAction(Player::ActionType::RunLeft);
            }
        }
    }
    // Nếu gần player, ưu tiên tấn công
    else {
        int actionChance = rand() % 100;

        // Kiểm tra nếu bot có cơ hội tấn công (50%)
        if (actionChance < 50 && bot.getAttackClock().getElapsedTime().asSeconds() > 1.5f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        }
        // Nếu bot có cơ hội nhảy (80%)
        else if (actionChance < 80 && botOnGround) {
            bot.setAction(Player::ActionType::Jump);
        }
        else {
            // Nếu không tấn công, đứng yên
            bot.setAction(Player::ActionType::Idle);
        }
    }
}
