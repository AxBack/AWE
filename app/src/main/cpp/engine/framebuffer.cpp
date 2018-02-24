#include "framebuffer.h"

namespace Engine {
    bool Framebuffer::init(int width, int height, bool alpha, bool depth)
    {
        release();

        m_width = width;
        m_height = height;

        glGenFramebuffers(1, &m_handle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

        m_format = alpha ? GL_RGBA : GL_RGB;

        glGenTextures(1, &m_texture);
        createTexture(m_texture, m_width, m_height, m_format);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
        GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, drawBuffers);

        if(depth)
        {
            glGenRenderbuffers(1, &m_depth);
            createDepthTexture(m_depth, m_width, m_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);
        }

        return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
    }
}