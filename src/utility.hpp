#pragma once
#include<fstream>

namespace mmdl
{
	template<typename T>
	void read_from_istream(std::ifstream& s, T* out, std::size_t size = sizeof(out))
	{
		s.read(reinterpret_cast<char*>(out), size);
	}
}