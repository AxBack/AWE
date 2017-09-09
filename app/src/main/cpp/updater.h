#pragma once

#include "pch.h"
#include "vector3.h"
#include "Point.h"
#include "vertex.h"
#include "mesh.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class Updater
{
private:
	static UINT sThreadCounter;

	typedef std::vector<Point> point_vec;
	typedef std::vector<Connection> connection_vec;
	typedef std::vector<Instance> instance_vec;

	UINT			m_id;

	std::mutex		m_mutex;
	bool 			m_running;
	std::thread		m_workThread;

	point_vec		m_points;
	point_vec		m_backupPoints;

	connection_vec	m_connections;

	instance_vec 	m_instances;

	float 			m_maxForce;
	float 			m_friction;

	void start();
	void stop();

	void run();

	void advance(float dt);

public:

	Updater()
	: m_maxForce(500.0f)
	, m_friction(0.05f)
	{
		m_id = sThreadCounter;
		++sThreadCounter;
		m_points.push_back({{0,500,1}, 50, 0, {0,0,0}});
		m_points.push_back({{500,-500,1}, 50, 0, {0,0,0}});
		m_points.push_back({{-500,-500,1}, 50, 0, {0,0,0}});
		addConnection(0,1, 450, 550);
		addConnection(1,2, 450, 550);
		addConnection(0,2, 450, 550);
		LOGI("Engine( Created: %d )", m_id);
	}

	void addConnection(UINT i1, UINT i2, float minDistance, float maxDistance)
	{
		m_connections.push_back({i1, i2, minDistance, maxDistance,
								 minDistance * minDistance, maxDistance * maxDistance});
	}

	virtual ~Updater()
	{
		stop();
	}

	bool init(float updateFrequency);

	void resume()
	{
		start();
	}

	void pause()
	{
		stop();
	}

	bool updateInstances(Mesh<Vertex, Instance>& mesh);
};
