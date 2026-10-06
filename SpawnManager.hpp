#pragma once

#include <cstddef>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

#include "enemy.hpp"

// Owns every enemy in the game as a fixed-size object pool.
//
// The pool is allocated once and never resized, so an enemy's index (used as
// its EntityID in the spatial hash) and its address stay valid for the whole
// run. An inactive enemy is a free slot: dead enemies are recycled through
// acquire() rather than destroyed and reallocated.
//
// Enemies come from two sources:
//   * the ambient population, which is topped up to `ambientTarget` on a ring
//     around the player whenever enemies die, and
//   * spawners, fixed world locations that emit one enemy per interval.
// The ambient top-up runs first each update, so keep the pool larger than the
// ambient target or spawners will have no free slots to draw from.
class SpawnManager : public sf::Drawable {
public:
  SpawnManager(std::size_t poolSize, int ambientTarget);

  // Updates active enemies, reclaims the ones that died, then spawns.
  void update(float dt, sf::Vector2f playerPos);

  // --- spawners ---
  // Turns `location` into a spawner that releases an enemy every `interval`
  // seconds, scattered up to `radius` away. Returns an id for removeSpawner().
  int addSpawner(sf::Vector2f location, float interval = 2.f, float radius = 32.f);
  bool removeSpawner(int id);
  void clearSpawners();
  std::size_t spawnerCount() const;

  // --- pool ---
  // Activates a free enemy at `position`; nullptr if the pool is exhausted.
  Enemy* acquire(sf::Vector2f position);
  // Returns an active enemy to the pool (e.g. to cull it); no-op if already free.
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
  std::vector<int> freeSlots;  // indices of inactive enemies, acquired from the back
  int active;
  int ambientTarget;

  std::vector<Spawner> spawners;
  int nextSpawnerId;

  std::mt19937 rng;
};
