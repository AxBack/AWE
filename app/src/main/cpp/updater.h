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
	connection_vec	m_backupConnections;

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

		m_points.push_back({{0,250,1}, 50, 0, {0,0,0}});

		add({500,-250,1}, 50, 0);
		add({-500,-250,1}, 50, 0);

		addConnection(1,2, 490.0f, 510.0f);
		LOGI("Engine( Created: %d )", m_id);
	}

	void add(Vector3 point, float size, UINT connectedTo)
	{
		m_points.push_back({point, size, 0, {0,0,0}});
		Vector3 diff = point - m_points[connectedTo].position;
		float length= diff.length();

		addConnection(connectedTo, m_points.size()-1, length-10.0f, length + 10.0f);
	}

	void addConnection(UINT i1, UINT i2, float minDistance, float maxDistance)
	{
		m_connections.push_back({i1, i2, minDistance, maxDistance,
								 minDistance * minDistance, maxDistance * maxDistance,
								 0.0f});
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
