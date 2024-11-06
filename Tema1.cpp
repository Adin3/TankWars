#include "lab_m1/Tema1/Tema1.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Transform2D.h"

using namespace std;



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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader* shader = new Shader("WaterShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Water.FS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    water = Water();

    
    terrain = Terrain(resolution);
    EntityManager::getInstance().addPlayer(std::make_shared<Player>(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_SPACE, &terrain, glm::vec3(0, 1, 0), 100));
    //EntityManager::getInstance().addPlayer(std::make_shared<Player>(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_ENTER, &terrain, glm::vec3(1, 0, 0), 600));
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
   
    // std::cout << "FPS: " << 1000 / deltaTimeSeconds / 1000 << std::endl;
    int loc = shaders["WaterShader"]->GetUniformLocation("time");
    shaders["WaterShader"]->Use();
    glUniform1f(loc, (float)glfwGetTime());

    for (auto& r : terrain.Render(deltaTimeSeconds)) {
        RenderMesh2D(r.tile, shaders[r.shader], r.model);
    }
    for (auto& e : EntityManager::getInstance().getPlayers()) {
        for (auto& p : e.second->Render(deltaTimeSeconds)) {
            RenderMesh2D(p.tile, shaders[p.shader], p.model);
        }
    }

    for (auto& e : EntityManager::getInstance().getEntities()) {
        for (auto& r : e.second->Render(deltaTimeSeconds)) {
            RenderMesh2D(r.tile, shaders[r.shader], r.model);
        }
    }

    for (auto& p : EntityManager::getInstance().getPlayers()) {
        EntityManager::getInstance().checkPlayerCollision(p.second);
    }

    for (auto& r : water.Render(deltaTimeSeconds)) {
        RenderMesh2D(r.tile, shaders[r.shader], r.model);
    }
}


void TankWars::FrameEnd()
{
}


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    for (auto& p : EntityManager::getInstance().getPlayers()) {
        p.second->Input(window, deltaTime);
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_L) {
        terrain.setFrameTrue();
    }
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
