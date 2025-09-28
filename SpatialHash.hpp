#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <cmath>

struct Vector2iHash {
    std::size_t operator()(const sf::Vector2i& v) const noexcept {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};

enum class EntityType {PlayerType, EnemyType, AttackType}; 
struct EntityID {
  EntityType type;
  int index;

  bool operator==(const EntityID& other) const {
    return type == other.type && index == other.index;
  }
};

namespace std {
    template <>
    struct hash<EntityID> {
        std::size_t operator()(const EntityID& id) const noexcept {
            return (static_cast<std::size_t>(id.type) << 32) ^ std::hash<int>()(id.index);
        }
    };
}

class SpatialHash {
public:
  SpatialHash(float cellSize) : m_cellSize(cellSize) {}
  void clear();
  void insert(sf::Vector2f pos, EntityID eType);
  std::vector<EntityID> query(sf::Vector2f pos) const;
  std::vector<EntityID> query(sf::Vector2i cell) const;
  sf::Vector2i worldToCell(sf::Vector2f pos) const;
  float distance(const sf::Vector2f& a, const sf::Vector2f& b);

private:
  float m_cellSize;
  std::unordered_map<sf::Vector2i, std::vector<EntityID>, Vector2iHash> m_cells;
};
