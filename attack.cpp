#include "attack.hpp"

attack::attack() {
    damage = 100;
    active = false;
    activeDuration = sf::seconds(0.2);
    activeTimeElapsed = sf::Time::Zero;
    attackCD = sf::seconds(0.75);
    timeSinceLastAttack = sf::Time::Zero;
    firstAttack.setFillColor(sf::Color::White);
    firstAttack.setSize(sf::Vector2f(200, 25));
    firstAttack.setOrigin(20, 20);
}

void attack::update(float deltatime, sf::Vector2f position) {
    sf::Time dt = sf::seconds(deltatime);

    if (!active) {
        timeSinceLastAttack += dt;
        if (timeSinceLastAttack >= attackCD) {
            active = true;
            timeSinceLastAttack = sf::Time::Zero;
        }
    }
    else {
        activeTimeElapsed += dt;
        if (activeTimeElapsed >= activeDuration) {
            active = false;
            activeTimeElapsed = sf::Time::Zero;
            hitID.clear();
        }
    }
    firstAttack.setPosition(position);
}

bool attack::getActiveState() const {
    return active;
}

bool attack::registerHit(const EntityID& id) {
    return hitID.insert(id).second;
}

int attack::getDamage() const {
    return damage;
}

sf::Vector2f attack::getPosition() const {
    return firstAttack.getPosition();
}

sf::FloatRect attack::getBounds() const {
    return firstAttack.getGlobalBounds();
}

void attack::draw(sf::RenderTarget& target, sf::RenderStates states) const  {
    if (active) {
        target.draw(firstAttack, states);
    }
}
