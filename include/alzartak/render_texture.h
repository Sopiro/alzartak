#pragma once

#include "common.h"

namespace alzartak
{

class RenderTexture
{
public:
    RenderTexture() = default;
    ~RenderTexture()
    {
        Destroy();
    }

    void Create(Point2i new_size)
    {
        if (size == new_size)
        {
            return;
        }
        size = new_size;

        // Clear old resources
        if (color) glDeleteTextures(1, &color);
        if (depth) glDeleteRenderbuffers(1, &depth);
        if (fbo) glDeleteFramebuffers(1, &fbo);

        // Create FBO
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Color texture
        glGenTextures(1, &color);
        glBindTexture(GL_TEXTURE_2D, color);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

        // Depth render buffer
        glGenRenderbuffers(1, &depth);
        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth);

        WakAssert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Destroy()
    {
        if (color) glDeleteTextures(1, &color);
        if (depth) glDeleteRenderbuffers(1, &depth);
        if (fbo) glDeleteFramebuffers(1, &fbo);
        color = depth = fbo = 0;
    }

    void Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint fbo = 0;
    GLuint color = 0;
    GLuint depth = 0;
    Point2i size = Vec2i::zero;
};

} // namespace alzartak
