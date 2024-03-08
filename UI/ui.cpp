#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace sf;
class GAMEUI{
    private:
const float PLAYER_WIDTH = 50.f;
const float PLAYER_HEIGHT = 50.f;
const float PlayerStep = 0.05;
public:
int gameui() {
    // Initialize the SFML window
    RenderWindow window(VideoMode(800, 600), "Game Start UI");
 Texture backg;
    if (!backg.loadFromFile("PNG/bgimage.png")) {
        std::cerr<< "Failed to load player image." << std::endl;
        return 1;
    }

    // Create player sprite
    Sprite backgrounds(backg);
    backgrounds.setScale(2.5, 3);
    backgrounds.setPosition(0, 0);
    // Load font
    Font font;
    if (!font.loadFromFile("PNG/fonts.ttf")) {
        std::cerr<< "Failed to load font." << std::endl;
        return 1;
    }

    // Load player image
    Texture playerTexture;
    if (!playerTexture.loadFromFile("PNG/player_spaceship.png")) {
        std::cerr<< "Failed to load player image." << std::endl;
        return 1;
    }

    // Create player sprite
    Sprite logoSprite(playerTexture);
    logoSprite.setScale(0.5, 0.5);
    logoSprite.setPosition(325, 150);

    // Create start button
    RectangleShape startButton(Vector2f(200, 50));
    startButton.setFillColor(Color::Green);
    startButton.setPosition(300, 350);

    // Create exit button
    RectangleShape exitButton(Vector2f(200, 50));
    exitButton.setFillColor(sf::Color(255, 59, 88));
    exitButton.setPosition(300, 440);

    Text startText("Start", font, 45);
    startText.setFillColor(sf::Color(144, 12, 63));
    startText.setPosition(350, 340);

    // Create exit button text
    Text exitText("Exit", font, 35);
    exitText.setFillColor(sf::Color(144, 12, 63));
    exitText.setPosition(360, 440);

    // Load sound
    SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("PNG/sound_effect.wav")) {
        std::cerr<< "Failed to load sound effect" << std::endl;
        return 1;
    }

    // Create sound object
    Sound sound;
    sound.setBuffer(soundBuffer);
    sound.setLoop(true);
    sound.play();
    // Create sound button
    CircleShape soundButton(25);
    soundButton.setFillColor(Color::Black);
    soundButton.setPosition(0, 0);

    // Load sound image
Texture soundImageOn;
    if (!soundImageOn.loadFromFile("PNG/onbg.png")) {
        std::cerr << "Failed to load sound image (on)." << std::endl;
        return 1;
    }
    Texture soundImageOff;
    if (!soundImageOff.loadFromFile("PNG/offbg.png")) {
        std::cerr << "Failed to load sound image (off)." << std::endl;
        return 1;
    }

// Create sound sprite
Sprite soundSprite(soundImageOn);
soundSprite.setScale(0.2, 0.2); // adjust the scale as needed
soundSprite.setPosition(0, 0); // adjust the position as needed

    // Create a variable to store the state of the sound button
    bool isSoundOn = true;

    // Main game loop
    while (window.isOpen()) {
        window.clear();
        window.draw(backgrounds);
        // Draw all the objects on the screen
        window.draw(logoSprite);
        window.draw(startButton);
        window.draw(exitButton);
        window.draw(startText);
        window.draw(exitText);
        window.draw(soundButton);
        window.draw(soundSprite);

        // Display the window
        window.display();
        // Handle events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }


            // Check if start button is clicked
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePosition = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    // Start the game
                    // ...
        
                    printf("start game\n");
                }
            }
            // Change button color on hover
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                startButton.setFillColor(sf::Color(72, 140, 11 ));
            else
                startButton.setFillColor(sf::Color(127, 254, 14 ));

            if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                exitButton.setFillColor(sf::Color(72, 140, 11 ));
            else
                exitButton.setFillColor(sf::Color(255, 59, 88));

            // Check if exit button is clicked
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePosition = Mouse::getPosition(window);
                if (exitButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    // Exit the game
                    window.close();
                }
            }

            // Handle sound button click event
                        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePosition = Mouse::getPosition(window);
                if (soundButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    // Toggle the state of the sound button
                    isSoundOn = !isSoundOn;

                    // Play sound if sound is on
                    if (isSoundOn) {
                        sound.play();
                    } else {
                        // Pause sound if sound is off
                        sound.pause();
                    }

                    // Update the sound sprite
                    if (isSoundOn) {
                        soundSprite.setTexture(soundImageOn);
                    } else {
                        soundSprite.setTexture(soundImageOff);
                    }
                }
            }
        }
        }
        return 0;
}};
int main(){
    GAMEUI aab;
    aab.gameui();
    return 0;
}