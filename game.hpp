#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "player.hpp"
#include "map.hpp"
#include "attack.hpp"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;

class game {
public:
  game();
  void run();
  void changeMap(std::string filename);

private:
  void processEvents();
  void update();
  void render();
  void collision(float dt);

  sf::RenderWindow window;
  sf::View camera;
  player Player;
  attack playerAttack;
  std::vector<Enemy> Goons;
  map Map;
  sf::Clock clock;
  SpatialHash Grid;
};