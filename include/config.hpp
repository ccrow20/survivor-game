#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

namespace config {

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 500;

constexpr int TILE_SIZE = 64;
const std::string GROUND_IMAGE = "assets/textures/ground_01.png";
const std::string WALL_IMAGE = "assets/textures/wall.png";
const std::string FONT_PATH = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf";
const std::string LAVA_IMAGE = "assets/textures/lava.png";
constexpr float LAVA_DAMAGE_PER_SECOND = 25.f;
constexpr float PLAYER_MAX_HEALTH = 100.f;
constexpr float ENEMY_CONTACT_DAMAGE = 10.f;
constexpr float ENEMY_CONTACT_INTERVAL = 1.f;
constexpr float HEALTHBAR_HEIGHT = 5.f;
constexpr float HEALTHBAR_OFFSET = 4.f;
constexpr float SPATIAL_CELL_SIZE = 64.f;

constexpr int AMBIENT_ENEMY_COUNT = 200;
constexpr int ENEMY_POOL_SIZE = 300;

const sf::Vector2f ATTACK_OFFSET(40.f, 5.f);

}
