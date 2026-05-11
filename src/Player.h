#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

class Player {
public: 
    enum class ActionType { Idle, Kick, Punch, Jump, RunLeft, RunRight, Hitten, Parry };



private:
    sf::Sprite sprite;
    sf::Texture texture;
    std::string imagePrefix;

    sf::Vector2f velocity;
    bool isHit = false;
    sf::Clock hitClock;

    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarFront;

    bool isAttacking = false;
    sf::Clock actionClock;
    sf::Clock attackClock; // Thêm để quản lý cooldown tấn công

    float speed;
    int hp;
    bool isOnGround;
    sf::Clock damageCooldown; //code moi them
    ActionType currentAction;

public:
    //enum class ActionType { Idle, Kick, Punch, Jump, RunLeft, RunRight, Hitten };
    Player(const std::string& textureFile, const std::string& prefix, sf::Vector2f startPos);

    // Trong file Player.h
    void handleInput(sf::Keyboard::Key trai, sf::Keyboard::Key phai, sf::Keyboard::Key nhay,
                     sf::Keyboard::Key dam, sf::Keyboard::Key da, sf::Keyboard::Key do_don, 
                     bool isPlayer1);
    void botAI(const Player& opponent, float deltaTime);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    void takeDamage(int amount);
    bool isDead() const;
    sf::FloatRect getBounds() const;
    void setHit();
    void setAction(ActionType type);
    sf::Clock& getDamageCooldown();
    sf::Clock& getAttackClock(); // Thêm để lấy attackClock
    sf::Vector2f getVelocity() const; 
    int getHealth() const;
    sf::Sprite& getSprite();
    bool isCurrentlyAttacking() const; // Kiểm tra xem nhân vật có đang tấn công không
    ActionType getCurrentAction() const;
	sf::Vector2f getPosition() const;
	void attack();  // Phương thức tấn công
    void jump();    // Phương thức nhảy
    void setImagePrefix(const std::string& prefix);
    bool getIsHit() const { return isHit; } 
    bool getIsAttacking() const { return isAttacking; }
    bool isParrying() const { return currentAction == ActionType::Parry; }
};

#endif