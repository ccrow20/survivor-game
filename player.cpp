#include "player.hpp"

player::player() {
    block.setSize(sf::Vector2f(30, 60));
    block.setFillColor(sf::Color::Green);
    block.setPosition(config::WINDOW_WIDTH / 2, config::WINDOW_HEIGHT / 2);
    lastKeyPress = RIGHT;
    Speed = 400;
    Level = 2;
}

void player::update(float deltaTime, const map& level) {
    sf::Vector2f movement = readInput();

    moveAxis(sf::Vector2f(movement.x * deltaTime, 0.f), level);
    moveAxis(sf::Vector2f(0.f, movement.y * deltaTime), level);
}

sf::Vector2f player::readInput() {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movement.y -= Speed;
        lastKeyPress = UP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement.x -= Speed;
        lastKeyPress = LEFT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movement.y += Speed;
        lastKeyPress = DOWN;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement.x += Speed;
        lastKeyPress = RIGHT;
    }

    if (movement.x != 0.f && movement.y != 0.f) {
        movement *= 1.f / std::sqrt(2.f);
    }
    return movement;
}

void player::moveAxis(sf::Vector2f delta, const map& level) {
    sf::Vector2f next = block.getPosition() + delta;
    if (!level.collides(next, block.getSize())) {
        block.setPosition(next);
    }
}

Direction player::getDirection() const {
    return lastKeyPress;
}

int player::getLevel() const {
    return Level;
}

sf::Vector2f player::getPosition() const {
    return block.getPosition();
}

sf::FloatRect player::getBounds() const {
    return block.getGlobalBounds();
}

void player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(block, states);
}
