#pragma once

#include <vector>
#include "../pch.h"

namespace Engine {

	class IMesh
	{
	public:
		virtual GLuint getIndexBuffer() const = 0;

		virtual GLuint getStaticBuffer() const = 0;

		virtual GLuint getDynamicBuffer() const = 0;
	};

	template<class T, class K>
	class Mesh : public IMesh
	{
	private:

#define NR_BUFFERS 3
#define STATIC_BUFFER_INDEX 0
#define INDEX_BUFFER_INDEX 1
#define DYNAMIC_BUFFER_INDEX 2

		GLuint m_buffers[NR_BUFFERS];

		GLuint m_nrIndices;
		GLuint m_nrInstances;

	public:

		Mesh()
				: m_nrIndices(0), m_nrInstances(0)
		{
		}

		virtual ~Mesh()
		{
			clean();
		}

		bool init(const UINT nrVertices, T* pVertices, const UINT nrIndices, GLushort* pIndices)
		{
			glGenBuffers(NR_BUFFERS, m_buffers);

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[STATIC_BUFFER_INDEX]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(T) * nrVertices, pVertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER_INDEX]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * nrIndices, pIndices,
						 GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			m_nrIndices = nrIndices;

			return true;
		}

		void clean()
		{
			glDeleteBuffers(NR_BUFFERS, m_buffers);
		}

		void updateInstances(const UINT nrInstances, const K* pInstances)
		{
			size_t size = nrInstances * sizeof(K);
			if(size ==  0)
			{
				m_nrInstances = 0;
				return;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[DYNAMIC_BUFFER_INDEX]);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			K* p = static_cast<K*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, size,
													GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
			memcpy(p, pInstances, size);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_nrInstances = nrInstances;
		}

		void render()
		{
			glDrawElementsInstanced(GL_TRIANGLES, m_nrIndices, GL_UNSIGNED_SHORT, nullptr,
									m_nrInstances);
		}

		GLuint getStaticBuffer() const override { return m_buffers[STATIC_BUFFER_INDEX]; }
		GLuint getDynamicBuffer() const override { return m_buffers[DYNAMIC_BUFFER_INDEX]; }
		GLuint getIndexBuffer() const override { return m_buffers[INDEX_BUFFER_INDEX]; }
		bool hasInstances() { return m_nrInstances > 0; }

	};
}