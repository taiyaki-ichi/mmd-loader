#pragma once
#include"buffer.hpp"

namespace mmdl::vpd
{
	template<typename T>
	struct vpd_data_traits
	{
		static T construct(std::size_t size);

		static void emplace_back(T& vpd_data, buffer const&);
	};
}