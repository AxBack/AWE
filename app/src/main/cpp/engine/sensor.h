#pragma once

#include <android/sensor.h>
#include "vector3.h"

namespace Engine {

	class Sensor
	{
	private:

		Math::Quaternion m_rotation;

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
		}

		void pause()
		{
			ASensorEventQueue_disableSensor(m_pEventQueue, m_pAccelerometer);
		}

		void update()
		{
			ALooper_pollAll(0, NULL, NULL, NULL);
			ASensorEvent event;
			while(ASensorEventQueue_getEvents(m_pEventQueue, &event, 1) > 0)
			{
				m_rotation = Math::Quaternion::fromEulerAngles(event.vector.x, event.vector.y, event.vector.z);
			}
		}

		Math::Quaternion getRotation() const { return m_rotation; }

	};
};
