#include "SpawnManager.hpp"

#include <algorithm>

namespace {

constexpr int RING_JITTER = 200;
constexpr int RING_RADIUS = 600;
constexpr float MIN_INTERVAL = 0.01f;

}

SpawnManager::SpawnManager(std::size_t poolSize, int ambientTarget)
    : pool(poolSize), active(0), ambientTarget(ambientTarget), nextSpawnerId(0),
      rng(std::random_device{}()) {
    freeSlots.reserve(poolSize);
    // Reversed so the lowest index is handed out first.
    for (int i = static_cast<int>(poolSize) - 1; i >= 0; i--) {
        freeSlots.push_back(i);
    }
}

void SpawnManager::update(float dt, sf::Vector2f playerPos) {
    std::vector<bool> wasActive(pool.size());
    for (std::size_t i = 0; i < pool.size(); i++) {
        wasActive[i] = pool[i].getActiveState();
        pool[i].update(dt, playerPos);
    }

    reclaimDead(wasActive);
    topUpAmbient(playerPos);
    updateSpawners(dt);
}

// Enemies deactivate themselves when their health runs out; hand those slots
// back to the pool.
void SpawnManager::reclaimDead(const std::vector<bool>& wasActive) {
    for (std::size_t i = 0; i < pool.size(); i++) {
        if (wasActive[i] && !pool[i].getActiveState()) {
            freeSlots.push_back(static_cast<int>(i));
            active--;
        }
    }
}

void SpawnManager::topUpAmbient(sf::Vector2f playerPos) {
    while (active < ambientTarget) {
        if (!acquire(ringPosition(playerPos)))
            break;
    }
}

void SpawnManager::updateSpawners(float dt) {
    for (Spawner& spawner : spawners) {
        spawner.timer += dt;
        while (spawner.timer >= spawner.interval) {
            spawner.timer -= spawner.interval;
            if (!acquire(scatter(spawner.position, spawner.radius))) {
                // Pool is full: drop the backlog instead of bursting later.
                spawner.timer = 0.f;
                break;
            }
        }
    }
}

int SpawnManager::addSpawner(sf::Vector2f location, float interval, float radius) {
    int id = nextSpawnerId++;
    spawners.push_back({id, location, std::max(interval, MIN_INTERVAL), std::max(radius, 0.f), 0.f});
    return id;
}

bool SpawnManager::removeSpawner(int id) {
    auto it = std::find_if(spawners.begin(), spawners.end(),
                           [id](const Spawner& s) { return s.id == id; });
    if (it == spawners.end())
        return false;
    spawners.erase(it);
    return true;
}

void SpawnManager::clearSpawners() {
    spawners.clear();
}

std::size_t SpawnManager::spawnerCount() const {
    return spawners.size();
}

Enemy* SpawnManager::acquire(sf::Vector2f position) {
    if (freeSlots.empty())
        return nullptr;

    int index = freeSlots.back();
    freeSlots.pop_back();

    pool[index].activate(position);
    active++;
    return &pool[index];
}

void SpawnManager::despawn(int index) {
    if (index < 0 || index >= static_cast<int>(pool.size()) || !pool[index].getActiveState())
        return;

    pool[index].deactivate();
    freeSlots.push_back(index);
    active--;
}

void SpawnManager::despawnAll() {
    for (int i = 0; i < static_cast<int>(pool.size()); i++) {
        despawn(i);
    }
}

void SpawnManager::setAmbientTarget(int target) {
    ambientTarget = std::max(target, 0);
}

int SpawnManager::getAmbientTarget() const {
    return ambientTarget;
}

std::size_t SpawnManager::capacity() const {
    return pool.size();
}

int SpawnManager::activeCount() const {
    return active;
}

int SpawnManager::freeCount() const {
    return static_cast<int>(freeSlots.size());
}

Enemy& SpawnManager::operator[](int index) {
    return pool[index];
}

const Enemy& SpawnManager::operator[](int index) const {
    return pool[index];
}

std::vector<Enemy>::iterator SpawnManager::begin() {
    return pool.begin();
}

std::vector<Enemy>::iterator SpawnManager::end() {
    return pool.end();
}

std::vector<Enemy>::const_iterator SpawnManager::begin() const {
    return pool.begin();
}

std::vector<Enemy>::const_iterator SpawnManager::end() const {
    return pool.end();
}

void SpawnManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const Enemy& enemy : pool) {
        target.draw(enemy, states);
    }
}

// A point on a ring around `center`, pushed away from it on both axes.
sf::Vector2f SpawnManager::ringPosition(sf::Vector2f center) {
    std::uniform_int_distribution<int> dist(-RING_JITTER, RING_JITTER);
    int xOffset = dist(rng);
    int yOffset = dist(rng);
    return {
        center.x + xOffset + (xOffset >= 0 ? RING_RADIUS : -RING_RADIUS),
        center.y + yOffset + (yOffset >= 0 ? RING_RADIUS : -RING_RADIUS)
    };
}

sf::Vector2f SpawnManager::scatter(sf::Vector2f center, float radius) {
    if (radius <= 0.f)
        return center;
    std::uniform_real_distribution<float> dist(-radius, radius);
    return {center.x + dist(rng), center.y + dist(rng)};
}
