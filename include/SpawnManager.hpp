#pragma once

#include <cstddef>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

#include "enemy.hpp"

class SpawnManager : public sf::Drawable {
public:
  SpawnManager(std::size_t poolSize, int ambientTarget);

  void update(float dt, sf::Vector2f playerPos);

  int addSpawner(sf::Vector2f location, float interval = 2.f, float radius = 32.f);
  bool removeSpawner(int id);
  void clearSpawners();
  std::size_t spawnerCount() const;

  Enemy* acquire(sf::Vector2f position);

  void despawn(int index);
  void despawnAll();

  void setAmbientTarget(int target);
  int getAmbientTarget() const;

  std::size_t capacity() const;
  int activeCount() const;
  int freeCount() const;

  Enemy& operator[](int index);
  const Enemy& operator[](int index) const;
  std::vector<Enemy>::iterator begin();
  std::vector<Enemy>::iterator end();
  std::vector<Enemy>::const_iterator begin() const;
  std::vector<Enemy>::const_iterator end() const;

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  struct Spawner {
    int id;
    sf::Vector2f position;
    float interval;
    float radius;
    float timer;
  };

  void reclaimDead(const std::vector<bool>& wasActive);
  void topUpAmbient(sf::Vector2f playerPos);
  void updateSpawners(float dt);

  sf::Vector2f ringPosition(sf::Vector2f center);
  sf::Vector2f scatter(sf::Vector2f center, float radius);

  std::vector<Enemy> pool;
  std::vector<int> freeSlots;
  int active;
  int ambientTarget;

  std::vector<Spawner> spawners;
  int nextSpawnerId;

  std::mt19937 rng;
};
