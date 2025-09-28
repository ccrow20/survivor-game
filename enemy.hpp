#pragma once

#include <cmath>
#include <random>
#include <iostream>
#include "player.hpp"

class Enemy {
public:
  Enemy(player& p);
  void update(float deltatime);
  sf::Vector2f normalize(const sf::Vector2f& v);
  void takeDmg(int damage, int id);
  void draw(sf::RenderWindow& window);
  void setActiveState(bool newState);
  void move(float x, float y);
  void Slide(Enemy& other);
  bool getActiveState();
  sf::Vector2f getVelocity(float deltatime);
  sf::Vector2f getPosition();
  sf::FloatRect getBounds();

private:
  player& Player;
  sf::RectangleShape enemyHitBox;
  sf::Vector2f previousPos, currentPos;
  int health;
  bool active;
};