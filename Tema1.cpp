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
    glm::ivec2 pixelatedResolution = glm::ivec2(320, 240);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader* shader = new Shader("WaterShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Water.FS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    shader = new Shader("FrameBufferShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Screen.FS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.x, resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution.x, resolution.y); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    frameBufferMesh = object2D::CreateSquareTex("framebuffer", glm::vec3(0), 1, glm::vec3(1, 1, 1), true);

    water = Water();
    terrain = Terrain(resolution);
}


TankWars::~TankWars()
{
}


void TankWars::Init()
{

}

void TankWars::FrameStart()
{
    glm::ivec2 resolution = window->GetResolution();
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.x, resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // Also resize the renderbuffer if used
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution.x, resolution.y);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.45, 0.75, 0.9, 0.1);
    //glClearColor(0.0f, 0.0f, 0.f, 0.1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void TankWars::Update(float deltaTimeSeconds)
{
    int loc = shaders["WaterShader"]->GetUniformLocation("time");
    shaders["WaterShader"]->Use();
    glUniform1f(loc, (float)glfwGetTime());
    // Send time uniform to WaterShader
    switch (gameState) {
    case GameState::MENU:

        for (auto& r : terrain.Render(deltaTimeSeconds)) {
            RenderMesh2D(r.tile, shaders[r.shader], r.model);
        }

        for (auto& r : water.Render(deltaTimeSeconds)) {
            RenderMesh2D(r.tile, shaders[r.shader], r.model);
        }

        if (window->KeyHold(GLFW_KEY_SPACE)) {
            gameState = GameState::PREGAME;
        }
        break;
    case GameState::PREGAME:
        EntityManager::getInstance().addPlayer(std::make_shared<Player>(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_SPACE, &terrain, glm::vec3(0, 1, 0), 100));
        EntityManager::getInstance().addPlayer(std::make_shared<Player>(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_ENTER, &terrain, glm::vec3(1, 0, 0), 900));
        gameState = GameState::GAME;
        break;
    case GameState::GAME:

        // Render terrain and entities
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
        if (EntityManager::getInstance().getPlayers().size() < 2)
            gameState = GameState::END;
        break;
    case GameState::END:
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
        if (window->KeyHold(GLFW_KEY_R)) {
            EntityManager::getInstance().clearPlayers();
            
            terrain = Terrain(window->GetResolution());

            gameState = GameState::PREGAME;
        }
        break;
    }
    

    // Switch to default framebuffer and render the full-screen quad
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // Disable depth test for screen-space quad

    // Clear screen
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec2 res = window->GetResolution();
    // glViewport(0, 0, res.x, res.y); // Set viewport to full window size

    // Set up model matrix for full-screen quad rendering
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(res.x + scaleF * 2.5 + 40, res.y + scaleF + 40, 1.0f)); // Scale quad to cover entire screen
    // model = glm::scale(model, glm::vec3(res.x, res.y, 1.0f));
    // Render the quad with framebuffer texture
    RenderSimpleMesh(frameBufferMesh, shaders["FrameBufferShader"], model, textureColorbuffer);
}


void TankWars::FrameEnd()
{
}

void TankWars::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, unsigned int texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);             // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, texture1);   // Bind the texture to the active texture unit
        int loc_texture = glGetUniformLocation(shader->program, "texture1");
        glUniform1i(loc_texture, 0);              // Set the sampler uniform to use texture unit 0
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    for (auto& p : EntityManager::getInstance().getPlayers()) {
        p.second->Input(window, deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_1)) {
        scaleF -= deltaTime * 50;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleF += deltaTime * 50;
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
