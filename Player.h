#pragma once

#include "lab_m1/Tema1/Entity.h"
#include "lab_m1/Tema1/Terrain.h"
#include "lab_m1/Tema1/Bullet.h"

class Player : public Entity
{
public:
    Player() {};
    Player(unsigned int ktml, unsigned int ktmr,
        unsigned int kpml, unsigned int kpmr,
        unsigned int ks, Terrain* terrain, glm::vec3 color, float posX);
    ~Player();
    void Init(Terrain* terrain, glm::vec3 color);
    std::vector<Renderer> Render(float deltaTime);
    void Input(WindowObject* window, float deltaTime);
    void getHurt();
private:
    Terrain* terrain;
    Mesh* body;
    Mesh* chassis;
    Mesh* bullet;
    Mesh* head;
    float tankX;
    float squareSide;
    float shootX = 0;
    float tankAngle = 0;
    unsigned int keyTankMoveLeft;
    unsigned int keyTankMoveRight;
    unsigned int keyPipeMoveLeft;
    unsigned int keyPipeMoveRight;
    unsigned int keyShoot;
    glm::vec3 color;
    glm::vec2 bulletOrigin;
    float ag;
    float maxShootTime = 1.f;
    float reloadShootTime = 0.f;
    int health = 3;

};
