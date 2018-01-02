#include "particle_shader.h"
#include "vertex.h"

namespace Electric {

    bool ParticleShader::init(AAssetManager* pAssetManager, const Engine::IMesh& mesh) {

        m_program = createProgram(pAssetManager, "shaders/ParticleShader.vs", "shaders/Simple.ps");
        m_viewProjectionLocation = glGetUniformLocation(m_program, VIEW_PROJECTION);

        bindTo(mesh);

        return true;
    }

    void ParticleShader::bindTo(const Engine::IMesh &mesh) {
        if (m_vao <= 0)
            glGenVertexArrays(1, &m_vao);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), 0);
        glVertexAttribDivisor(1, 1);

        glBindVertexArray(0);
    }
}
