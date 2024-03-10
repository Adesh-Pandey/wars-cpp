#include "SFML/System/Vector2.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream> // Include this header for stringstream
#include <stdexcept>
#include <vector>

using namespace sf;

using namespace std;

const float PLAYER_WIDTH = 50.f;
const float PLAYER_HEIGHT = 50.f;

const float ENEMY_WIDTH = 50.f;
const float ENEMY_HEIGHT = 50.f;

const float PlayerStep = 3.f;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

const float BULLET_SIZE = 3.f;
const float ENEMY_STEP = 0.08f;
const float BULLET_STEP = 0.25;
enum GameState {
  STARTING,
    RUNNING,
    GAME_OVER
};

GameState gameState = GameState::STARTING;

class Coord {
public:
  float x = 0.f;
  float y = 0.f;
  Coord(float a, float b) {
    x = a;
    y = b;
  }
};

enum PlayerMovementDirection { UP = 1, DOWN, LEFT, RIGHT };

class Entity {
public:
  // Virtual destructor to ensure proper cleanup of derived classes
  virtual ~Entity() {}

  // Pure virtual function for movement - to be implemented in derived classes
  virtual void move(float dx, float dy) = 0;

  // Additional pure virtual functions can be declared here
  // For example, update or render methods
  virtual void update() = 0;
  virtual void render(RenderWindow &window) = 0;

  // Other common functionalities for entities can be added here
  // For instance, getters or setters for position, velocity, etc.
};

class Player : public Entity {
private:
  sf::Texture texture;
  Sprite playerSprite;

public:
  Player() {

    // Load a playerSprite to display
    if (!texture.loadFromFile("player_spaceship.png")) {
      throw std::runtime_error("The texture is not loaded");
      return;
    }

    playerSprite.setTexture(texture);

    Vector2u textureSize = texture.getSize();

    // Calculate the scale factors
    float scaleX = PLAYER_WIDTH / textureSize.x;
    float scaleY = PLAYER_HEIGHT / textureSize.y;

    // Set the scale of the playerSprite
    playerSprite.setPosition(300, 400);
    playerSprite.setScale(scaleX, scaleY);
  }
  void resetCoordinate(){
        playerSprite.setPosition(300, 400);
  }
  FloatRect getGlobalBounds(){
    return playerSprite.getGlobalBounds();
  }
  void move(float dx, float dy) override { playerSprite.move(dx, dy); }

  void changeInCoord(float dx, float dy) {
    Vector2f oldPos = playerSprite.getPosition();

    const float oldX = oldPos.x;

    const float oldY = oldPos.y;

    if (oldX + dx > ScreenWidth - 20 - PLAYER_WIDTH || oldX + dx <= 20) {
      return;
    }

    if (oldY + dy > ScreenHeight - 20 - PLAYER_HEIGHT || oldY + dy <= 20) {
      return;
    }
    move(dx, dy);
  }

  void keyPressedForMovement(PlayerMovementDirection key) {
    switch (key) {
    case PlayerMovementDirection::LEFT:
      changeInCoord(-1 * PlayerStep, 0.f);
      break;

    case PlayerMovementDirection::RIGHT:
      changeInCoord(PlayerStep, 0.f);

      break;

    case PlayerMovementDirection::DOWN:
      changeInCoord(0.f, PlayerStep);

      break;

    case PlayerMovementDirection::UP:
      changeInCoord(0.f, -1 * PlayerStep);

      break;

    default:
      break;
    }
  }

  void update() override {
    // Implement how the player updates (e.g., handle input, collisions)
  }

  void render(RenderWindow &window) override {
    // Draw the player's sprite to the screen
    window.draw(playerSprite);
  }

  Vector2f getCoord() {
    Vector2f pos = playerSprite.getPosition();

    return Vector2f(pos.x + PLAYER_WIDTH / 2, pos.y + PLAYER_HEIGHT / 2);
  }

  Sprite getSprite() { return playerSprite; }
};

void whichKeyPressedCheck(Player *player, Keyboard::Key keyCode) {
  switch (keyCode) {
  case Keyboard::Key::Down:
    player->keyPressedForMovement(PlayerMovementDirection::DOWN);
    break;

  case Keyboard::Key::Up:
    player->keyPressedForMovement(PlayerMovementDirection::UP);
    break;

  case Keyboard::Key::Left:
    player->keyPressedForMovement(PlayerMovementDirection::LEFT);
    break;

  case Keyboard::Key::Right:
    player->keyPressedForMovement(PlayerMovementDirection::RIGHT);
    break;

  default:
    break;
  }
}

