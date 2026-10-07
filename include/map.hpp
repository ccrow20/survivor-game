#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>

#include "config.hpp"

class map : public sf::Drawable, public sf::Transformable {
public:
  struct TileInfo {
    int tilesetIndex;
    bool solid;
    std::string image;
    float damagePerSecond = 0.f;
  };

  map();
  void setTileType(char symbol, TileInfo info);
  void setDefaultTile(TileInfo info);
  bool load();
  bool loadFromFile(const std::string& filename);

  const std::vector<std::vector<char>>& getMap() const;
  int getHeight() const;
  int getWidth() const;

  bool isWall(int tileX, int tileY) const;
  bool collides(sf::Vector2f pos, sf::Vector2f size) const;

  float damageAt(sf::Vector2f pos, sf::Vector2f size) const;

  friend std::istream& operator>>(std::istream& in, map& s);

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  static int toTile(float pos);
  const TileInfo& tileInfo(char symbol) const;

  int mapHeight;
  int mapWidth;
  std::vector<std::vector<char>> mapTiles;
  std::unordered_map<char, TileInfo> tileTypes;
  TileInfo defaultTile;

  struct Layer {
    sf::Texture texture;
    sf::VertexArray vertices{sf::Quads};
  };
  std::map<std::string, Layer> layers;
  std::string baseImage;
};
