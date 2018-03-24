#pragma once

#include <fstream>
#include <sstream>

namespace Engine { namespace IO {

	class BinaryReader
	{
	private:

		std::istream* m_pStream;
		bool m_swapEndian = false;

	public:

		BinaryReader(std::istream* pStream, bool swapEndian = true)
			: m_pStream(pStream)
			, m_swapEndian(swapEndian)
		{
		}

		virtual ~BinaryReader()
		{
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
		T read() { return read<T>(m_swapEndian); }

		template <typename T>
		T read(bool swapEndian)
		{
			if(!m_pStream->good())
				return T();
			T t;
			m_pStream->read(reinterpret_cast<char*>(&t), sizeof(T));
			if(swapEndian)
				t = swap_endian(t);
			return t;
		}
	};

	class BinaryFileReader : public BinaryReader
	{
		std::ifstream m_file;

	public:
		BinaryFileReader(const char* file, bool swapEndian = true)
			: m_file(file, std::ios::in | std::ios::binary)
			, BinaryReader(&m_file)
		{
		}

		virtual ~BinaryFileReader()
		{
			m_file.close();
		}
	};

	class BinaryStringReader : public BinaryReader
	{
		std::stringstream m_stream;

	public:
		BinaryStringReader(const std::string& string, bool swapEndian = true)
			: m_stream(string, std::ios::in | std::ios::binary)
			, BinaryReader(&m_stream, swapEndian)
		{
		}
	};
}}