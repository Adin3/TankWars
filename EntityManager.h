#pragma once

#include <memory>
#include <unordered_map>
#include <queue>
#include "lab_m1/Tema1/Entity.h"

class Player;
class Entity;

class EntityManager 
{
private:
    // Private constructor to prevent instantiation from outside
    EntityManager() = default;
    ~EntityManager() = default;

    // Disallow copying and assignment
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    // Storage for entities
    std::unordered_map<int, std::shared_ptr<Entity>> entities;
    std::unordered_map<int, std::shared_ptr<Player>> players;
    int counter = 0;
    std::queue<int> free_ids;

public:
    // Static method to access the singleton instance
    static EntityManager& getInstance();

    // Method to add an entity
    void addEntity(std::shared_ptr<Entity> e);

    void addPlayer(std::shared_ptr<Player> p);

    void clearPlayers();
    // Method to get an entity by ID
    std::shared_ptr<Entity> getEntity(int id) const;

    std::unordered_map<int, std::shared_ptr<Entity>> getEntities() const;

    std::unordered_map<int, std::shared_ptr<Player>> getPlayers() const;

    // Method to remove an entity by ID
    void removeEntity(int id);

    void removePlayers(int id);

    void checkPlayerCollision(std::shared_ptr<Player> p);
};