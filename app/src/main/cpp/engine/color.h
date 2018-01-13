#pragma once

namespace Math {

	struct Color
	{
		float m_data[4];

		float a() { return m_data[0]; }
		float r() { return m_data[1]; }
		float g() { return m_data[2]; }
		float b() { return m_data[3]; }
	};

};