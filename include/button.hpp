#pragma once

#include <functional>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>

class Button {
public:
  Button(const std::string& text, const sf::Font& font, sf::Vector2f size,
         std::function<void()> cb);

  void setPosition(sf::Vector2f position);
  bool contains(sf::Vector2f point) const;
  void setHovered(bool hovered);
  void click();
  void draw(sf::RenderTarget& target) const;

private:
  sf::RectangleShape box;
  sf::Text label;
  std::function<void()> onClick;
};
