#pragma once

#include <string>
#include <android/asset_manager.h>
#include "../pch.h"
#include <sstream>
#include <exception>
#include <stdexcept>

namespace Engine {

    class Shader {
    protected:

        GLuint m_program;
        GLuint m_vao;

    protected:

        static bool verifyShader(GLuint shader) {
            GLint param = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
            if (param == GL_FALSE) {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &param);
                GLchar *pLog = new GLchar[param];
                glGetShaderInfoLog(shader, param, nullptr, pLog);
                LOGE("verifyShader( %s )", pLog);
                SAFE_DELETE_ARRAY(pLog);
            }
            return true;
        }

        static bool loadFile(AAssetManager *pAssetManager, const std::string &asset, std::string &outFile) {
            AAsset *pAsset = AAssetManager_open(pAssetManager, asset.c_str(),
                                                AASSET_MODE_STREAMING);
            if (pAsset == nullptr) {
                LOGE("loadFile( Could not get asset: %s )", asset.c_str());
                return false;
            }

            outFile.clear();

            static const size_t size = 1024;
            int r = 0;
            char *pBuffer = new char[size];
            while ((r = AAsset_read(pAsset, pBuffer, size)) > 0) {
                outFile.insert(outFile.end(), pBuffer, pBuffer + r);
            }
            AAsset_close(pAsset);

            SAFE_DELETE_ARRAY(pBuffer);
            return true;
        }

        static GLuint createProgram(AAssetManager *pAssetManager, const char* vsFile, const char* psFile)        {
            std::string file;
            if (!loadFile(pAssetManager, vsFile, file))
                return GL_FALSE;

            const char *pFile = file.c_str();

            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &pFile, nullptr);
            glCompileShader(vs);
            if (!verifyShader(vs))
                return GL_FALSE;

            if (!loadFile(pAssetManager, psFile, file))
                return GL_FALSE;

            pFile = file.c_str();

            GLuint ps = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(ps, 1, &pFile, nullptr);
            glCompileShader(ps);
            if (!verifyShader(ps))
                return GL_FALSE;

            GLuint program = glCreateProgram();
            glAttachShader(program, vs);
            glAttachShader(program, ps);
            glLinkProgram(program);

            glDetachShader(program, vs);
            glDetachShader(program, ps);

            glDeleteShader(vs);
            glDeleteShader(ps);

            GLint param = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &param);
            if (param == GL_FALSE) {
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &param);
                GLchar *pLog = new GLchar[param];
                glGetProgramInfoLog(program, param, nullptr, pLog);
                LOGE("init( %s )", pLog);
                SAFE_DELETE_ARRAY(pLog);
                return GL_FALSE;
            }

            return program;
        }

        GLint getLocation(const char* uniform)
        {
            GLint location = glGetUniformLocation(m_program, uniform);
            if(location < 0)
            {
                std::stringstream ss;
                ss << uniform << " not found in shader";
                throw std::invalid_argument(ss.str().c_str());
            }
            return location;
        }

    public:

        Shader()
            : m_program(0)
            , m_vao(0)
        {
        }

        virtual ~Shader() {
            release();
        }

        void release() {
            if (m_program > 0) {
                glDeleteProgram(m_program);
                m_program = 0;
            }

            if (m_vao > 0) {
                glDeleteVertexArrays(1, &m_vao);
                m_vao = 0;
            }
        }
    };
}
