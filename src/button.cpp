#include "button.hpp"

namespace {
const sf::Color NORMAL_FILL(30, 30, 30);
const sf::Color HOVER_FILL(70, 70, 70);
}

Button::Button(const std::string& text, const sf::Font& font, sf::Vector2f size,
               std::function<void()> cb)
    : onClick(std::move(cb)) {
    box.setSize(size);
    box.setOrigin(size / 2.f);
    box.setFillColor(NORMAL_FILL);
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::White);

    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(28);
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2.f,
                    bounds.top + bounds.height / 2.f);
}

void Button::setPosition(sf::Vector2f position) {
    box.setPosition(position);
    label.setPosition(position);
}

bool Button::contains(sf::Vector2f point) const {
    return box.getGlobalBounds().contains(point);
}

void Button::setHovered(bool hovered) {
    box.setFillColor(hovered ? HOVER_FILL : NORMAL_FILL);
}

void Button::click() {
    if (onClick) onClick();
}

void Button::draw(sf::RenderTarget& target) const {
    target.draw(box);
    target.draw(label);
}
