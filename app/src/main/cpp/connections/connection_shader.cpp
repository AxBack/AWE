#include "connection_shader.h"

namespace Connections {

    bool ConnectionShader::init(AAssetManager *pAssetManager, const Engine::IMesh &mesh) {

        m_program = createProgram(pAssetManager, "shaders/ConnectionShader_vs.glsl", "shaders/Simple_ps.glsl");
        m_viewProjectionLocation = glGetUniformLocation(m_program, VIEW_PROJECTION);

        bindTo(mesh);

        return true;
    }

    void ConnectionShader::bindTo(const Engine::IMesh &mesh) {
        if (m_vao <= 0)
            glGenVertexArrays(1, &m_vao);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(3));

        glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ConnectionInstance), 0);
        glVertexAttribDivisor(2, 1);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ConnectionInstance),
                              BUFFER_OFFSET(3));
        glVertexAttribDivisor(3, 1);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(ConnectionInstance),
                              BUFFER_OFFSET(6));
        glVertexAttribDivisor(4, 1);

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(ConnectionInstance),
                              BUFFER_OFFSET(7));
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(ConnectionInstance),
                              BUFFER_OFFSET(10));
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}