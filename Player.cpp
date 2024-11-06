#include "lab_m1/Tema1/Player.h"
#include "lab_m1/Tema1/Tema1.h"
#include "core/gpu/mesh.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Transform2D.h"
#include <iostream>

Player::Player(unsigned int ktml, unsigned int ktmr,
    unsigned int kpml, unsigned int kpmr,
    unsigned int ks, Terrain* terrain, glm::vec3 color, float posX) {

    this->terrain = terrain;

    glm::vec3 corner = glm::vec3(0, 0, 0);

    body = object2D::CreateSquare("square2", corner, 20, color / glm::vec3(3, 3, 1), true);
    bullet = object2D::CreateCircle("bullet", corner, 5, glm::vec3(1, 1, 1), true);
    chassis = object2D::CreateChassis("chassis", corner, 20, color, true);
    head = object2D::CreateCircle("circle", corner, 20, color, true);

    tankX = posX;
    this->color = color;
    this->keyTankMoveLeft = ktml;
    this->keyTankMoveRight = ktmr;
    this->keyPipeMoveLeft = kpml;
    this->keyPipeMoveRight = kpmr;
    this->keyShoot = ks;
    pos = { tankX, terrain->getHeightMap()[tankX] };
    radius = 40;
}

Player::~Player() {

}

void Player::Init(Terrain* terrain, glm::vec3 color)
{   
    
}

std::vector<Renderer> Player::Render(float deltaTime)
{
    reloadShootTime += deltaTime;

    std::vector<Renderer> renderer;
    std::vector<int> points = terrain->getHeightMap();
    squareSide = terrain->getTileSize();
    float sign = 1;
    float tankY = glm::mix(points[(tankX + 40) / squareSide], points[(tankX + 80) / squareSide], ((int)tankX % (int)squareSide) / squareSide);
    glm::vec2 normal = glm::vec2(40, 0);
    glm::vec2 deviation = glm::vec2(40, points[(tankX + 80) / squareSide] - tankY);
    glm::vec2 deviation1 = glm::vec2(-40, points[glm::max(0.f,(tankX)) / squareSide] - tankY);
    glm::vec2 addin = deviation - deviation1;
    float dot = glm::dot(glm::normalize(normal), glm::normalize(addin));
    float angle = glm::acos(dot);
    if (deviation.y < deviation1.y && sign == 1) sign *= -1;
    if (deviation.y > deviation1.y && sign == -1) sign *= -1;
    tankAngle = sign * angle;

    pos = { tankX + 40, tankY - 10 };

    glm::mat3 modelMatrix;
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tankX + 40, tankY - 10);
    modelMatrix *= transform2D::Rotate(sign * angle);
    modelMatrix *= transform2D::Translate(-40, 0);
    modelMatrix *= transform2D::Scale(4, 4);
    renderer.push_back(Renderer(chassis, "VertexColor", modelMatrix));

    glm::mat3 modelMatrix2 = glm::mat3(1);
    modelMatrix2 *= modelMatrix;
    modelMatrix2 *= transform2D::Translate(10, 8);
    modelMatrix2 *= transform2D::Scale(0.25, 0.25);
    renderer.push_back(Renderer(head, "VertexColor", modelMatrix2));

    glm::mat3 modelMatrix3 = glm::mat3(1);
    modelMatrix3 *= transform2D::Translate(tankX + 40, tankY - 10);
    modelMatrix3 *= transform2D::Rotate(sign * angle);
    modelMatrix3 *= transform2D::Translate(-2, 40);
    modelMatrix3 *= transform2D::Rotate(shootX - sign * angle);
    modelMatrix3 *= transform2D::Translate(0, 15);
    modelMatrix3 *= transform2D::Scale(0.5, 2);
    modelMatrix3 *= transform2D::Translate(-5, -5);
    renderer.push_back(Renderer(body, "VertexColor", modelMatrix3));

    glm::mat3 bulletModelMatrix = glm::mat3(1);
    bulletModelMatrix *= modelMatrix3;
    bulletModelMatrix *= transform2D::Translate(2, 20);

    bulletOrigin = glm::vec2(bulletModelMatrix[2][0], bulletModelMatrix[2][1]);
    //renderer.push_back(Renderer(bullet, "VertexColor", bulletModelMatrix));
    ag = glm::radians(90.f) + shootX;
    float g = 9.8;
    float v = 150;
    float x = bulletOrigin.x;
    /*std::cout << ag << " " << std::endl;
    std::cout << (glm::radians(90.f) - ag) / glm::radians(180.f) << std::endl;*/
    for (int i = 25; i <= 800; i += 25) {
        float x;
        if (ag > glm::radians(90.f))
            x = -i;
        else
            x = i;
        float y = x * glm::tan(ag) - (g * x * x * 2) / (v * v * glm::cos(ag) * glm::cos(ag));
        bulletModelMatrix = glm::mat3(1);
        bulletModelMatrix *= transform2D::Translate(bulletOrigin.x + x + 2, bulletOrigin.y + y + 2);
        renderer.push_back(Renderer(bullet, "VertexColor", bulletModelMatrix));
    }
    return renderer;
}

void Player::Input(WindowObject* window, float deltaTime)
{
    if (window->KeyHold(keyTankMoveLeft) && tankX > 1.f) {
        tankX -= deltaTime * 100;
    }
    if (window->KeyHold(keyTankMoveRight) && tankX < terrain->getTileSize() * terrain->getHeightMap().size() - 82) {
        tankX += deltaTime * 100;
    }
    if (window->KeyHold(keyPipeMoveLeft) && shootX > -1.3 + tankAngle) {
        shootX -= deltaTime * 2;
    }
    if (window->KeyHold(keyPipeMoveRight) && shootX < 1.5 + tankAngle) {
        shootX += deltaTime * 2;
    }
    if (window->KeyHold(keyShoot) && reloadShootTime >= maxShootTime) {
        reloadShootTime = 0;
        EntityManager::getInstance().addEntity(std::make_shared<Bullet>(terrain, color, bulletOrigin, ag));
    }
}

void Player::getHurt()
{
    health--;
    if (health == 0) {
        std::cout << "MORT";
        EntityManager::getInstance().removePlayers(id);
    }
    std::cout << "RIP" << health;
}
