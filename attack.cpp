#include "attack.hpp"

attack::attack(player &p) : Player(p) {
    damage = 100;
    active = false;
    activeDuration = sf::seconds(1.f);
    activeTimeElapsed = sf::Time::Zero;
    attackCD = sf::seconds(0.3);
    timeSinceLastAttack = sf::Time::Zero;
    firstAttack.setFillColor(sf::Color::White);
    firstAttack.setRadius(20);
    firstAttack.setOrigin(20, 20);
    firstAttack.setPosition(Player.getPosition());
}

void attack::update(float deltatime) {
    sf::Vector2f offSet;
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

    if (Player.getDirection() == UP) {
        offSet = sf::Vector2f(15.f, -30.f);
    }
    else if (Player.getDirection() == DOWN) {
        offSet = sf::Vector2f(15.f, 90.f);
    }
    else if (Player.getDirection() == LEFT) {
        offSet = sf::Vector2f(-30.f, 30.f);
    }
    else if (Player.getDirection() == RIGHT) {
        offSet = sf::Vector2f(60.f, 30.f);
    }
    firstAttack.setPosition(Player.getPosition() + offSet);
}

bool attack::getActiveState() {
    return active;
}

void attack::checkCollision(EntityID& ID) {
    hitID.insert(ID);
}

std::unordered_set<EntityID>& attack::getSet() {
    return hitID;
}

void attack::setInsert(EntityID& id) {
    hitID.insert(id);
}

int attack::getDamage() {
    return damage;
}

sf::Vector2f attack::getPosition() {
    return firstAttack.getPosition();
}

sf::FloatRect attack::getBounds() {
    return firstAttack.getGlobalBounds();
}

void attack::draw(sf::RenderTarget& target, sf::RenderStates states) const  {
    if (active) {
        target.draw(firstAttack, states);
    }
}