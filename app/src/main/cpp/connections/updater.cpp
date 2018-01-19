#include "updater.h"

namespace Connections {

    void Updater::advance(float dt) {
        if (m_points.size() == 0)
            return;

        event_vec events;
        {
            std::lock_guard<std::mutex> _(m_eventMutex);
            events.swap(m_events);
        }

        for (auto &it : events) {

        }

        for (auto &it : m_connections) {
            Math::Vector3 diff = m_points[it.i1].position - m_points[it.i2].position;
            float lengthSq = diff.lengthSq();
            if (lengthSq > it.maxDistanceSq) {
                float length = diff.length();
                float over = std::min(length - it.maxDistance, m_maxForce);
                it.tension = -(over / m_maxForce);

                diff.normalize();
                diff *= over;
                m_points[it.i1].force -= diff;
                m_points[it.i2].force += diff;
            } else if (lengthSq < it.minDistanceSq) {
                float length = diff.length();
                float under = std::min(it.minDistance - length, m_maxForce);
                it.tension = (under / m_maxForce);

                diff.normalize();
                diff *= under;
                m_points[it.i1].force += diff;
                m_points[it.i2].force -= diff;
            } else
                it.tension = 0.0f;
        }

        for (auto &it : m_points) {
            if (it.position.lengthSq() > m_maxOffsetSq) {
                float length = it.position.length();
                float over = std::min(length - m_maxOffset, m_maxForce);
                Math::Vector3 diff = it.position;
                diff.normalize();
                it.force -= diff * over;
            }

            it.position += it.force * dt;
            it.force -= it.force * ((1.0f - m_friction) * dt);
        }

        {
            std::lock_guard<std::mutex> _(m_pointMutex);

            if (m_backupPoints.size() != m_points.size())
                m_backupPoints.resize(m_points.size());
            memcpy(&m_backupPoints[0], &m_points[0], sizeof(Point) * m_points.size());

            if (m_backupConnections.size() != m_connections.size())
                m_backupConnections.resize(m_connections.size());
            memcpy(&m_backupConnections[0], &m_connections[0],
                   sizeof(Connection) * m_backupConnections.size());
        }
    }


    bool Updater::updateInstances(Engine::InstancedMesh<Vertex, PointInstance> &pointMesh,
                                  Engine::InstancedMesh <Vertex, ConnectionInstance> &connectionMesh) {
        {
            std::lock_guard<std::mutex> _(m_pointMutex);

            if (m_pointInstances.size() != m_backupPoints.size())
                m_pointInstances.resize(m_backupPoints.size());

            for (int i = 0; i < m_backupPoints.size(); ++i) {
                m_pointInstances[i].x = m_backupPoints[i].position.x();
                m_pointInstances[i].y = m_backupPoints[i].position.y();
                m_pointInstances[i].z = m_backupPoints[i].position.z();
                m_pointInstances[i].size = m_backupPoints[i].size;

                m_pointInstances[i].r = m_backupPoints[i].color.x();
                m_pointInstances[i].g = m_backupPoints[i].color.y();
                m_pointInstances[i].b = m_backupPoints[i].color.z();
            }

            if (m_connectionInstances.size() != m_backupConnections.size())
                m_connectionInstances.resize(m_backupConnections.size());

            for (int i = 0; i < m_backupConnections.size(); ++i) {
                const Point &p1 = m_backupPoints[m_backupConnections[i].i1];
                const Point &p2 = m_backupPoints[m_backupConnections[i].i2];

                m_connectionInstances[i].startx = p1.position.x();
                m_connectionInstances[i].starty = p1.position.y();
                m_connectionInstances[i].startz = p1.position.z();

                m_connectionInstances[i].endx = p2.position.x();
                m_connectionInstances[i].endy = p2.position.y();
                m_connectionInstances[i].endz = p2.position.z();

                m_connectionInstances[i].tension = m_backupConnections[i].tension;

                m_connectionInstances[i].startr = p1.color.x();
                m_connectionInstances[i].startg = p1.color.y();
                m_connectionInstances[i].startb = p1.color.z();

                m_connectionInstances[i].endr = p2.color.x();
                m_connectionInstances[i].endg = p2.color.y();
                m_connectionInstances[i].endb = p2.color.z();
            }
        }

        pointMesh.updateInstances(m_pointInstances.size(), &m_pointInstances[0]);
        connectionMesh.updateInstances(m_connectionInstances.size(), &m_connectionInstances[0]);

        return true;
    }

    void Updater::add(Math::Vector3 point, Math::Vector3 color, float size, UINT connectedTo) {
        m_points.push_back({point, color, size, 0, {0, 0, 0}});
        Math::Vector3 diff = point - m_points[connectedTo].position;
        float length = diff.length();

        addConnection(connectedTo, m_points.size() - 1, length - 10.0f, length + 10.0f);
    }

    void Updater::addConnection(UINT i1, UINT i2, float minDistance, float maxDistance) {
        m_connections.push_back({i1, i2, minDistance, maxDistance,
                                 minDistance * minDistance, maxDistance * maxDistance,
                                 0.0f});
    }

    void Updater::rayTest(const Math::Vector3 &start, const Math::Vector3 &end) {
        std::lock_guard<std::mutex> _(m_eventMutex);
        m_events.push_back({start, end});
    }
}
