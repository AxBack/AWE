#pragma once

#include "../pch.h"
#include <mutex>
#include <vector>

namespace Engine {

	class Framebuffer
	{
	public:

		enum DepthType { NONE, WRITE, READ_WRITE };

	private:

		DepthType m_depthType;

		GLsizei m_width;
		GLsizei m_height;

		GLuint m_handle;
		GLuint m_depth;

		std::vector<GLuint> m_textures;

		static GLuint createTexture(GLint internalFormat, GLuint format, GLuint type,
									GLsizei width, GLsizei height, GLenum filter,
									GLenum wrap, GLenum attachment)
		{
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
			return texture;
		}

	public:

		Framebuffer()
			: m_width(0)
			, m_height(0)
			, m_handle(0)
			, m_depth(0)
			, m_depthType(NONE)
		{
		}

		virtual ~Framebuffer()
		{
			release();
		}

		void release()
		{
			if(m_textures.size() > 0)
			{
				glDeleteTextures(static_cast<GLsizei>(m_textures.size()), &m_textures[0]);
				m_textures.clear();
			}

			if(m_depth > 0)
			{
				glDeleteTextures(1, &m_depth);
				m_depth = 0;
			}

			if(m_handle > 0)
			{
				glDeleteFramebuffers(1, &m_handle);
				m_handle = 0;
			}
		}

		bool init(int width, int height, bool alpha, DepthType depthType);
		bool init(int width, int height, UINT nrAttachments, GLint* pFormats, DepthType depthType);

		void set() const
		{
			if(m_handle > 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
				glViewport(0, 0, m_width, m_height);
			}
		}

		void clear()
		{
			if(m_depthType != NONE)
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			else
				glClear(GL_COLOR_BUFFER_BIT);
		}

		void bind(UINT index = 0) const
		{
			if(index < m_textures.size())
				glBindTexture(GL_TEXTURE_2D, m_textures[index]);
		}

		void bindDepth() const
		{
			if(m_depthType == READ_WRITE)
				glBindTexture(GL_TEXTURE_2D, m_depth);
		}

		GLsizei getWidth() const { return m_width; }
		GLsizei getHeight() const { return m_height; }
	};
};