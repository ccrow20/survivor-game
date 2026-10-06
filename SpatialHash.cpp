#include "SpatialHash.hpp"

sf::Vector2i SpatialHash::worldToCell(sf::Vector2f pos) const {
    return {
        static_cast<int>(std::floor(pos.x / m_cellSize)),
        static_cast<int>(std::floor(pos.y / m_cellSize))
    };
}

std::vector<EntityID> SpatialHash::query(sf::Vector2f pos) const {
    return query(worldToCell(pos));
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

std::vector<EntityID> SpatialHash::queryNearby(sf::Vector2f pos) const {
    sf::Vector2i cell = worldToCell(pos);
    std::vector<EntityID> result;
    appendCells(result, {cell.x - 1, cell.y - 1}, {cell.x + 1, cell.y + 1});
    return result;
}

std::vector<EntityID> SpatialHash::queryArea(const sf::FloatRect& area) const {
    sf::Vector2i minCell = worldToCell({area.left, area.top});
    sf::Vector2i maxCell = worldToCell({area.left + area.width, area.top + area.height});

    std::vector<EntityID> result;
    appendCells(result, {minCell.x - 1, minCell.y - 1}, {maxCell.x + 1, maxCell.y + 1});
    return result;
}

void SpatialHash::appendCells(std::vector<EntityID>& out, sf::Vector2i minCell, sf::Vector2i maxCell) const {
    for (int y = minCell.y; y <= maxCell.y; y++) {
        for (int x = minCell.x; x <= maxCell.x; x++) {
            auto it = m_cells.find({x, y});
            if (it != m_cells.end())
                out.insert(out.end(), it->second.begin(), it->second.end());
        }
    }
}
