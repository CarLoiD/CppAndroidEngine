#include "graphics_context.h"

#include <GLES2/gl2.h>

void GraphicsContext::Create(const uint32_t width, const uint32_t height)
{
    mWidth  = width;
    mHeight = height;
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

uint32_t GraphicsContext::GetDisplayWidth() const
{
    return mWidth;
}

uint32_t GraphicsContext::GetDisplayHeight() const
{
    return mHeight;
}