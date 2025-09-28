#include "player.hpp"

player::player() {
    block.setSize(sf::Vector2f(30, 60));
    block.setFillColor(sf::Color::Green);
    block.setPosition(1000/2, 500/2);
    lastKeyPress = RIGHT;
    Speed = 200;
}

void player::update(float deltaTime, std::vector<std::vector<char>> mapFile) {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movement.y -= 1.f * Speed;
        lastKeyPress = UP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement.x -= 1.f * Speed;
        lastKeyPress = LEFT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movement.y += 1.f * Speed;
        lastKeyPress = DOWN;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement.x += 1.f * Speed;
        lastKeyPress = RIGHT;
    }

    if (movement.x != 0.f && movement.y != 0.f) {
        movement *= 1.f / std::sqrt(2.f);
    }

    sf::Vector2f currentPosition = block.getPosition();

    sf::Vector2f nextPositionX = currentPosition + sf::Vector2f(movement.x * deltaTime, 0.f);
    if (!checkCollision(nextPositionX, mapFile)) {
        block.setPosition(nextPositionX);
    }

    sf::Vector2f nextPositionY = block.getPosition() + sf::Vector2f(0.f, movement.y * deltaTime);
    if (!checkCollision(nextPositionY, mapFile)) {
        block.setPosition(nextPositionY);
    }
}

Direction player::getDirection() {
    return lastKeyPress;
}

sf::Vector2f player::getPosition() {
    return block.getPosition();
}

sf::FloatRect player::getBounds() {
    return block.getGlobalBounds();
}

int player::toTile(float pos) {
    return static_cast<int>(pos) / 64;
}

bool player::isWall(int x, int y, std::vector<std::vector<char>> mapFile) {
    return mapFile[y][x] == '#';
}

bool player::checkCollision(sf::Vector2f nextPos, std::vector<std::vector<char>> mapFile) {
    int left = toTile(nextPos.x);
    int right = toTile(nextPos.x + block.getGlobalBounds().width);
    int top = toTile(nextPos.y);
    int bottom = toTile(nextPos.y + block.getGlobalBounds().height);

    for (int y = top; y <= bottom; ++y) {
        for (int x = left; x <= right; ++x) {
            if (isWall(x, y, mapFile)) {
                return true;
            }
        }
    }
    return false;
}

void player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(block, states);
}