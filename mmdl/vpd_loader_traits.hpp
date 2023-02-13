#pragma once
#include"vpd_buffer.hpp"

namespace mmdl
{
	template<typename T>
	struct vpd_data_traits
	{
		static T construct(std::size_t size);

		static void emplace_back(T& vpd_data, vpd_buffer const&);
	};
}