#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>

const int TILE_SIZE = 64;

class map : public sf::Drawable, public sf::Transformable {
public:
  map();
  bool load(const std::string& tilesetPath);
  std::vector<std::vector<char>> getMap();
  int getHeight();
  int getWidth();

  friend std::istream& operator>>(std::istream& in, map& s);

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  int mapHeight;
  int mapWidth;
  sf::Texture tGround, tWall;
  //mutable sf::Sprite sGround, sWall;
  std::vector<std::vector<char>> mapTiles;
  sf::VertexArray vertArray;
};