#pragma once

#include "../engine/updater.h"
#include "../engine/vector3.h"
#include "point.h"
#include "vertex.h"

namespace Connections {

    class Updater : public Engine::Updater {
    private:

        struct Ray {
            Math::Vector3 start, end;
        };

        typedef std::vector<Point> point_vec;
        typedef std::vector<Connection> connection_vec;
        typedef std::vector<PointInstance> point_instance_vec;
        typedef std::vector<ConnectionInstance> connection_instance_vec;
        typedef std::vector<Ray> event_vec;

        UINT m_id;

        std::mutex m_eventMutex;
        event_vec m_events;

        std::mutex m_pointMutex;

        point_vec m_points;
        point_vec m_backupPoints;

        connection_vec m_connections;
        connection_vec m_backupConnections;

        point_instance_vec m_pointInstances;
        connection_instance_vec m_connectionInstances;

        float m_maxForce;
        float m_friction;

        float m_maxOffset;
        float m_maxOffsetSq;

        void add(Math::Vector3 point, Math::Vector3 color, float size, UINT connectedTo);

        void addConnection(UINT i1, UINT i2, float minDistance, float maxDistance);

    protected:

        virtual void advance(float dt) override;

    public:

        Updater()
                : m_maxForce(500.0f), m_friction(0.05f), m_maxOffset(1000.0f),
                  m_maxOffsetSq(m_maxOffset * m_maxOffset) {
            m_points.push_back({{0, 25, 100}, {1, 0, 0}, 5, 0, {0, 0, 0}});

            add({50, -25, 100}, {0, 1, 0}, 5, 0);
            add({-50, -25, 100}, {0, 0, 1}, 5, 0);

            addConnection(1, 2, 49.0f, 51.0f);
            LOGI("Engine( Created: %d )", m_id);
        }

        bool updateInstances(Engine::Mesh <Vertex, PointInstance> &pointMesh,
                             Engine::Mesh <Vertex, ConnectionInstance> &connectionMesh);

        void rayTest(const Math::Vector3 &start, const Math::Vector3 &end);
    };
}