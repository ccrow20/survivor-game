#include "game.hpp"

#include <cstdio>
#include <cstdlib>

game::game()
    : spawnManager(config::ENEMY_POOL_SIZE, config::AMBIENT_ENEMY_COUNT),
      Grid(config::SPATIAL_CELL_SIZE) {
    window.create(sf::VideoMode(config::WINDOW_WIDTH, config::WINDOW_HEIGHT), "Survivors");
    camera.setSize(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    camera.setCenter(config::WINDOW_WIDTH / 2, config::WINDOW_HEIGHT / 2);

    hudView.setSize(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    hudView.setCenter(config::WINDOW_WIDTH / 2, config::WINDOW_HEIGHT / 2);

    if (!font.loadFromFile(config::FONT_PATH)) {
        std::cerr << "Failed to load font: " << config::FONT_PATH << "\n";
    }
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(2.f);
    updateTimerText();

    player_attacks.emplace_back();
    State = GAME_STATE::Title;
    buildTitleScreen();
}

void game::buildTitleScreen() {
    titleText.setFont(font);
    titleText.setString("SURVIVOR GAME!");
    titleText.setCharacterSize(72);
    sf::FloatRect bounds = titleText.getLocalBounds();
    titleText.setOrigin(bounds.left + bounds.width / 2.f, 0.f);

    sf::Vector2f size(240.f, 60.f);
    title_buttons.emplace_back("Play", font, size, [this] {
        resetGame();
        State = GAME_STATE::Gameplay;
    });
    title_buttons.emplace_back("Maps", font, size, [] {});        // TODO
    title_buttons.emplace_back("Characters", font, size, [] {});  // TODO

    layoutTitleScreen();
}

void game::resetGame() {
    Player.reset();
    spawnManager.despawnAll();
    player_attacks.clear();
    player_attacks.emplace_back();
    camera.setCenter(Player.getPosition());
    elapsedTime = 0.f;
    updateTimerText();
    clock.restart();
}

void game::layoutTitleScreen() {
    sf::Vector2f s = hudView.getSize();
    titleText.setPosition(s.x / 2.f, 40.f);
    for (size_t i = 0; i < title_buttons.size(); ++i) {
        title_buttons[i].setPosition({s.x / 2.f, s.y * 0.4f + i * 90.f});
    }
}

void game::updateTimerText() {
    int total = static_cast<int>(elapsedTime);
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%02d:%02d", total / 60, total % 60);
    timerText.setString(buf);

    sf::FloatRect bounds = timerText.getLocalBounds();
    timerText.setOrigin(bounds.left + bounds.width / 2.f, 0.f);
    timerText.setPosition(hudView.getSize().x / 2.f, 10.f);
}

void game::run() {
    loadLevel("assets/maps/map1.txt");

    while (window.isOpen()) {
        processEvents();
        if (State == GAME_STATE::Title) {
            renderTitle();
        }
        else if (State == GAME_STATE::Gameplay) {
            update(clock.restart().asSeconds());
            render();
        }
        else if (State == GAME_STATE::Pause) {
            //run pause screen
        }
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
            hudView.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
            hudView.setCenter(event.size.width / 2.f, event.size.height / 2.f);
            updateTimerText();
            layoutTitleScreen();
        }
        else if (State == GAME_STATE::Title) {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window), hudView);
            if (event.type == sf::Event::MouseMoved) {
                for (auto& b : title_buttons) {
                    b.setHovered(b.contains(mouse));
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button == sf::Mouse::Left) {
                for (auto& b : title_buttons) {
                    if (b.contains(mouse)) {
                        b.click();
                        break;  // click may change State
                    }
                }
            }
        }
    }
}

void game::update(float dt) {
    Player.update(dt, Map);
    if (Player.getHealth() <= 0.f) {
        onPlayerDeath();
    }
    for (auto& playerAttack : player_attacks) {
        playerAttack.update(dt, Player.getPosition() + config::ATTACK_OFFSET);
    }
    spawnManager.update(dt, Player.getPosition());

    collision(dt);

    camera.setCenter(Player.getPosition());

    elapsedTime += dt;
    updateTimerText();
}

void game::render() {
    window.clear();
    window.setView(camera);
    window.draw(Map);
    window.draw(Player);
    for (const auto& playerAttack : player_attacks) {
        window.draw(playerAttack);
    }
    window.draw(spawnManager);

    window.setView(hudView);
    window.draw(timerText);
    window.display();
}

void game::renderTitle() {
    window.clear(sf::Color::Black);
    window.setView(hudView);

    window.draw(titleText);
    for (const auto& b : title_buttons) {
        b.draw(window);
    }
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
    std::vector<bool> touching(spawnManager.capacity(), false);
    for (const EntityID& id : Grid.queryNearby(Player.getPosition())) {
        if (id.type != EntityType::EnemyType)
            continue;

        const Enemy& goon = spawnManager[id.index];
        if (goon.getActiveState() && Player.getBounds().intersects(goon.getBounds())) {
            touching[id.index] = true;
        }
    }

    for (int i = 0; i < static_cast<int>(spawnManager.capacity()); i++) {
        if (spawnManager[i].updateContact(touching[i], dt)) {
            Player.takeDamage(config::ENEMY_CONTACT_DAMAGE);
        }
    }
}

void game::onPlayerDeath() {
    //State = GAME_STATE::Gameover
    State = GAME_STATE::Title;
}

void game::checkAttackEnemyCollisions() {
    for (auto& playerAttack : player_attacks) {
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
