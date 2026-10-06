#include "enemy.hpp"

namespace {

constexpr int MAX_HEALTH = 100;
constexpr float SPEED = 100.f;

}

Enemy::Enemy() : health(0), active(false) {
    enemyHitBox.setFillColor(sf::Color::Red);
    enemyHitBox.setSize(sf::Vector2f(30, 60));
}

void Enemy::update(float deltatime, sf::Vector2f target) {
    if (!active)
        return;

    previousPos = enemyHitBox.getPosition();
    if (health <= 0) {
        active = false;
        return;
    }
    sf::Vector2f direction = normalize(target - enemyHitBox.getPosition());
    enemyHitBox.move(direction * SPEED * deltatime);
    currentPos = enemyHitBox.getPosition();
}

sf::Vector2f Enemy::normalize(const sf::Vector2f& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length != 0)
        return sf::Vector2f(v.x / length, v.y / length);
    else
        return sf::Vector2f(0.f, 0.f);
}

void Enemy::takeDmg(int damage, int id) {
    health -= damage;
    std::cout << "Enemy " << id << " took " << damage << " damage. Health now: " << health << std::endl;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (active) {
        target.draw(enemyHitBox, states);
    }
}

bool Enemy::getActiveState() const {
    return active;
}

sf::Vector2f Enemy::getVelocity(float deltatime) const {
    return (currentPos - previousPos) / deltatime;
}

void Enemy::move(float x, float y) {
    enemyHitBox.move(x, y);
}

void Enemy::Slide(Enemy& other) {
    sf::FloatRect intersection;
    if (enemyHitBox.getGlobalBounds().intersects(other.getBounds(), intersection)) {
        float dx = intersection.width / 2.0;
        float dy = intersection.height / 2.0;

        if (dx < dy) {
            if (enemyHitBox.getPosition().x < other.getPosition().x) {
                enemyHitBox.move(-dx, 0);
                other.move(dx, 0);
            }
            else {
                enemyHitBox.move(dx, 0);
                other.move(-dx, 0);
            }
        }
        if (dy < dx) {
            if (enemyHitBox.getPosition().y < other.getPosition().y) {
                enemyHitBox.move(0, -dy);
                other.move(0, dy);
            }
            else {
                enemyHitBox.move(0, dy);
                other.move(0, -dy);
            }
        }
    }
}

void Enemy::activate(sf::Vector2f position) {
    health = MAX_HEALTH;
    active = true;
    enemyHitBox.setPosition(position);
    previousPos = currentPos = position;
}

void Enemy::deactivate() {
    health = 0;
    active = false;
}

sf::FloatRect Enemy::getBounds() const {
    return enemyHitBox.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const {
    return enemyHitBox.getPosition();
}
