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

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  sf::Vector2f readInput();
  // Moves one axis at a time so the player can slide along walls.
  void moveAxis(sf::Vector2f delta, const map& level);

  Direction lastKeyPress;
  sf::RectangleShape block;
  int Level;
  int Health;
  float Speed;
};
