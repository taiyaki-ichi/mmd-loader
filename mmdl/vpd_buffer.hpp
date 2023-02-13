#pragma once
#include<string>
#include<array>

namespace mmdl
{
	struct vpd_buffer
	{
		std::string name{};
		std::array<float, 3> transform{};
		std::array<float, 4> quaternion{};
	};
}