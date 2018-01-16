#pragma once

#include "../pch.h"
#include <mutex>

namespace Engine {

	class Framebuffer
	{
	private:

		GLsizei m_width;
		GLsizei m_height;
		GLenum	m_format;

		GLuint m_handle;
		GLuint m_texture;
		GLuint m_depth;

		static void createTexture(GLuint texture, GLsizei width, GLsizei height, GLenum format)
		{
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,format, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		static void createDepthTexture(GLuint texture, GLsizei width, GLsizei height)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, texture);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		}

	public:

		Framebuffer()
			: m_width(0)
			, m_height(0)
			, m_handle(0)
			, m_texture(0)
			, m_depth(0)
		{
		}

		virtual ~Framebuffer()
		{
			release();
		}

		void release()
		{
			if(m_texture > 0)
				glDeleteTextures(1, &m_texture);

			if(m_depth > 0)
				glDeleteTextures(1, &m_texture);

			if(m_handle > 0)
				glDeleteFramebuffers(1, &m_handle);
		}

		bool init(int width, int height, bool alpha, bool depth)
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

		void set()
		{
			if(m_handle > 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
				glViewport(0, 0, m_width, m_height);
			}
		}

		void bind()
		{
			if(m_texture > 0)
				glBindTexture(GL_TEXTURE_2D, m_texture);
		}
	};
};