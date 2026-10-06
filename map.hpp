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
  // How one map character looks and behaves.
  struct TileInfo {
    int tilesetIndex;   // tile number within the image, row by row (0 for a single-tile image)
    bool solid;         // blocks movement
    std::string image;  // image file to draw from; empty = the default tile's image
  };

  // Starts with a solid wall ('#') and a floor default; see map.cpp.
  map();
  // Defines (or redefines) the tile a map character stands for. Call before
  // load() so the drawn tiles pick it up.
  void setTileType(char symbol, TileInfo info);
  // The tile used for characters with no entry (the floor).
  void setDefaultTile(TileInfo info);
  // Builds the drawable map from the tile table; returns false if an image
  // fails to load or a tile index lies outside its image.
  bool load();
  // Reads a map description from `filename`; returns false if it can't be opened.
  bool loadFromFile(const std::string& filename);

  const std::vector<std::vector<char>>& getMap() const;
  int getHeight() const;
  int getWidth() const;

  // Tiles outside the map count as walls.
  bool isWall(int tileX, int tileY) const;
  // True if a box with top-left `pos` and the given size touches any wall tile.
  bool collides(sf::Vector2f pos, sf::Vector2f size) const;

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

  // One draw call per texture, so tiles are grouped by the image they use.
  struct Layer {
    sf::Texture texture;
    sf::VertexArray vertices{sf::Quads};
  };
  std::map<std::string, Layer> layers;  // keyed by image path
  std::string baseImage;                // the floor's image; its layer is drawn first
};
