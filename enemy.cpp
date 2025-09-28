#include "enemy.hpp"

Enemy::Enemy(player &p) : Player(p) {
    static std::mt19937 rng(std::random_device{}()); 
    std::uniform_int_distribution<int> dist(-200, 200);

    int xOffset = dist(rng);
    int yOffset = dist(rng);

    active = true;
    health = 100;
    enemyHitBox.setFillColor(sf::Color::Red);
    enemyHitBox.setSize(sf::Vector2f(30, 60));
    enemyHitBox.setPosition((Player.getPosition().x) + xOffset + 300, (Player.getPosition().y) + yOffset + 300);
}

void Enemy::update(float deltatime) {
    previousPos = enemyHitBox.getPosition();
    if (health <= 0) {
        active = false;
        return;
    }
    sf::Vector2f direction = Player.getPosition() - enemyHitBox.getPosition();
    
    direction = normalize(direction);
    float speed = 100.f;
    enemyHitBox.move(direction * speed * deltatime);
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

void Enemy::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(enemyHitBox);
    }
}

bool Enemy::getActiveState() {
    return active;
}

sf::Vector2f Enemy::getVelocity(float deltatime) {
    return (currentPos - previousPos) / deltatime;
}

void Enemy::setActiveState(bool newState) {
    active = newState;
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

sf::FloatRect Enemy::getBounds() {
    return enemyHitBox.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() {
    return enemyHitBox.getPosition();
}