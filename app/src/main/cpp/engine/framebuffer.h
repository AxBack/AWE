#pragma once

#include "../pch.h"

namespace Engine {

	class Framebuffer
	{
	private:

		GLsizei m_width;
		GLsizei m_height;

		GLuint m_handle;
		GLuint m_texture;
		GLuint m_depth;

	public:

		Framebuffer()
			: m_width(0)
			, m_height(0)
			, m_handle(0)
			, m_texture(0)
			, m_depth(0)
		{
		}

		bool init(int width, int height, bool alpha, bool depth)
		{
			m_width = width;
			m_height = height;

			glGenFramebuffers(1, &m_handle);
			glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

			GLenum format = alpha ? GL_RGBA : GL_RGB;

			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
			GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
			glDrawBuffers(1, drawBuffers);

			if(depth)
			{
				glGenRenderbuffers(1, &m_depth);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);
			}

			return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
		}

		void set()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
			glViewport(0,0, m_width, m_height);
		}

		void bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}
	};
};