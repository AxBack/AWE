#include "framebuffer.h"

namespace Engine {

    bool Framebuffer::init(int width, int height, bool alpha, DepthType depthType)
    {
        GLint formats[] = {alpha ? GL_RGBA : GL_RGB };
		return init(width, height, 1, formats, depthType);
    };

	bool Framebuffer::init(int width, int height, UINT nrAttachments, GLint* pFormats,
						   DepthType depthType)
	{
		release();

		m_width = width;
		m_height = height;
		m_depthType = depthType;

		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		std::vector<GLenum> attachments;
		for(UINT i=0; i<nrAttachments; ++i)
		{
			GLenum a = GL_COLOR_ATTACHMENT0+i;
			attachments.push_back(a);
			m_textures.push_back(createTexture(pFormats[i], static_cast<GLenum>(pFormats[i]), GL_UNSIGNED_BYTE,
											   width, height, GL_LINEAR, GL_CLAMP_TO_EDGE, a));
		}

		switch(m_depthType)
		{
			default:
			case NONE:
				break;
			case WRITE:
				glGenRenderbuffers(1, &m_depth);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);
				break;
			case READ_WRITE:
				m_depth = createTexture(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, width, height,
										GL_LINEAR, GL_CLAMP_TO_EDGE, GL_DEPTH_ATTACHMENT);
				break;
		}

		glDrawBuffers(static_cast<GLsizei>(attachments.size()), &attachments[0]);

		return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
	}
}