Coord getPlayerCenterFromCoord(Vector2f coord) {
  cout << " Player coord (" << coord.x << " , " << coord.y << " )" << endl;
  return Coord(coord.x + PLAYER_WIDTH / 2.0f + 200.f,
               coord.y + PLAYER_HEIGHT / 2.0f);
}

class Bullet {
  Texture texture;
  Sprite bulletSprite;

public:
  Bullet(float x, float y) {
    // Load a playerSprite to display
    if (!texture.loadFromFile("bullet.png")) {
      throw std::runtime_error("The texture is not loaded");
      return;
    }

    bulletSprite.setTexture(texture);

    Vector2u textureSize = texture.getSize();

    // Calculate the scale factors
    float scaleX = BULLET_SIZE / textureSize.x;
    float scaleY = BULLET_SIZE / textureSize.y;

    // Set the scale of the playerSprite
    bulletSprite.setPosition(x, y);

    bulletSprite.setScale(scaleX, scaleY);
  }

  // for initialization in array
  Bullet() {}

  void move() { bulletSprite.move(0, -BULLET_STEP); }
  void render(RenderWindow &window) {
    // Draw the player's sprite to the screen
    window.draw(bulletSprite);
  }
  FloatRect getGlobalBounds(){
    return bulletSprite.getGlobalBounds();
  }
  Vector2f getCoord() {
    Vector2f pos = bulletSprite.getPosition();

    return Vector2f(pos.x + BULLET_SIZE / 2, pos.y + BULLET_SIZE / 2);
  }
};

class Enemy {

private:
  Texture texture;
  Sprite enemySprite;

public:
  Enemy(Coord coord) {

    // Load a playerSprite to display
    if (!texture.loadFromFile("player_spaceship.png")) {
      throw std::runtime_error("The texture is not loaded");
      return;
    }

    enemySprite.setTexture(texture);

    Vector2u textureSize = texture.getSize();

    // Calculate the scale factors
    float scaleX = ENEMY_WIDTH / textureSize.x;
    float scaleY = ENEMY_HEIGHT / textureSize.y;

    // Set the scale of the playerSprite
    enemySprite.setPosition(coord.x, coord.y);
    enemySprite.setScale(scaleX, scaleY);
  }

  void move() { enemySprite.move(0, ENEMY_STEP); }
  void render(RenderWindow &window) {
    // Draw the player's sprite to the screen
    window.draw(enemySprite);
  }
  FloatRect getGlobalBounds(){
    return enemySprite.getGlobalBounds();
  }

  Vector2f getCoord() {
    Vector2f pos = enemySprite.getPosition();

    return Vector2f(pos.x + ENEMY_WIDTH / 2, pos.y + ENEMY_HEIGHT / 2);
  }
};

Coord randomCoordinateAboveScreenForEnemySpwan() {

  return Coord(rand() % (800 - (int)ENEMY_HEIGHT), -1 * rand() % 1000);
}

bool shouldRemoveBullet(Bullet bb) {
  const Vector2f coord = bb.getCoord();
  if (coord.y < -20) {
    return true;
  }
  return false;
}
bool shouldRemoveEnemy(Enemy ee) {
  const Vector2f coord = ee.getCoord();
  if (coord.y > 620) {
    return true;
  }
  return false;
}
bool isCollision(Bullet& sprite1, Enemy& sprite2) {
    // Get bounding boxes of sprites
    sf::FloatRect boundingBox1 = sprite1.getGlobalBounds();
    sf::FloatRect boundingBox2 = sprite2.getGlobalBounds();

    // Check for overlap
    if (boundingBox1.intersects(boundingBox2)) {
        return true; // Collision detected
    }

    return false; // No collision
}
bool isCollision(Player& player, Enemy& enemy) {
    // Get bounding boxes of sprites
    sf::FloatRect playerBounds = player.getGlobalBounds();
    sf::FloatRect enemyBounds = enemy.getGlobalBounds();

    // Check for overlap
    if (playerBounds.intersects(enemyBounds)) {
        return true; // Collision detected
    }

    return false; // No collision
}



