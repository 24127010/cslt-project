// main.cpp
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "DifficultyMenu.h"
#include "GameBotEasy.h"
#include "GameBotMedium.h"
#include "GameBotHard.h"
#include "GamePVP.h"

void runGamePVP(sf::RenderWindow& window) {
    PVPMode game(window); // Khởi tạo PVPMode
    game.runGame();       // Gọi phương thức runGame của PVPMode
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "US FIGHTERS");

    Menu mainMenu(800, 600);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                // Điều hướng menu chính
                if (event.key.code == sf::Keyboard::Up)
                    mainMenu.moveUp();
                else if (event.key.code == sf::Keyboard::Down)
                    mainMenu.moveDown();
                else if (event.key.code == sf::Keyboard::Enter) {
                    int choice = mainMenu.getSelectedIndex();

                    switch (choice) {
                        case 0: { // Chơi với bot → mở menu chọn difficulty
                            DifficultyMenu diffMenu(800, 600);
                            bool choosing = true;

                            while (choosing && window.isOpen()) {
                                sf::Event diffEvent;
                                while (window.pollEvent(diffEvent)) {
                                    if (diffEvent.type == sf::Event::Closed) {
                                        window.close();
                                    } else if (diffEvent.type == sf::Event::KeyPressed) {
                                        if (diffEvent.key.code == sf::Keyboard::Up)
                                            diffMenu.moveUp();
                                        else if (diffEvent.key.code == sf::Keyboard::Down)
                                            diffMenu.moveDown();
                                        
                                        else if (diffEvent.key.code == sf::Keyboard::Enter) {
                                            choosing = false;
                                            // Chạy mode tương ứng
                                            switch (diffMenu.getDifficulty()) {
                                                case BotDifficulty::Easy: {
                                                    EasyMode easyMode(window);
                                                    easyMode.runGame();
                                                    break;
                                                }
                                                case BotDifficulty::Medium: {
                                                    MediumMode mediumMode(window);
                                                    mediumMode.runGame();
                                                    break;
                                                }
                                                case BotDifficulty::Hard: {
                                                    HardMode hardMode(window);
                                                    hardMode.runGame();
                                                    break;
                                                }
                                            }
                                        }
                                        else if (diffEvent.key.code == sf::Keyboard::O) {
                                             // 👉 Quay về menu chính 
                                            choosing = false; }
                                    }
                                }
                                window.clear();
                                diffMenu.draw(window);
                                window.display();
                            }
                            break;
                        }

                        case 1: { // PVP mode
                            runGamePVP(window);
                            break;
                        }

                        case 2: { // Exit
                            window.close();
                            break;
                        }
                    }
                }
            }
        }

        window.clear();
        mainMenu.draw(window);
        window.display();
    }

    return 0;
}
