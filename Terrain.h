#pragma once

#include "lab_m1/Tema1/Entity.h"

class Entity;

class Terrain : public Entity
{
public:
    Terrain();
    Terrain(glm::ivec2 resolution);
    ~Terrain();
    std::vector<Renderer> Render(float deltaTime);
    std::vector<int> getHeightMap();
    void setHeightMap(std::vector<int> p);
    float getTileSize();
    void setFrameTrue();
private:
    float number_of_tiles;
    float tile_size;
    Mesh* tile;
    std::vector<int> points;
    bool frame = true;
};
