#pragma once

#include "pch.h"
#include "vector3.h"
#include "point.h"
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
	typedef std::vector<PointInstance> point_instance_vec;
	typedef std::vector<ConnectionInstance> connection_instance_vec;

	UINT			m_id;

	std::mutex		m_mutex;
	bool 			m_running;
	std::thread		m_workThread;

	point_vec		m_points;
	point_vec		m_backupPoints;

	connection_vec	m_connections;
	connection_vec	m_backupConnections;

	point_instance_vec 	m_pointInstances;
	connection_instance_vec m_connectionInstances;


	float 			m_maxForce;
	float 			m_friction;

	void start();
	void stop();

	void run();

	void advance(float dt);

	void add(Vector3 point, float size, UINT connectedTo);
	void addConnection(UINT i1, UINT i2, float minDistance, float maxDistance);

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

	bool updateInstances(Mesh<Vertex, PointInstance>& pointMesh,
						 Mesh<Vertex, ConnectionInstance>& connectionMesh);
};
