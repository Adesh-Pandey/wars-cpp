#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
class gameover{
    public:
int displayGameOverScreen() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Over UI");
    sf::SoundBuffer gameover;
    if (!gameover.loadFromFile("PNG/oversound.wav")) {
        std::cerr << "Failed to load sound file." << std::endl;
        return 1;
    }

    // Create SFML sound and set its buffer
    sf::Sound over;
    over.setBuffer(gameover);
    // Play the sound
    over.play();
    // Load font
    sf::Font font;
    if (!font.loadFromFile("PNG/fonts.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return 1;
    }

    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("PNG/gamebg.png")) {
        std::cerr << "Failed to load game over texture." << std::endl;
        return 1;
    }

    // Create the sprite for the game over texture
    sf::Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setScale(3,3);
    gameOverSprite.setPosition(0, 0);
    //gameOverText.setPosition(300, 200);

    // Restart button
    sf::RectangleShape restartButton(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setPosition(300, 300);

    // Restart button text
    sf::Text restartText("Restart", font, 40);
    restartText.setFillColor(sf::Color(255, 59, 88));
    restartText.setPosition(350, 300);

    // Exit button
    sf::RectangleShape exitButton(sf::Vector2f(200, 50));
    exitButton.setFillColor(sf::Color(144, 12, 63));
    exitButton.setPosition(300, 400);

    // Exit button text
    sf::Text exitText("Exit", font, 40);
    exitText.setFillColor(sf::Color(255, 59, 88));
    exitText.setPosition(380, 400);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        // Add code to restart the game
                        std::cout << "Restarting game..." << std::endl;
                    }
                    if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        window.close();
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(gameOverSprite);
        window.draw(restartButton);
        window.draw(restartText);
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }

    return 0;
}};
int main(){
    gameover aab;
    aab.displayGameOverScreen();
    return 0;
}