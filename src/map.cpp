#include "map.hpp"

#include <algorithm>

map::map() : mapHeight(0), mapWidth(0), defaultTile{0, false, config::GROUND_IMAGE} {
    setTileType('#', {0, true, config::WALL_IMAGE});
    setTileType('~', {0, false, config::LAVA_IMAGE, config::LAVA_DAMAGE_PER_SECOND});
}

void map::setDefaultTile(TileInfo info) {
    defaultTile = std::move(info);
}

void map::setTileType(char symbol, TileInfo info) {
    tileTypes[symbol] = info;
}

const map::TileInfo& map::tileInfo(char symbol) const {
    auto it = tileTypes.find(symbol);
    return it != tileTypes.end() ? it->second : defaultTile;
}

bool map::load() {
    layers.clear();
    baseImage = defaultTile.image;
    const int tile = config::TILE_SIZE;

    auto getLayer = [&](const std::string& path) -> Layer* {
        auto it = layers.find(path);
        if (it == layers.end()) {
            it = layers.try_emplace(path).first;
            if (!it->second.texture.loadFromFile(path)) {
                std::cerr << "Failed to load tile image " << path << "\n";
                return nullptr;
            }
        }
        return &it->second;
    };

    auto addQuad = [&](Layer& layer, const std::string& path, int tilesetIndex, int i, int j) {
        int tilesPerRow = std::max(1, static_cast<int>(layer.texture.getSize().x) / tile);
        int tu = tilesetIndex % tilesPerRow;
        int tv = tilesetIndex / tilesPerRow;
        if (tilesetIndex < 0 || (tv + 1) * tile > static_cast<int>(layer.texture.getSize().y)) {
            std::cerr << "Tile index " << tilesetIndex << " is outside image " << path << "\n";
            return false;
        }

        sf::Vertex quad[4];
        quad[0].position = sf::Vector2f(i * tile, j * tile);
        quad[1].position = sf::Vector2f((i + 1) * tile, j * tile);
        quad[2].position = sf::Vector2f((i + 1) * tile, (j + 1) * tile);
        quad[3].position = sf::Vector2f(i * tile, (j + 1) * tile);

        quad[0].texCoords = sf::Vector2f(tu * tile, tv * tile);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tile, tv * tile);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tile, (tv + 1) * tile);
        quad[3].texCoords = sf::Vector2f(tu * tile, (tv + 1) * tile);

        for (const sf::Vertex& v : quad) {
            layer.vertices.append(v);
        }
        return true;
    };

    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            const TileInfo& info = tileInfo(mapTiles[j][i]);
            const std::string& imagePath = info.image.empty() ? baseImage : info.image;

            Layer* layer = getLayer(imagePath);
            bool ok = layer != nullptr;

            bool isFloor = imagePath == baseImage && info.tilesetIndex == defaultTile.tilesetIndex;
            if (ok && !isFloor) {
                Layer* floor = getLayer(baseImage);
                ok = floor != nullptr && addQuad(*floor, baseImage, defaultTile.tilesetIndex, i, j);
            }
            if (ok) {
                ok = addQuad(*layer, imagePath, info.tilesetIndex, i, j);
            }
            if (!ok) {
                layers.clear();
                return false;
            }
        }
    }
    return true;
}

bool map::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open map file " << filename << std::endl;
        return false;
    }
    file >> *this;
    return true;
}

int map::getHeight() const {
    return mapHeight;
}

int map::getWidth() const {
    return mapWidth;
}

const std::vector<std::vector<char>>& map::getMap() const {
    return mapTiles;
}

bool map::isWall(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= mapWidth || tileY >= mapHeight) {
        return true;
    }
    return tileInfo(mapTiles[tileY][tileX]).solid;
}

bool map::collides(sf::Vector2f pos, sf::Vector2f size) const {
    int left = toTile(pos.x);
    int right = toTile(pos.x + size.x);
    int top = toTile(pos.y);
    int bottom = toTile(pos.y + size.y);

    for (int y = top; y <= bottom; ++y) {
        for (int x = left; x <= right; ++x) {
            if (isWall(x, y)) {
                return true;
            }
        }
    }
    return false;
}

float map::damageAt(sf::Vector2f pos, sf::Vector2f size) const {
    float damage = 0.f;
    for (int y = toTile(pos.y); y <= toTile(pos.y + size.y); ++y) {
        for (int x = toTile(pos.x); x <= toTile(pos.x + size.x); ++x) {
            if (x >= 0 && y >= 0 && x < mapWidth && y < mapHeight) {
                damage = std::max(damage, tileInfo(mapTiles[y][x]).damagePerSecond);
            }
        }
    }
    return damage;
}

int map::toTile(float pos) {
    return static_cast<int>(pos) / config::TILE_SIZE;
}

std::istream& operator>>(std::istream& in, map& s) {
    std::string temp;
    in >> s.mapHeight >> s.mapWidth;
    s.mapTiles.assign(s.mapHeight, std::vector<char>(s.mapWidth));

    for (int i = 0; i < s.mapHeight; i++) {
        in >> temp;
        for (int j = 0; j < s.mapWidth; j++) {
            s.mapTiles[i][j] = temp[j];
        }
    }
    return in;
}

void map::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    auto drawLayer = [&](const Layer& layer) {
        sf::RenderStates layerStates = states;
        layerStates.texture = &layer.texture;
        target.draw(layer.vertices, layerStates);
    };

    auto base = layers.find(baseImage);
    if (base != layers.end())
        drawLayer(base->second);
    for (const auto& [path, layer] : layers) {
        if (path != baseImage)
            drawLayer(layer);
    }
}
