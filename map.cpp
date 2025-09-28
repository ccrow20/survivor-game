#include "map.hpp"
#include <iostream>


map::map () {
    mapWidth = 0;
    mapHeight = 0;

    if (!tGround.loadFromFile("ground_01.png")) {
        std::cerr << "Failed to load texture" << std::endl;
    }
}

bool map::load(const std::string& tilesetPath) {
    if (!tGround.loadFromFile(tilesetPath)) {
        return false;
    }
    vertArray.setPrimitiveType(sf::Quads);
    vertArray.resize(mapWidth * mapHeight * 4);

    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            char tileChar = mapTiles[j][i];

            int tileNumber;
            if (tileChar == '#') {
                tileNumber = 1;
            } 
            else {
                tileNumber = 0;
            }

            int tilesPerRow = tGround.getSize().x / TILE_SIZE;
            int tu = tileNumber % tilesPerRow;
            int tv = tileNumber / tilesPerRow;

            sf::Vertex* quad = &vertArray[(i + j * mapWidth) * 4];

            quad[0].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
            quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE, j * TILE_SIZE);
            quad[2].position = sf::Vector2f((i + 1) * TILE_SIZE, (j + 1) * TILE_SIZE);
            quad[3].position = sf::Vector2f(i * TILE_SIZE, (j + 1) * TILE_SIZE);

            quad[0].texCoords = sf::Vector2f(tu * TILE_SIZE, tv * TILE_SIZE);
            quad[1].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, tv * TILE_SIZE);
            quad[2].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, (tv + 1) * TILE_SIZE);
            quad[3].texCoords = sf::Vector2f(tu * TILE_SIZE, (tv + 1) * TILE_SIZE);
        }
    }

    return true;
}

int map::getHeight() {
    return mapHeight;
}

int map::getWidth() {
    return mapWidth;
}

std::vector<std::vector<char>> map::getMap() {
    return mapTiles;
}

std::istream& operator>>(std::istream& in, map& s) {
    std::string temp;
    in >> s.mapHeight >> s.mapWidth;
    s.mapTiles.resize(s.mapHeight, std::vector<char>(s.mapWidth));

    for (int i = 0; i < s.mapHeight; i++) {
        in >> temp;
        for (int j = 0; j < s.mapWidth; j++) {
            s.mapTiles[i][j] = temp[j];
        }
    }
    return in;
}

void map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &tGround;
    target.draw(vertArray, states);
}