#include "game.hpp"

#include <cstdlib>

game::game()
    : spawnManager(config::ENEMY_POOL_SIZE, config::AMBIENT_ENEMY_COUNT),
      Grid(config::SPATIAL_CELL_SIZE) {
    window.create(sf::VideoMode(config::WINDOW_WIDTH, config::WINDOW_HEIGHT), "Survivors");
    camera.setSize(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    camera.setCenter(config::WINDOW_WIDTH / 2, config::WINDOW_HEIGHT / 2);

    playerAttacks.emplace_back();
}

void game::run() {
    loadLevel("assets/maps/map1.txt");

    while (window.isOpen()) {
        processEvents();
        update(clock.restart().asSeconds());
        render();
    }
}

void game::loadLevel(const std::string& mapFile) {
    Map.loadFromFile(mapFile);
    if (!Map.load()) {
        std::cerr << "Failed to load level graphics\n";
    }
}

void game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::Resized) {
            camera.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
        }
    }
}

void game::update(float dt) {
    Player.update(dt, Map);
    if (Player.getHealth() <= 0.f) {
        onPlayerHit();
    }
    for (auto& playerAttack : playerAttacks) {
        playerAttack.update(dt, Player.getPosition() + config::ATTACK_OFFSET);
    }
    spawnManager.update(dt, Player.getPosition());

    collision(dt);

    camera.setCenter(Player.getPosition());
}

void game::render() {
    window.clear();
    window.setView(camera);
    window.draw(Map);
    window.draw(Player);
    for (const auto& playerAttack : playerAttacks) {
        window.draw(playerAttack);
    }
    window.draw(spawnManager);
    window.display();
}

void game::rebuildSpatialHash() {
    Grid.clear();
    Grid.insert(Player.getPosition(), EntityID{EntityType::PlayerType, 0});
    for (int i = 0; i < static_cast<int>(spawnManager.capacity()); i++) {
        Grid.insert(spawnManager[i].getPosition(), EntityID{EntityType::EnemyType, i});
    }
}

void game::collision(float dt) {
    rebuildSpatialHash();
    checkPlayerEnemyCollisions(dt);
    checkAttackEnemyCollisions();
    separateEnemies(dt);
}

void game::checkPlayerEnemyCollisions(float dt) {
    bool touching = false;
    for (const EntityID& id : Grid.queryNearby(Player.getPosition())) {
        if (id.type != EntityType::EnemyType)
            continue;

        const Enemy& goon = spawnManager[id.index];
        if (goon.getActiveState() && Player.getBounds().intersects(goon.getBounds())) {
            touching = true;
            break;
        }
    }

    if (!touching) {
        contactCooldown = 0.f;
        return;
    }

    contactCooldown -= dt;
    if (contactCooldown <= 0.f) {
        Player.takeDamage(config::ENEMY_CONTACT_DAMAGE);
        contactCooldown = config::ENEMY_CONTACT_INTERVAL;
    }
}

void game::onPlayerHit() {
    std::exit(1);
}

void game::checkAttackEnemyCollisions() {
    for (auto& playerAttack : playerAttacks) {
        if (!playerAttack.getActiveState())
            continue;

        for (const EntityID& id : Grid.queryArea(playerAttack.getBounds())) {
            if (id.type != EntityType::EnemyType)
                continue;

            Enemy& goon = spawnManager[id.index];
            if (goon.getActiveState() && playerAttack.getBounds().intersects(goon.getBounds())
                && playerAttack.registerHit(id)) {
                goon.takeDmg(playerAttack.getDamage(), id.index);
            }
        }
    }
}

void game::separateEnemies(float dt) {
    for (int i = 0; i < static_cast<int>(spawnManager.capacity()); i++) {
        Enemy& goon = spawnManager[i];

        for (const EntityID& id : Grid.queryNearby(goon.getPosition())) {
            if (id.type != EntityType::EnemyType || id.index == i)
                continue;

            Enemy& other = spawnManager[id.index];

            sf::FloatRect nextBounds = goon.getBounds();
            sf::Vector2f velocity = goon.getVelocity(dt);
            nextBounds.left += velocity.x * dt;
            nextBounds.top += velocity.y * dt;

            if (goon.getActiveState() && other.getActiveState() && nextBounds.intersects(other.getBounds())) {
                goon.Slide(other);
            }
        }
    }
}
