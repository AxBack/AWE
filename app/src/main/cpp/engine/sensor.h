#pragma once

#include <android/sensor.h>
#include "vector3.h"

namespace Engine {

	class Sensor
	{
	private:

		typedef std::chrono::steady_clock::time_point time_point;

		time_point 		m_lastRenderTime;

		Math::Vector3 m_rotation;

		ASensorManager* m_pSensorManager;
		const ASensor* m_pAccelerometer;
		ASensorEventQueue* m_pEventQueue;
		ALooper* m_pLooper;

	public:

		Sensor()
		{

		}

		virtual ~Sensor()
		{
			pause();
		}

		bool init(int id)
		{
			m_pSensorManager = ASensorManager_getInstance();
			m_pAccelerometer = ASensorManager_getDefaultSensor(m_pSensorManager, ASENSOR_TYPE_GYROSCOPE);
			m_pLooper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
			m_pEventQueue = ASensorManager_createEventQueue(m_pSensorManager, m_pLooper, id, NULL, NULL);
			resume();
			return true;
		}

		void resume()
		{
			ASensorEventQueue_enableSensor(m_pEventQueue, m_pAccelerometer);
			ASensorEventQueue_setEventRate(m_pEventQueue, m_pAccelerometer, 100);
			m_lastRenderTime = std::chrono::steady_clock::now();
		}

		void pause()
		{
			ASensorEventQueue_disableSensor(m_pEventQueue, m_pAccelerometer);
		}

		void update()
		{
			time_point now = std::chrono::steady_clock::now();
			std::chrono::duration<float> secs = now - m_lastRenderTime;
			m_lastRenderTime = now;
			float dt = secs.count();

			ALooper_pollAll(0, NULL, NULL, NULL);
			ASensorEvent event;
			while(ASensorEventQueue_getEvents(m_pEventQueue, &event, 1) > 0)
			{
				m_rotation += Math::Vector3{
						TO_DEGREES(event.vector.x),
						TO_DEGREES(event.vector.y),
						TO_DEGREES(event.vector.z)
				} * dt;
			};
		}

		Math::Vector3 getRotation() const { return m_rotation; }

	};
};
