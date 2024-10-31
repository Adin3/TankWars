#include "lab_m1/Tema1/Tema1.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Transform2D.h"

using namespace std;
using namespace m1;



TankWars::TankWars()
{
    srand(time(NULL));
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    // GetCameraInput()->SetActive(true);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    number_of_tiles = resolution.x / 1;
    squareSide = resolution.x / number_of_tiles;

    Mesh* square1 = object2D::CreateSquare("square1", corner, 1, glm::vec3(1, 1, 0.6), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, 20, glm::vec3(0, 1, 0), true);
    AddMeshToList(square2);

    for (int i = 0; i < number_of_tiles; i++) {
        points.push_back(100 * glm::sin(i * resolution.x / number_of_tiles * 0.01f - 0.7) + 350);
    }
    points.push_back(points[number_of_tiles - 1]);
    tankX = 100;
}


TankWars::~TankWars()
{
}


void TankWars::Init()
{

}


void TankWars::FrameStart()
{
    glClearColor(0.45, 0.75, 0.9, 0.1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void TankWars::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    glm::mat3 modelMatrix;
    for (int i = 0; i < number_of_tiles; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(0, points[i]);
        modelMatrix *= transform2D::Translate(i * squareSide, -points[i]);
        modelMatrix *= transform2D::Shear(0.f, (points[i+1] - points[i]) / squareSide);
        modelMatrix *= transform2D::Scale(squareSide, points[i]);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
    float sign = 1;
    float tankY = glm::mix(points[(tankX + 40) / squareSide], points[(tankX + 80) / squareSide], ((int)tankX % (int)squareSide) / squareSide);
    glm::vec2 normal = glm::vec2(40, 0);
    glm::vec2 deviation = glm::vec2(40, points[(tankX + 80) / squareSide] - tankY);
    glm::vec2 deviation1 = glm::vec2(-40, points[(tankX) / squareSide] - tankY);
    glm::vec2 addin = deviation - deviation1;
    float dot = glm::dot(glm::normalize(normal), glm::normalize(addin));
    float angle = glm::acos(dot);
    if (deviation.y < deviation1.y && sign == 1) sign *= -1;
    if (deviation.y > deviation1.y && sign == -1) sign *= -1;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tankX+40, tankY - 10);
    modelMatrix *= transform2D::Rotate(sign * angle);
    modelMatrix *= transform2D::Translate(-40, 0);
    modelMatrix *= transform2D::Scale(4, 2);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    glm::mat3 modelMatrix2 = glm::mat3(1);
    modelMatrix2 *= modelMatrix;
    //modelMatrix2 *= transform2D::Rotate(-sign * angle);
    modelMatrix2 *= transform2D::Translate(5, 20);
    modelMatrix2 *= transform2D::Scale(0.5, 0.5);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix2);

    glm::mat3 modelMatrix3 = glm::mat3(1);
    /*modelMatrix3 *= modelMatrix2;
    modelMatrix3 *= transform2D::Translate(8.5, 10);
    modelMatrix3 *= transform2D::Rotate(shootX);
    modelMatrix3 *= transform2D::Translate(-2.5, 10);
    modelMatrix3 *= transform2D::Scale(0.5, 2);*/
    modelMatrix3 *= transform2D::Translate(tankX + 40, tankY - 10);
    modelMatrix3 *= transform2D::Rotate(sign * angle);
    modelMatrix3 *= transform2D::Translate(-2, 40);
    modelMatrix3 *= transform2D::Rotate(shootX);
    modelMatrix3 *= transform2D::Translate(0, 25);
    modelMatrix3 *= transform2D::Scale(0.5, 2);
    modelMatrix3 *= transform2D::Translate(-5, -5);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix3);
}


void TankWars::FrameEnd()
{
}


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_A)) {
        tankX -= deltaTime * 100;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tankX += deltaTime * 100;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        shootX -= deltaTime * 10;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        shootX += deltaTime * 10;
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    
}


void TankWars::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void TankWars::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
