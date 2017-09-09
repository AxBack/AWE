#include "updater.h"

#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include <cmath>

#define EPSILON 0.0001f

UINT Updater::sThreadCounter = 0;

bool Updater::init(float updateFrequency)
{
	stop();

	return true;
}

void Updater::start()
{
	std::lock_guard<std::mutex> _(m_mutex);
	if(m_running)
		return;

	m_running = true;
	m_workThread = std::thread([=]() { this->run(); });
	LOGI("WorkerThread( Started: %d )", m_id);
}

void Updater::stop()
{
	if(!m_running)
		return;

	{
		std::lock_guard<std::mutex> _(m_mutex);
		m_running = false;
	}

	if(m_workThread.joinable())
	{
		try
		{
			m_workThread.join();
			LOGI("WorkerThread( Ended: %d )", m_id);
		}
		catch(...)
		{
			LOGE("WorkerThread( Failed to join: %d )", m_id);
		}
	}
}

void Updater::run()
{
	using framerate = std::chrono::duration<std::chrono::steady_clock::rep, std::ratio<1, 60>>;
	auto next = std::chrono::steady_clock::now() + framerate(1);
	float dt = 1.0f/60.0f;

	while(true)
	{
		std::this_thread::sleep_until(next);
		next += framerate{1};

		advance(dt);

		std::lock_guard<std::mutex> _(m_mutex);
		if(!m_running)
			break;
	}
}

void Updater::advance(float dt)
{
	if(m_points.size() == 0)
		return;

	for(auto& it : m_connections)
	{
		Vector3 diff = m_points[it.i1].position - m_points[it.i2].position;
		float lengthSq = diff.lengthSq();
		if(lengthSq > it.maxDistanceSq)
		{
			float length = diff.length();
			float over = std::min(length - it.maxDistance, m_maxForce);
			it.tension = m_maxForce / length;

			diff.normalize();
			diff *= over;
			m_points[it.i1].force -= diff;
			m_points[it.i2].force += diff;
		}
		else if(lengthSq < it.minDistanceSq)
		{
			float length = diff.length();
			float under = std::min(it.minDistance - length, m_maxForce);
			it.tension = m_maxForce / length;

			diff.normalize();
			diff *= under;
			m_points[it.i1].force += diff;
			m_points[it.i2].force -= diff;
		} else
			it.tension = 0.0f;
	}

	for(auto& it : m_points)
	{
		it.position += it.force * dt;
		it.force -= it.force * ((1.0f - m_friction) * dt);
	}

	{
		std::lock_guard<std::mutex> _(m_mutex);

		if(m_backupPoints.size() != m_points.size())
			m_backupPoints.resize(m_points.size());
		memcpy(&m_backupPoints[0], &m_points[0], sizeof(Point) * m_points.size());

		if(m_backupConnections.size() != m_connections.size())
			m_backupConnections.resize(m_connections.size());
		memcpy(&m_backupConnections[0], &m_connections[0], sizeof(Connection) * m_backupConnections.size());
	}
}

bool Updater::updateInstances(Mesh<Vertex, Instance>& mesh)
{
	{
		std::lock_guard<std::mutex> _(m_mutex);
		if(m_instances.size() != m_backupPoints.size())
			m_instances.resize(m_backupPoints.size());

		for(int i = 0; i < m_backupPoints.size(); ++i)
		{
			m_instances[i].x = m_backupPoints[i].position.x;
			m_instances[i].y = m_backupPoints[i].position.y;
			m_instances[i].z = m_backupPoints[i].position.z;
			m_instances[i].size = m_backupPoints[i].size;
		}
	}

	mesh.updateInstances(m_instances.size(), &m_instances[0]);

	return true;
}
