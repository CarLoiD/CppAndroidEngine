#include "main.h"

void Application::Create()
{
}

void Application::Update(const float deltaTime)
{
    const TouchScreenId id = TouchScreenId::Touch;

    const float posX = g_displayInput.GetTouchScreenX(id);
    const float posY = g_displayInput.GetTouchScreenY(id);

    glClearColor(posX, posY, (posX + posY) / 2.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Application::Destroy()
{
}