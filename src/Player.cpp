#include "Player.h"
#include <iostream>

Player::Player(const std::string& textureFile, const std::string& prefix, sf::Vector2f startPos)
    : imagePrefix(prefix) {
    isOnGround = false;

    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Lỗi: Không thể load " << textureFile << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(startPos);
    speed = 200.f;
    hp = 200;
    velocity = {0.f, 0.f};
	currentAction = ActionType::Idle; 
    hpBarBack.setSize({50.f, 5.f});
    hpBarBack.setFillColor(sf::Color::Black);

    hpBarFront.setSize({50.f, 5.f});
    hpBarFront.setFillColor(sf::Color::Green);
}

void Player::handleInput(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key jump,
                         sf::Keyboard::Key punch, sf::Keyboard::Key kick, 
                         sf::Keyboard::Key parry, bool isPlayer1) {
    
    velocity.x = 0.f;

    // 1. Logic Parry (Đỡ đòn) - Phải xử lý cả hai trường hợp Nhấn và Thả
    if (sf::Keyboard::isKeyPressed(parry)) {
        setAction(ActionType::Parry);
        return; // Đang giữ phím thì thoát hàm, giữ màu vàng và không nhận input khác
    } 
    else {
        // Nếu không nhấn phím parry mà nhân vật vẫn đang ở trạng thái Parry
        // thì phải trả về Idle để reset màu và trạng thái
        if (currentAction == ActionType::Parry) {
            sprite.setColor(sf::Color::White); // Trả màu về trắng ngay lập tức
            setAction(ActionType::Idle);
        }
    }

    // 2. Ngăn input di chuyển nếu đang trong animation tấn công (để bảo vệ đòn đánh)
    if (isAttacking) return;

    // 3. Kiểm tra Tấn công
    if (sf::Keyboard::isKeyPressed(punch)) {
        setAction(ActionType::Punch);
        return;
    } else if (sf::Keyboard::isKeyPressed(kick)) {
        setAction(ActionType::Kick);
        return;
    }

    // 4. Kiểm tra Di chuyển
    if (sf::Keyboard::isKeyPressed(left)) {
        velocity.x = -speed;
        setAction(ActionType::RunLeft);
    } else if (sf::Keyboard::isKeyPressed(right)) {
        velocity.x = speed;
        setAction(ActionType::RunRight);
    } else if (isOnGround) {
        setAction(ActionType::Idle);
    }

    // 5. Kiểm tra Nhảy
    if (sf::Keyboard::isKeyPressed(jump) && isOnGround) {
        velocity.y = -300.f;
        isOnGround = false;
        setAction(ActionType::Jump);
    }
}

void Player::update(float deltaTime) {
    // Áp dụng trọng lực
    velocity.y += 500.f * deltaTime;
    if (isAttacking) velocity.x = 0.f;
    sprite.move(velocity * deltaTime);

    // Giới hạn không cho đi ra khỏi màn hình (X)
    float spriteWidth = sprite.getGlobalBounds().width;
    if (sprite.getPosition().x < 0) {
        sprite.setPosition(0, sprite.getPosition().y);
    }
    if (sprite.getPosition().x > 800 - spriteWidth) {
        sprite.setPosition(800 - spriteWidth, sprite.getPosition().y);
    }

    // Giới hạn dưới (ground)
    float groundY = 600.f;  
	float spriteHeight = sprite.getGlobalBounds().height;  
	if (sprite.getPosition().y + spriteHeight >= groundY) {
	    sprite.setPosition(sprite.getPosition().x, groundY - spriteHeight);
	    velocity.y = 0;
	    isOnGround = true;
	} else {
        isOnGround = false;
    }

    // Hết trạng thái bị đánh trúng sau 0.2s
    if (isHit && hitClock.getElapsedTime().asSeconds() > 0.2f) {
        isHit = false;
        sprite.setColor(sf::Color::White);
    }

    // Hết trạng thái tấn công sau 0.2s (giữ đúng bản cũ)
    if (isAttacking && actionClock.getElapsedTime().asSeconds() > 0.4f) { 
        isAttacking = false;
        setAction(ActionType::Idle); // Ép về Idle để sẵn sàng cho lần đấm tiếp theo
    }

    // Hết trạng thái bị trúng đòn (giữ nguyên)
    if (isHit && hitClock.getElapsedTime().asSeconds() > 0.2f) {
        isHit = false;
        sprite.setColor(sf::Color::White);
    }
}


