#include "lab_m1/Tema1/Terrain.h"
#include "lab_m1/Tema1/Tema1.h"
#include "core/gpu/mesh.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Transform2D.h"

#include <iostream>

Terrain::Terrain()
{
}

Terrain::Terrain(glm::ivec2 resolution)
{
    glm::vec3 corner = glm::vec3(0, 0, 0);
    number_of_tiles = resolution.x / 10;
    tile_size = resolution.x / number_of_tiles;
    tile = object2D::CreateSquare("square1", corner, 1, glm::vec3(1, 1, 0.6), true);

    for (int i = 0; i < number_of_tiles; i++) {
        float y = 15 * glm::sin(i * tile_size * 0.025 - 0.9) +
            104 * glm::sin(i * tile_size * 0.01 + 1.3) +
            117 * glm::sin(i * tile_size * 0.01 + 5) + 250;
        points.push_back(y);
    }
    points.push_back(points[number_of_tiles - 1]);
}

Terrain::~Terrain()
{
}

std::vector<Renderer> Terrain::Render(float deltaTime)
{
    
    std::vector<Renderer> renderer;
    glm::mat3 modelMatrix;
    for (int i = 0; i < number_of_tiles; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(0, points[i]);
        modelMatrix *= transform2D::Translate(i * tile_size, -points[i]);
        modelMatrix *= transform2D::Shear(0.f, (points[i + 1] - points[i]) / tile_size);
        modelMatrix *= transform2D::Scale(tile_size, points[i]);
        renderer.push_back(Renderer(tile, "VertexColor", modelMatrix));
    }

    if (frame == true) {
        //frame = false;
        std::vector<int> tempPoints = points;
        for (int i = 0; i < number_of_tiles - 1; i++) {
            if (10 < abs(points[i] - points[i + 1])) {
                if (points[i] > points[i + 1]) {
                    tempPoints[i] -= 1;
                    tempPoints[i + 1] += 1;
                }
                else {
                    tempPoints[i + 1] -= 1;
                    tempPoints[i] += 1;
                }
            }
        }
        points = tempPoints;
    }


    //for (int i = 1; i < number_of_tiles; i++) {
    //    glm::vec2 right = glm::vec2(1, points[i + 1] - points[i]);
    //    glm::vec2 left = glm::vec2(-1, points[i - 1] - points[i]);
    //    glm::vec2 d = right - left;
    //    float dot = glm::dot(glm::normalize(right), glm::normalize(d));
    //    float angle = glm::acos(dot);
    //    //std::cout << angle << " " << std::endl;
    //    if (abs(angle) > glm::radians(40.f)) {
    //        tempPoints[i] -= 2;
    //    }
    //    /*if (2 < abs(points[i + 1] - points[i - 1]))
    //        points[i] = (points[i + 1] + points[i - 1]) / 2;*/
    //}
    
    return renderer;
}

std::vector<int> Terrain::getHeightMap()
{
    return points;
}

void Terrain::setHeightMap(std::vector<int> p)
{
    points = p;
}

float Terrain::getTileSize()
{
    return tile_size;
}

void Terrain::setFrameTrue()
{
    frame = true;
}
