#include "GameBotEasy.h"
#include <cstdlib>
#include <cmath>

EasyMode::EasyMode(sf::RenderWindow& win)
    : BaseGameMode(win, 
                   "assets/images/easybot.png",  // Bot texture
                   "assets/images/easybg.jpg",   // Background image
                   "assets/sounds/easymode.ogg") // Music file
{
    bot.setImagePrefix("easybot"); // Thêm hàm setImagePrefix để đổi prefix
}

void EasyMode::aiControlBot(float deltaTime) {
    sf::FloatRect playerBounds = player.getBounds();
    sf::FloatRect botBounds = bot.getBounds();
    float distanceX = playerBounds.left - botBounds.left;

    bool botOnGround = bot.getVelocity().y == 0 &&
        bot.getSprite().getPosition().y + bot.getSprite().getGlobalBounds().height >= 600;

    int actionChance = rand() % 100;

    if (std::abs(distanceX) > 120) {
        bot.setAction(distanceX > 0 ? Player::ActionType::RunRight
                                    : Player::ActionType::RunLeft);
    }
    else if (std::abs(distanceX) > 40) {
        if (actionChance < 20 &&
            bot.getAttackClock().getElapsedTime().asSeconds() > 2.5f) {
            bot.setAction(rand() % 2 == 0
                              ? Player::ActionType::Punch
                              : Player::ActionType::Kick);
        } else if (actionChance < 30 && botOnGround) {
            bot.setAction(Player::ActionType::Jump);
        } else {
            bot.setAction(Player::ActionType::Idle);
        }
    }
    else {
        if (bot.getAttackClock().getElapsedTime().asSeconds() > 2.5f) {
            bot.setAction(rand() % 2 == 0
                              ? Player::ActionType::Punch
                              : Player::ActionType::Kick);
        } else {
            bot.setAction(Player::ActionType::Idle);
        }
    }
}

void EasyMode::update(float dt) {
    // Cập nhật logic của người chơi
    player.handleInput(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, 
                       sf::Keyboard::J, sf::Keyboard::K, sf::Keyboard::Q, true);
    player.update(dt); // Cập nhật người chơi (di chuyển, hành động...)

    // Cập nhật bot (bot có thể tự động di chuyển hoặc hành động)
    aiControlBot(dt);  // Điều khiển hành động của bot

    // Cập nhật bot
    bot.update(dt);    // Cập nhật bot (di chuyển, hành động...)

    // Kiểm tra va chạm giữa người chơi và bot
    bool isColliding = player.getBounds().intersects(bot.getBounds());

    // Nếu người chơi tấn công và có va chạm với bot
    if (isColliding && player.isCurrentlyAttacking() &&
        player.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
        bot.takeDamage(8);        // Bot nhận sát thương
        bot.setHit();             // Đánh dấu bot bị trúng đòn
        player.getAttackClock().restart();  // Khởi động lại cooldown của người chơi
    }

    // Nếu bot tấn công và có va chạm với người chơi
    if (isColliding && bot.isCurrentlyAttacking() &&
        bot.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
        player.takeDamage(8);     // Người chơi nhận sát thương
        player.setHit();          // Đánh dấu người chơi bị trúng đòn
        bot.getAttackClock().restart();    // Khởi động lại cooldown của bot
    }

    // Cập nhật thanh máu của người chơi và bot
    playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
    botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

    // Kiểm tra kết thúc game: nếu người chơi hoặc bot chết
    if (player.isDead()) {
        gameOverText.setString("EasyBot Wins");  // Thông báo bot thắng
        gameEnded = true;
    } else if (bot.isDead()) {
        gameOverText.setString("Player Wins");   // Thông báo người chơi thắng
        gameEnded = true;
    }
}
void EasyMode::render() {
    // Xóa màn hình và vẽ lại tất cả các đối tượng
    window.clear();

    // Vẽ nền game
    window.draw(background);

    // Vẽ thanh máu của người chơi và bot
    window.draw(playerHealthBar);
    window.draw(botHealthBar);

    // Vẽ chữ "VS" giữa người chơi và bot
    window.draw(vsText);

    // Vẽ nhân vật người chơi và bot
    player.render(window);
    bot.render(window);

    // Nếu game kết thúc, vẽ dòng chữ game over
    if (gameEnded) {
        window.draw(gameOverText);
    }

    // Vẽ thông báo thoát game
    window.draw(exitText);

    // Cập nhật cửa sổ để hiển thị những gì đã vẽ
    window.display();
}


