#pragma once

#include "lab_m1/Tema1/Entity.h"
#include "lab_m1/Tema1/Terrain.h"

class Bullet : public Entity
{
public:
    Bullet();
    Bullet(Terrain* terrain, glm::vec3 color, glm::vec2 origin, float ag);
    ~Bullet();
    std::vector<Renderer> Render(float deltaTime) override;
private:
    glm::vec2 origin;
    Terrain* terrain;
    float ag;
    glm::vec3 color;
    Mesh* bullet;
    float x = 0;
};