void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);

    sf::Vector2f pos = sprite.getPosition();
    hpBarBack.setPosition(pos.x, pos.y - 10);
    hpBarFront.setPosition(pos.x, pos.y - 10);

    hpBarFront.setSize({50.f * (hp / 200.f), 5.f});
    window.draw(hpBarBack);
    window.draw(hpBarFront);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::takeDamage(int amount) {
	if (damageCooldown.getElapsedTime().asSeconds() < 0.5f) return; 
    hp -= amount;
    if (hp < 0) hp = 0;
    setAction(ActionType::Hitten);
    damageCooldown.restart();  
}

bool Player::isDead() const {
    return hp <= 0;
}

void Player::setHit() {
    isHit = true;
    sprite.setColor(sf::Color::Red);
    hitClock.restart();
}

sf::Clock& Player::getDamageCooldown() {
    return damageCooldown;
} // code moi them

sf::Vector2f Player::getVelocity() const {
    return velocity;
}

int Player::getHealth() const {
    return hp;
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();  // Trả về vị trí của sprite
}

sf::Sprite& Player::getSprite() { // Định nghĩa hàm getSprite
    return sprite;
}

bool Player::isCurrentlyAttacking() const {
    return isAttacking; 
}

Player::ActionType Player::getCurrentAction() const { // Thêm để bot phân tích hành động player1
    return currentAction;
}

sf::Clock& Player::getAttackClock() {
    return attackClock;
}


void Player::setAction(ActionType type) {
    currentAction = type;
    std::string path = "assets/images/" + imagePrefix;

    switch (type) {
        case ActionType::Idle:
            texture.loadFromFile(path + ".png");
            velocity.x = 0.f;// code moi
            isAttacking = false;
            break;
        case ActionType::Kick:
            texture.loadFromFile(path + "_kick.png");
            velocity.x = 0.f; // Ngung di chuyen khi da
            isAttacking = true;
            actionClock.restart();
            attackClock.restart(); 
            break;
        case ActionType::Punch:
            texture.loadFromFile(path + "_punch.png");
            velocity.x = 0.f; // Ngung di chuyen khi danh
            isAttacking = true;
            actionClock.restart();
            attackClock.restart(); 
            break;
        case ActionType::Jump:
            texture.loadFromFile(path + "_jump.png");
            break;
        case ActionType::RunLeft:
            texture.loadFromFile(path + "_runleft.png");
            velocity.x = -speed;
            break;
        case ActionType::RunRight:
            texture.loadFromFile(path + "_runright.png");
            velocity.x = speed;
            break;
        case ActionType::Hitten:
            texture.loadFromFile(path + "_hitten.png");
            velocity.x = 0.f; // Ngung di chuyen khi bi danh
            isAttacking = false; // Coi như cũng đang "animation" tạm thời
            isHit = true;
            hitClock.restart();
            break;
        case ActionType::Parry:
            if (!texture.loadFromFile(path + "_parry.png")) {
                texture.loadFromFile(path + ".png");
                sprite.setColor(sf::Color(255, 215, 0));
            }
            velocity.x = 0.f;
            isAttacking = false;
            break;
    }

    sprite.setTexture(texture);
}
void Player::attack() {
    // Nếu chưa tấn công, thiết lập trạng thái tấn công
    if (!isAttacking) {
        isAttacking = true;
        setAction(ActionType::Punch);  // hoặc Kick tùy vào loại tấn công
        actionClock.restart();  // Thiết lập lại đồng hồ đếm thời gian
    }
}

void Player::jump() {
    // Nếu chưa nhảy và đang ở trên mặt đất
    if (isOnGround) {
        velocity.y = -300.f;  // Thiết lập vận tốc cho nhảy
        isOnGround = false;
        setAction(ActionType::Jump);  // Thiết lập hành động là nhảy
    }
}

void Player::setImagePrefix(const std::string& prefix) {
    imagePrefix = prefix;
}