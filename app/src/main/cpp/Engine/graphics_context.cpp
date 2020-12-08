#include "graphics_context.h"

void GraphicsContext::Create(const uint32_t width, const uint32_t height)
{
    Width  = width;
    Height = height;
}

void GraphicsContext::ClearBackBuffer(const float r, const float g, const float b, const float a)
{
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearDepthf(1.0f);
}

void GraphicsContext::EnableDepthBufferTesting()
{
    glEnable(GL_DEPTH_TEST);
}

void GraphicsContext::DisableDepthBufferTesting()
{
    glDisable(GL_DEPTH_TEST);
}

void GraphicsContext::Draw(const PrimitiveType& type, const uint32_t offset, const uint32_t count)
{
    glDrawArrays((uint32_t)type, offset, count);
}

void GraphicsContext::DrawIndexed(const PrimitiveType& type, const uint32_t count)
{
    glDrawElements((uint32_t)type, count, GL_UNSIGNED_SHORT, nullptr);
}

uint32_t GraphicsContext::GetDisplayWidth() const
{
    return Width;
}

uint32_t GraphicsContext::GetDisplayHeight() const
{
    return Height;
}