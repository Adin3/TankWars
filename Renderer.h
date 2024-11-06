#pragma once

#include "components/simple_scene.h"

struct Renderer {
	Mesh* tile;
	std::string shader;
	glm::mat3 model;

	Renderer(Mesh* tile, std::string shader, glm::mat3 model) {
		this->tile = tile;
		this->shader = shader;
		this->model = model;
	}
};