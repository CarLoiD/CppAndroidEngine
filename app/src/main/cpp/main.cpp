#include "main.h"

void Application::Create()
{
}

void Application::Update(const float deltaTime)
{
    const TouchScreenId id = TouchScreenId::Touch;

    const float posX = GetTouchScreenX(id);
    const float posY = GetTouchScreenY(id);

    gfxClearBackBuffer(45.0f, 45.0f, 45.0f);
}

void Application::Destroy()
{
}