int gameRunning(sf::RenderWindow& window, Player& player, vector<Bullet>& bulletBuffer, int& levelEmemyCount, vector<Enemy>& activeEnemyList,sf::Font& font,int& score){
       // Score value
  

    // Text object for displaying score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    Event event;
    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == Event::KeyPressed &&
                 event.key.code == Keyboard::Space) {

        // add bullet in the bullet buffer
        const Vector2f currentPlayerCoord = player.getCoord();

        bulletBuffer.push_back(
            Bullet(currentPlayerCoord.x, currentPlayerCoord.y));
      }
    

    if (event.type == Event::KeyPressed) {
      whichKeyPressedCheck(&player, event.key.code);
    }
    }

    const int diff = levelEmemyCount - activeEnemyList.size();

    if (diff > 0) {
      for (int i = 0; i < diff; i++) {
        activeEnemyList.push_back(
            Enemy(randomCoordinateAboveScreenForEnemySpwan()));
      }
    }

    bulletBuffer.erase(
        remove_if(bulletBuffer.begin(), bulletBuffer.end(), shouldRemoveBullet),
        bulletBuffer.end());

    activeEnemyList.erase(remove_if(activeEnemyList.begin(),
                                    activeEnemyList.end(), shouldRemoveEnemy),
                          activeEnemyList.end());
 for (Enemy& enemy : activeEnemyList) {
        if (isCollision(player, enemy)) {
            // Game over logic
            gameState = GameState::GAME_OVER;
            break;
        }
    }
 
for (auto it = bulletBuffer.begin(); it != bulletBuffer.end();) {
    bool bulletCollided = false;

        for (auto enemyIt = activeEnemyList.begin(); enemyIt != activeEnemyList.end(); ++enemyIt) {

        // Check collision between bullet and enemy
        if (isCollision(*it, *enemyIt)) {
            // Handle collision (e.g., remove enemy, bullet)
            it = bulletBuffer.erase(it); // Remove bullet
            enemyIt=activeEnemyList.erase(enemyIt);
                        score++;

            bulletCollided = true;
            break; // Break out of the inner loop
        }
    }

    if (!bulletCollided) {
        // If no collision occurred, move to the next bullet
        ++it;
    }
}


    // collision detection

   window.clear();

    for (Enemy &e : activeEnemyList) {
      e.move();

      e.render(window);
    }

    for (Bullet &b : bulletBuffer) {
      b.move();
      b.render(window);
    }

    player.render(window);
 
    
       std::stringstream ss;
        ss << "Score: " << score;
        scoreText.setString(ss.str());


        window.draw(scoreText);
    window.display();
    
  
  return 0;
}

int gameui(sf::RenderWindow& window,bool& isSoundOn,sf::Sound& sound
) {
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

    // Main game loop
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
                    gameState=GameState::RUNNING;      
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
        return 0;
};

int displayGameOverScreen(sf::RenderWindow& window,sf::SoundBuffer& gameover,sf::Sound& over) {
  
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
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        // Add code to restart the game
                        gameState=GameState::RUNNING;

                        std::cout << "Restarting game..." << std::endl;
                    }
                    if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        window.close();
                    }
                }
            }
            // Change button color on hover
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                restartButton.setFillColor(sf::Color(72, 140, 11 ));
            else
                restartButton.setFillColor(sf::Color::Green);

            if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                exitButton.setFillColor(sf::Color(72, 140, 11 ));
            else
                exitButton.setFillColor(sf::Color(144, 12, 63));
        }

        window.clear(sf::Color::Black);
        window.draw(gameOverSprite);
        window.draw(restartButton);
        window.draw(restartText);
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    

    return 0;
}
int main() {

 Player player;
     bool isSoundOn = true;

  vector<Bullet> bulletBuffer;

  int levelEmemyCount = 10;
   vector<Enemy> activeEnemyList;
    // Font for the text
    sf::Font font;
      int score = 0;
     if (!font.loadFromFile("arial.ttf")) {
        // handle error if the font file is not found
        return EXIT_FAILURE;
    }
      // Create the SFML window
    sf::SoundBuffer gameover;
    if (!gameover.loadFromFile("PNG/oversound.wav")) {
        std::cerr << "Failed to load sound file." << std::endl;
        return 1;
    }

    // Create SFML sound and set its buffer
    sf::Sound over;
      // Load sound
    SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("PNG/sound_effect.wav")) {
        std::cerr<< "Failed to load sound effect" << std::endl;
        return 1;
    }

    // Create sound object
    sf::Sound sound;
    sound.setBuffer(soundBuffer);
    sound.setLoop(true);
    sound.play();




    Event event;
  RenderWindow window(VideoMode(ScreenWidth, ScreenHeight), "Game Window");

    while (window.isOpen()) {

     window.clear();

  if(gameState==GameState::RUNNING){ 
     gameRunning(window,player,bulletBuffer,levelEmemyCount,activeEnemyList,font,score);
  }else if(gameState==GameState::STARTING){
    gameui(window,isSoundOn,sound);

  }else if(gameState==GameState::GAME_OVER){
    displayGameOverScreen(window,gameover,over);
    bulletBuffer.clear();
    activeEnemyList.clear();
    score=0;
    player.resetCoordinate();


  }
  
    }

 
  


 
  return EXIT_SUCCESS;
  return 0;
}