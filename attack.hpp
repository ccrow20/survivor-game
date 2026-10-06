#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>

#include "SpatialHash.hpp"

class attack : public sf::Drawable {
public:
  attack();
  void update(float deltatime, sf::Vector2f position);
  bool getActiveState() const;
  int getDamage() const;
  // Records `id` as hit this swing; false if it was already hit.
  bool registerHit(const EntityID& id);
  sf::Vector2f getPosition() const;
  sf::FloatRect getBounds() const;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  sf::RectangleShape firstAttack;
  std::unordered_set<EntityID> hitID;
  sf::Time timeSinceLastAttack, attackCD, activeDuration, activeTimeElapsed;
  bool active;
  int damage;
};
