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
#include "button.hpp"

enum class GAME_STATE {Title, Gameplay, Pause, Gameover};

class game {
public:
  game();
  void run();

private:
  void processEvents();
  void update(float dt);
  void render();
  void renderTitle();
  void resetGame();
  void buildTitleScreen();
  void layoutTitleScreen();

  void rebuildSpatialHash();
  void collision(float dt);
  void checkPlayerEnemyCollisions(float dt);
  void checkAttackEnemyCollisions();
  void separateEnemies(float dt);
  void onPlayerDeath();

  void updateTimerText();

  void loadLevel(const std::string& mapFile);

  sf::RenderWindow window;
  sf::View camera;
  player Player;
  std::vector<attack> player_attacks;
  SpawnManager spawnManager;
  map Map;
  sf::Clock clock;
  SpatialHash Grid;
  GAME_STATE State;

  sf::View hudView;
  sf::Font font;
  sf::Text timerText;
  float elapsedTime = 0.f;

  sf::Text titleText;
  std::vector<Button> title_buttons;
};
