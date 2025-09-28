#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_set>

#include "enemy.hpp"
#include "SpatialHash.hpp"

class attack : public sf::Drawable {
public:
  attack(player &p);
  void update(float deltatime);
  bool getActiveState();
  void checkCollision(EntityID& ID);
  int getDamage();
  std::unordered_set<EntityID>& getSet();
  void setInsert(EntityID& ID);
  sf::Vector2f getPosition();
  sf::FloatRect getBounds();
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  player &Player;
  sf::CircleShape firstAttack;
  std::unordered_set<EntityID> hitID;
  sf::Time timeSinceLastAttack, attackCD, activeDuration, activeTimeElapsed;
  bool active;
  int damage;
};