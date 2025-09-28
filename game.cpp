#include "game.hpp"

game::game() : Player(), playerAttack(Player), Grid(64) {
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Survivors");
    camera.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    camera.setCenter(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    for (int i = 0; i < 100; i++) {
        Goons.emplace_back(Player);
    }
}

void game::run() {
    changeMap("map1.txt");
    if (!Map.load("ground_01.png")) {
        std::cerr << "Failed to load tileset\n";
    }
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        Grid.clear();
        float deltaTime = clock.restart().asSeconds();
        Player.update(deltaTime, Map.getMap());
        playerAttack.update(deltaTime);
        for (auto& goon : Goons) {
            goon.update(deltaTime);
        }

        collision(deltaTime);

        camera.setCenter(Player.getPosition());
        window.clear();
        window.setView(camera);
        window.draw(Map);
        window.draw(Player);
        window.draw(playerAttack);
        for (auto& goon : Goons) {
            goon.draw(window);
        }
        window.display(); 
    }
}

void game::changeMap(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open map file " << filename << std::endl;
    }

    file >> Map;
}

void game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void game::update() {
    float deltaTime = clock.restart().asSeconds();

    Player.update(deltaTime, Map.getMap());
    camera.setCenter(Player.getPosition());
}

void game::render() {
   window.clear();
   window.setView(camera);
   window.draw(Map);
   window.draw(Player);
   window.display(); 
}

void game::collision(float dt) {
    std::vector<EntityID> candidates;
    Grid.insert(Player.getPosition(), EntityID{EntityType::PlayerType, 0});
    for (int i = 0; i < static_cast<int>(Goons.size()); i++) {
        Grid.insert(Goons[i].getPosition(), EntityID{EntityType::EnemyType, i});
    }

    sf::Vector2f playerPos = Player.getPosition();
    sf::Vector2i cell = Grid.worldToCell(playerPos);
    candidates.clear();
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            sf::Vector2i neighbor(cell.x + dx, cell.y + dy);

            auto local = Grid.query(neighbor);
            candidates.insert(candidates.end(), local.begin(), local.end());
        }
    }

    for (EntityID& id : candidates) {
        if (id.type == EntityType::EnemyType) {
            if (Player.getBounds().intersects(Goons[id.index].getBounds()) && Goons[id.index].getActiveState()) {
                std::exit(1);
            }
        }
    }

    sf::FloatRect attackBounds = playerAttack.getBounds();
    sf::Vector2i minCell = Grid.worldToCell({attackBounds.left, attackBounds.top});
    sf::Vector2i maxCell = Grid.worldToCell({attackBounds.left + attackBounds.width, attackBounds.top + attackBounds.height});

    minCell.x -= 1;
    minCell.y -= 1;
    maxCell.x += 1;
    maxCell.y += 1;


    candidates.clear();
    for (int y = minCell.y; y <= maxCell.y; y++) {
        for (int x = minCell.x; x <= maxCell.x; x++) {
            sf::Vector2i cell(x, y);

            auto local = Grid.query(cell);
            candidates.insert(candidates.end(), local.begin(), local.end());
        }
    }

    for (EntityID& id : candidates) {
        if (id.type == EntityType::EnemyType) {
            if (playerAttack.getBounds().intersects(Goons[id.index].getBounds()) && playerAttack.getActiveState() == true && Goons[id.index].getActiveState() == true) {
                if (playerAttack.getSet().find(id) == playerAttack.getSet().end()) {
                    playerAttack.getSet().insert(id);
                    Goons[id.index].takeDmg(playerAttack.getDamage(), id.index);
                }
            }
        }
    }

    


    for (int i = 0; i < static_cast<int>(Goons.size()); i++) {
        sf::Vector2f pos = Goons[i].getPosition();

        cell = Grid.worldToCell(pos);
        candidates.clear();
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                sf::Vector2i neighbor(cell.x + dx, cell.y + dy);

                auto local = Grid.query(neighbor);
                candidates.insert(candidates.end(), local.begin(), local.end());
            }
        }

        for (EntityID& id : candidates) {
            if (id.index == i) continue;
            sf::FloatRect nextBounds = Goons[i].getBounds();
            sf::Vector2f velocity = Goons[i].getVelocity(dt);
            nextBounds.left += velocity.x * dt;
            nextBounds.top += velocity.y * dt;

            if (nextBounds.intersects(Goons[id.index].getBounds()) && Goons[i].getActiveState() == true && Goons[id.index].getActiveState() == true) {
                Goons[i].Slide(Goons[id.index]);
            }
        }
    }
}
