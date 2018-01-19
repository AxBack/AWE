#pragma once

#include <vector>
#include "../pch.h"

namespace Engine {

	template<class T>
	class Mesh
	{
	protected:

		GLuint m_vertexBuffer;
		GLuint m_indexBuffer;

		GLuint m_nrIndices;

	public:

		Mesh()
			: m_nrIndices(0)
		{
		}

		virtual ~Mesh()
		{
			clean();
		}

		virtual bool init(const UINT nrVertices, T* pVertices, const UINT nrIndices, GLushort* pIndices)
		{
			GLuint buffers[2];
			glGenBuffers(2, buffers);
			m_vertexBuffer = buffers[0];
			m_indexBuffer = buffers[1];

			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(T) * nrVertices, pVertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * nrIndices, pIndices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			m_nrIndices = nrIndices;

			return true;
		}

		virtual void clean()
		{
			GLuint buffers[] = {m_vertexBuffer, m_indexBuffer };
			glDeleteBuffers(2, buffers);
		}

		virtual void render() const
		{
			glDrawElements(GL_TRIANGLES, m_nrIndices, GL_UNSIGNED_SHORT, nullptr);
		}

		GLuint getStaticBuffer() const { return m_vertexBuffer; }
		GLuint getIndexBuffer() const { return m_indexBuffer; }

	};

	template<class T, class K>
	class InstancedMesh : public Mesh<T>
	{
	private:

		GLuint m_instanceBuffer;
		GLuint m_nrInstances;

	public:

		InstancedMesh()
				: Mesh<T>()
				, m_nrInstances(0)
		{
		}

		virtual bool init(const UINT nrVertices, T* pVertices, const UINT nrIndices, GLushort* pIndices) override
		{
			glGenBuffers(1, &m_instanceBuffer);
			return Mesh<T>::init(nrVertices, pVertices, nrIndices, pIndices);
		}

		virtual void clean() override
		{
			glDeleteBuffers(1, &m_instanceBuffer);
			Mesh<T>::clean();
		}

		void updateInstances(const UINT nrInstances, const K* pInstances)
		{
			size_t size = nrInstances * sizeof(K);
			if(size ==  0)
			{
				m_nrInstances = 0;
				return;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			K* p = static_cast<K*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, size,
													GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
			memcpy(p, pInstances, size);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_nrInstances = nrInstances;
		}

		virtual void render() const override
		{
			glDrawElementsInstanced(GL_TRIANGLES, Mesh<T>::m_nrIndices, GL_UNSIGNED_SHORT, nullptr, m_nrInstances);
		}

		bool hasInstances() { return m_nrInstances > 0; }
		GLuint getDynamicBuffer() const { return m_instanceBuffer; }
	};

};