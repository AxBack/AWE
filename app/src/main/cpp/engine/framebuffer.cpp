#include "framebuffer.h"
#include "util.h"

namespace Engine {

    bool Framebuffer::init(int width, int height, bool alpha, DepthType depthType)
    {
        GLint formats[] = {alpha ? GL_RGBA : GL_RGB };
		return init(width, height, 1, formats, depthType);
    }

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
			GLuint handle = Util::createTexture(pFormats[i], static_cast<GLenum>(pFormats[i]), GL_UNSIGNED_BYTE,
											   width, height, GL_LINEAR, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, a, GL_TEXTURE_2D, handle, 0);
			m_textures.push_back(handle);

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
				m_depth = Util::createTexture(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, width, height,
										GL_LINEAR, GL_CLAMP_TO_EDGE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
				break;
		}

		glDrawBuffers(static_cast<GLsizei>(attachments.size()), &attachments[0]);

		return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
	}

	void Framebuffer::attachDepthBuffer(GLuint depthBuffer, DepthType type)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
		if (type == WRITE)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		else if(type == READ_WRITE)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
