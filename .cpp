#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace sf;

using namespace std;

const float PLAYER_WIDTH = 50.f;
const float PLAYER_HEIGHT = 50.f;
const float PlayerStep = 0.05;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

const float BULLET_SIZE = 5.f;

const float BULLET_STEP = 0.05;

struct Coord {
  float x = 0.f;
  float y = 0.f;
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

    if (oldX + dx > ScreenWidth - 20 || oldX + dx <= 20) {
      return;
    }

    if (oldY + dy > ScreenHeight - 20 || oldY + dy <= 20) {
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

    return Vector2f{pos.x + PLAYER_WIDTH / 2, pos.y + PLAYER_HEIGHT / 2};
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
  return Coord{coord.x + PLAYER_WIDTH / 2 + 200, coord.y + PLAYER_HEIGHT / 2};
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

int main() {

  RenderWindow window(VideoMode(ScreenWidth, ScreenHeight), "Game Window");
  Player player;

  vector<Bullet> bulletBuffer;

  int bulletCount = 0;

  while (window.isOpen()) {

    cout << bulletCount << endl;

    // Process events
    Event event;

    while (window.pollEvent(event)) {
      // Close window: exit
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

    window.clear();

    for (Bullet b : bulletBuffer) {
      b.render(window);
      b.incrementPosition();
    }

    player.render(window);
    window.display();
  }

  return EXIT_SUCCESS;
  return 0;
}
