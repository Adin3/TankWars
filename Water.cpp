#include "Water.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Transform2D.h"

Water::Water()
{
	body = object2D::CreateSquare("bullet", glm::vec3(0, 0, 1), 10, glm::vec3(0, 0, 1), true);
}

Water::~Water()
{
}

std::vector<Renderer> Water::Render(float deltaTime)
{
	std::vector<Renderer> renderer;
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Scale(130, 10);

	renderer.push_back(Renderer(body, "WaterShader", modelMatrix));
	return renderer;
}
