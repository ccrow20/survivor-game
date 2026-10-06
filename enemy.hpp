#pragma once

#include <cmath>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

class Enemy : public sf::Drawable {
public:
  // Starts out inactive; SpawnManager brings it into play with activate().
  Enemy();
  // Walks toward `target`; does nothing while inactive.
  void update(float deltatime, sf::Vector2f target);
  void takeDmg(int damage, int id);
  void move(float x, float y);
  void Slide(Enemy& other);
  // Revives the enemy at full health at `position`.
  void activate(sf::Vector2f position);
  // Takes the enemy out of play without killing it through damage.
  void deactivate();
  bool getActiveState() const;
  sf::Vector2f getVelocity(float deltatime) const;
  sf::Vector2f getPosition() const;
  sf::FloatRect getBounds() const;

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  static sf::Vector2f normalize(const sf::Vector2f& v);

  sf::RectangleShape enemyHitBox;
  sf::Vector2f previousPos, currentPos;
  int health;
  bool active;
};
