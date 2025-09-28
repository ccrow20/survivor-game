#include "SpatialHash.hpp"

sf::Vector2i SpatialHash::worldToCell(sf::Vector2f pos) const {
    return {
        static_cast<int>(std::floor(pos.x / m_cellSize)),
        static_cast<int>(std::floor(pos.y / m_cellSize))
    };
}

std::vector<EntityID> SpatialHash::query(sf::Vector2f pos) const {
    sf::Vector2i cell = worldToCell(pos);
    auto it = m_cells.find(cell);
    if (it != m_cells.end())
        return it->second;
    return {};
}

std::vector<EntityID> SpatialHash::query(sf::Vector2i cell) const {
    auto it = m_cells.find(cell);
    if (it != m_cells.end())
        return it->second;
    return {};
}

void SpatialHash::insert(sf::Vector2f pos, EntityID eType) {
    sf::Vector2i cell = worldToCell(pos);
    m_cells[cell].push_back(eType);
}

void SpatialHash::clear() {
    m_cells.clear();
}

float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}
