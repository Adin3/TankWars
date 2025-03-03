#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Terrain.h"
#include "lab_m1/Tema1/Player.h"
#include "lab_m1/Tema1/Renderer.h"
#include "lab_m1/Tema1/EntityManager.h"
#include "lab_m1/Tema1/Water.h"

class TankWars : public gfxc::SimpleScene
{
public:
    TankWars();
    ~TankWars();

    void Init() override;
    void Render(Mesh* mesh, std::string shader, glm::mat3 model);

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;
    void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, unsigned int texture1);

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

    std::vector<int> points;
    int number_of_tiles;
    float shYX = 0.f;
    float squareSide;
    float tankX;
    float shootX = 0;
    Terrain terrain;
    Player player;
    Player player1;
    Water water;
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int rbo;
    Mesh* frameBufferMesh;
    float scaleF = 0;
    enum GameState {
        MENU,
        PREGAME,
        GAME,
        END
    } gameState = MENU;
};
