
#include "lab_m1/Tema1/EntityManager.h"
#include "lab_m1/Tema1/Player.h"
#include <iostream>
// Static method to access the singleton instance
EntityManager& EntityManager::getInstance() {
    static EntityManager instance;
    return instance;
}

// Method to add an entity
void EntityManager::addEntity(std::shared_ptr<Entity> e) {
    if (free_ids.empty()) {
        e.get()->setId(counter);
        entities[counter] = e;
        counter++;
    }
    else {
        e.get()->setId(free_ids.front());
        entities[free_ids.front()] = e;
        free_ids.pop();
    }
}

void EntityManager::addPlayer(std::shared_ptr<Player> p)
{
    if (free_ids.empty()) {
        p.get()->setId(counter);
        players[counter] = p;
        counter++;
    }
    else {
        p.get()->setId(free_ids.front());
        players[free_ids.front()] = p;
        free_ids.pop();
    }
}

// Method to get an entity by ID
std::shared_ptr<Entity> EntityManager::getEntity(int id) const {
    if (entities.find(id) != entities.end())
        return entities.find(id)->second;
}

std::unordered_map<int, std::shared_ptr<Entity>> EntityManager::getEntities() const {
    return entities;
}

std::unordered_map<int, std::shared_ptr<Player>> EntityManager::getPlayers() const
{
    return players;
}

// Method to remove an entity by ID
void EntityManager::removeEntity(int id) {
    entities.erase(id);
    free_ids.push(id);
}

void EntityManager::removePlayers(int id) {
    players.erase(id);
    free_ids.push(id);
}

void EntityManager::checkPlayerCollision(std::shared_ptr<Player> p)
{
    std::queue<int> del;
    for (auto& e : entities) {
        glm::ivec2 ePos = e.second.get()->getPosition();
        glm::ivec2 pPos = p.get()->getPosition();
        
        float distX = ePos.x - pPos.x;
        float distY = ePos.y - pPos.y;
        float distance = sqrt((distX * distX) + (distY * distY));
        
        if (distance <= e.second.get()->getRadius() + p.get()->getRadius()) {
            p.get()->getHurt();
            del.push(e.first);
        }
    }
    while (!del.empty()) {
        removeEntity(del.front());
        del.pop();
    }
}
