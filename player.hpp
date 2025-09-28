#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

enum Direction { UP, DOWN, LEFT, RIGHT };

class player : public sf::Drawable {
public:
  player();
  void update(float deltaTime, std::vector<std::vector<char>> mapFile);
  Direction getDirection();
  sf::Vector2f getPosition();
  sf::FloatRect getBounds();
  int toTile(float pos);
  bool isWall(int x, int y, std::vector<std::vector<char>> mapFile);
  bool checkCollision(sf::Vector2f nexPos, std::vector<std::vector<char>> mapFile);

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  //sf::Texture tPlayer;
  //sf::Sprite sPlayer;

  Direction lastKeyPress;
  sf::RectangleShape block;
  int Health;
  float Speed;
};