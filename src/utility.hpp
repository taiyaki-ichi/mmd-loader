#pragma once
#include<istream>

namespace mmdl
{
	template<typename T>
	void read_from_istream(std::istream& s, T* out)
	{
		s.read(reinterpret_cast<char*>(out), sizeof(*out));
	}

	template<typename T>
	void read_from_istream(std::istream& s, T* out, std::size_t size)
	{
		s.read(reinterpret_cast<char*>(out), size);
	}

	template<typename T, typename U>
	void read_intanger_from_istream(std::istream& s, T* out, U size)
	{
		switch (size)
		{
		case 1:
			std::uint8_t result8;
			read_from_istream(s, &result8);
			*out = static_cast<T>(result8);
			break;

		case 2:
			std::uint16_t result16;
			read_from_istream(s, &result16);
			*out = static_cast<T>(result16);
			break;

		case 4:
			std::int32_t result32;
			read_from_istream(s, &result32);
			*out = static_cast<T>(result32);
			break;

		case 8:
			std::uint64_t result64;
			read_from_istream(s, &result64);
			*out = static_cast<T>(result64);
			break;
		}
	}
}