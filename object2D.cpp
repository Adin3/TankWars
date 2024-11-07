#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color / glm::vec3(4)),
        VertexFormat(corner + glm::vec3(length, 0, 0), color / glm::vec3(4)),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_POINTS);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateSquareTex(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    glm::vec3 normal = glm::vec3(0, 1, 0);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color, normal, glm::vec2(0.0f, 0.0f)),
        VertexFormat(corner + glm::vec3(length, 0, 0), color, normal, glm::vec2(1.0f, 0.0f)),
        VertexFormat(corner + glm::vec3(length, length, 0), color, normal, glm::vec2(1.0f, 1.0f)),
        VertexFormat(corner + glm::vec3(0, length, 0), color, normal, glm::vec2(0.0f, 1.0f))
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_POINTS);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(corner, color));

    for (float theta = 0.f; theta <= 360.f; theta += 360.f / 12.f) {
        vertices.push_back(VertexFormat(corner + glm::vec3(length * glm::sin(glm::radians(theta)), length * glm::cos(glm::radians(theta)), 0), color / glm::vec3(1.5)));
    }

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    std::vector<unsigned int> indices;

    indices.push_back(0);  // Center point

    // Alternate between the edge points
    for (int i = 1; i <= 12; ++i) {
        indices.push_back(i);
    }

    indices.push_back(1);

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateChassis(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, 0.2 * length, 0), color / glm::vec3(2)),
        VertexFormat(corner + glm::vec3(0.1 * length, 0.4 * length, 0), color),
        VertexFormat(corner + glm::vec3(0.2 * length, 0.2 * length, 0), color / glm::vec3(2)),
        VertexFormat(corner + glm::vec3(0.3 * length, 0, 0), color / glm::vec3(4)),
        VertexFormat(corner + glm::vec3(0.7 * length, 0, 0), color / glm::vec3(4)),
        VertexFormat(corner + glm::vec3(0.8 * length, 0.2 * length, 0), color / glm::vec3(2)),
        VertexFormat(corner + glm::vec3(0.9 * length, 0.4 * length, 0), color),
        VertexFormat(corner + glm::vec3(1 * length, 0.2 * length, 0), color / glm::vec3(2)),
    };

    Mesh* chassis = new Mesh(name);
    chassis->SetDrawMode(GL_TRIANGLES);
    std::vector<unsigned int> indices = { 
        2, 3, 4,
        4, 5, 2,
        1, 0, 7,
        7, 6, 1,
    };

    chassis->InitFromData(vertices, indices);
    return chassis;
}