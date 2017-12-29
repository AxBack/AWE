#include "connection_shader.h"

namespace Connections {

    bool ConnectionShader::init(AAssetManager *pAssetManager, const Engine::IMesh &mesh) {
        std::string file;
        if (!loadFile(pAssetManager, "shaders/ConnectionShader.vs", file))
            return false;

        const char *pFile = file.c_str();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &pFile, nullptr);
        glCompileShader(vs);
        if (!verifyShader(vs))
            return false;

        if (!loadFile(pAssetManager, "shaders/ConnectionShader.ps", file))
            return false;

        pFile = file.c_str();

        GLuint ps = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(ps, 1, &pFile, nullptr);
        glCompileShader(ps);
        if (!verifyShader(ps))
            return false;

        m_program = glCreateProgram();
        glAttachShader(m_program, vs);
        glAttachShader(m_program, ps);
        glLinkProgram(m_program);

        glDetachShader(m_program, vs);
        glDetachShader(m_program, ps);

        glDeleteShader(vs);
        glDeleteShader(ps);

        GLint param = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, &param);
        if (param == GL_FALSE) {
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &param);
            GLchar *pLog = new GLchar[param];
            glGetProgramInfoLog(m_program, param, nullptr, pLog);
            LOGE("init( %s )", pLog);
            SAFE_DELETE_ARRAY(pLog);
            return false;
        }

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