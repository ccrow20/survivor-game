#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

#include "map.hpp"

enum Direction { UP, DOWN, LEFT, RIGHT };

class player : public sf::Drawable {
public:
  player();
  void update(float deltaTime, const map& level);
  Direction getDirection() const;
  int getLevel() const;
  sf::Vector2f getPosition() const;
  sf::FloatRect getBounds() const;
  float getHealth() const;
  void takeDamage(float amount);
  void reset();

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  sf::Vector2f readInput();

  void moveAxis(sf::Vector2f delta, const map& level);

  Direction lastKeyPress;
  sf::RectangleShape block;
  int Level;
  float Health;
  float Speed;
};
