#pragma once

#include "../engine/engine.h"
#include "point_shader.h"
#include "connection_shader.h"
#include "vertex.h"
#include "connection_shader.h"
#include "point_shader.h"
#include "updater.h"
#include "../engine/mesh.h"

namespace Connections {

    typedef Engine::Mesh <Vertex, PointInstance> PointMesh;
    typedef Engine::Mesh <Vertex, ConnectionInstance> ConnectionMesh;

    class ConnectionsEngine : public Engine::Engine {
    private:

        Updater m_updater;

        PointShader m_pointShader;
        PointMesh m_pointMesh;

        ConnectionShader m_connectionShader;
        ConnectionMesh m_connectionMesh;

    public:

        virtual void clear() override;

        virtual bool init(AAssetManager *pAssetManager) override;

        virtual bool render() override;

        virtual void resume() override { m_updater.resume(); }

        virtual void pause() override { m_updater.pause(); }

        virtual void updateSize(int width, int height) override;

        virtual void touch(float x, float y) override;

    };
}