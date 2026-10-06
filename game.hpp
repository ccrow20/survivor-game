#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "config.hpp"
#include "player.hpp"
#include "map.hpp"
#include "attack.hpp"
#include "SpawnManager.hpp"
#include "SpatialHash.hpp"

class game {
public:
  game();
  void run();

private:
  void processEvents();
  void update(float dt);
  void render();

  void rebuildSpatialHash();
  void collision(float dt);
  void checkPlayerEnemyCollisions();
  void checkAttackEnemyCollisions();
  void separateEnemies(float dt);
  void onPlayerHit();

  void loadLevel(const std::string& mapFile);

  sf::RenderWindow window;
  sf::View camera;
  player Player;
  std::vector<attack> playerAttacks;
  SpawnManager spawnManager;
  map Map;
  sf::Clock clock;
  SpatialHash Grid;
};
