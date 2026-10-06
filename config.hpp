#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

namespace config {

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 500;

constexpr int TILE_SIZE = 64;
const std::string GROUND_IMAGE = "textures/ground_01.png";
const std::string WALL_IMAGE = "textures/wall.png";
constexpr float SPATIAL_CELL_SIZE = 64.f;

// Enemies kept alive around the player, drawn from a larger pool so that
// spawners always have free slots to use.
constexpr int AMBIENT_ENEMY_COUNT = 100;
constexpr int ENEMY_POOL_SIZE = 300;

// Attack is positioned relative to the player's top-left corner.
const sf::Vector2f ATTACK_OFFSET(40.f, 5.f);

}
