#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

std::vector<Renderer> Entity::Render(float deltaTime)
{
    return std::vector<Renderer>();
}

void Entity::setId(int id)
{
    this->id = id;
}

glm::ivec2 Entity::getPosition()
{
    return pos;
}

float Entity::getRadius()
{
    return radius;
}
