#pragma once

#include <fstream>

namespace Engine {

	class BinaryReader
	{
	private:

		std::ifstream m_file;

	public:

		BinaryReader(const char* file, bool swapEndian = true)
			:m_file(file, std::ios::in | std::ios::binary)
		{
		}

		virtual ~BinaryReader()
		{
			m_file.close();
		}

		template<typename T>
		T swap_endian(T u)
		{
			static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

			union
			{
				T u;
				unsigned char u8[sizeof(T)];
			} source, dest;

			source.u = u;

			for(size_t k=0; k < sizeof(T); ++k)
				dest.u8[k] = source.u8[sizeof(T) - k - 1 ];

			return dest.u;
		};

		template <typename T>
		T read(bool swapEndian = true)
		{
			if(!m_file.good())
				return T();
			T t;
			m_file.read(reinterpret_cast<char*>(&t), sizeof(T));
			if(swapEndian)
				t = swap_endian(t);
			return t;
		}

	};

};