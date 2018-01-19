#include "particle_shader.h"
#include "vertex.h"

namespace Electric {

#define VIEW_PROJECTION "viewProjection"
#define UP "up"
#define RIGHT "right"

    bool ParticleShader::init(AAssetManager* pAssetManager, const Mesh& mesh) {

        m_program = createProgram(pAssetManager, "shaders/ParticleShader_vs.glsl", "shaders/SimpleShader_ps.glsl");
        m_viewProjectionLocation = getLocation(VIEW_PROJECTION);
        m_upLocation = getLocation(UP);
        m_rightLocation = getLocation(RIGHT);

        bindTo(mesh);

        return true;
    }

    void ParticleShader::bindTo(const Mesh &mesh) {
        if (m_vao <= 0)
            glGenVertexArrays(1, &m_vao);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionVertex), 0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), 0);
        glVertexAttribDivisor(1, 1);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), BUFFER_OFFSET(3));
        glVertexAttribDivisor(2, 1);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), BUFFER_OFFSET(4));
        glVertexAttribDivisor(3, 1);

        glBindVertexArray(0);
    }
}
