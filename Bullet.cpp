#include "Bullet.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Transform2D.h"
#include <iostream>

Bullet::Bullet()
{
}

Bullet::Bullet(Terrain* terrain, glm::vec3 color, glm::vec2 origin, float ag)
{
	this->id = id;
	this->terrain = terrain;
	glm::vec3 corner = glm::vec3(0, 0, 0);
	this->pos = origin;
	this->origin = origin;
	this->ag = ag;
	bullet = object2D::CreateCircle("bullet", corner, 10, color, true);
	radius = 10;
}

Bullet::~Bullet()
{
	std::cout << "GLONT DISTRUS" << std::endl;
}

std::vector<Renderer> Bullet::Render(float deltaTime)
{
	std::vector<Renderer> renderer;
	float g = 9.8;
	float v = 150;
	glm::mat3 modelMatrix = glm::mat3(1);
	if (ag > glm::radians(90.f))
		x += deltaTime * v * (glm::radians(90.f) - ag) / glm::radians(180.f) * 20;
	else
		x += deltaTime * v * (glm::radians(90.f) - ag) / glm::radians(180.f) * 20;

	float y = x * glm::tan(ag) - (g * x * x * 2) / (v * v * glm::cos(ag) * glm::cos(ag));
	pos = { origin.x + x, origin.y + y };
	modelMatrix *= transform2D::Translate(origin.x + x, origin.y + y);
	renderer.push_back(Renderer(bullet, "VertexColor", modelMatrix));
	std::vector<int> points = terrain->getHeightMap();
	
	if (pos.x < 10 || pos.x > terrain->getHeightMap().size() * terrain->getTileSize() - 10) {
		EntityManager::getInstance().removeEntity(id);
	}
	if (points[(x + origin.x) / terrain->getTileSize()] > y + origin.y || points[(origin.x + x + 1) / terrain->getTileSize()] > y + origin.y) {
		for (int i = -40; i < 40; i++) {

			float j = 40 - abs(i);
			float pyt = sqrt(40 * 40 - i * i);
			float value = glm::max(0.f, glm::min((x + origin.x + i) / terrain->getTileSize(), (float)(terrain->getHeightMap().size() - 1)));
			points[value] = glm::min((float)points[value], origin.y + y - pyt);
		}
		terrain->setHeightMap(points);
		EntityManager::getInstance().removeEntity(id);
	}

	
	return renderer;
}
