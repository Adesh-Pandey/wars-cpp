#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace sf;

using namespace std;

const float PLAYER_WIDTH = 50.f;
const float PLAYER_HEIGHT = 50.f;

const float ENEMY_WIDTH = 50.f;
const float ENEMY_HEIGHT = 50.f;

const float PlayerStep = 0.1f;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

const float BULLET_SIZE = 3.f;
const float ENEMY_STEP = 0.02f;
const float BULLET_STEP = 0.05;

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

  void move(float dx, float dy) override {
    // Implement the movement logic for the Player
    playerSprite.move(dx, dy);
  }

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
    playerSprite.setPosition(oldX + dx, oldY + dy);
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

  void incrementPosition() {
    const Vector2f oldPos = bulletSprite.getPosition();

    bulletSprite.setPosition(oldPos.x, oldPos.y - BULLET_STEP);
  }
  void render(RenderWindow &window) {
    // Draw the player's sprite to the screen
    window.draw(bulletSprite);
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

  void incrementPosition() {
    const Vector2f oldPos = enemySprite.getPosition();

    enemySprite.setPosition(oldPos.x, oldPos.y + ENEMY_STEP);
  }
  void render(RenderWindow &window) {
    // Draw the player's sprite to the screen
    window.draw(enemySprite);
  }
};

Coord randomCoordinateAboveScreenForEnemySpwan() {

  return Coord(rand() % (800 - (int)ENEMY_HEIGHT), -1 * rand() % 1000);
}

int main() {

  RenderWindow window(VideoMode(ScreenWidth, ScreenHeight), "Game Window");
  Player player;

  vector<Bullet> bulletBuffer;

  int levelEmemyCount = 10;

  vector<Enemy> activeEnemyList;

  while (window.isOpen()) {

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
    }

    if (event.type == Event::KeyPressed) {
      whichKeyPressedCheck(&player, event.key.code);
    }

    const int diff = levelEmemyCount - activeEnemyList.size();

    if (diff > 0) {
      for (int i = 0; i < diff; i++) {
        activeEnemyList.push_back(
            Enemy(randomCoordinateAboveScreenForEnemySpwan()));
      }
    }

    // collision detection

    // clean up bullets after they move outside screen
    // remove enemies after they go below screen

    window.clear();

    for (Enemy &e : activeEnemyList) {
      e.incrementPosition();

      e.render(window);
    }

    for (Bullet &b : bulletBuffer) {
      b.incrementPosition();

      b.render(window);
    }

    player.render(window);
    window.display();
  }
  return EXIT_SUCCESS;
  return 0;
}
