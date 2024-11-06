#include "lab_m1/Tema1/Entity.h"

class Water
{
public:
    Water();
    ~Water();
    std::vector<Renderer> Render(float deltaTime);
private:
    Mesh* body;
};