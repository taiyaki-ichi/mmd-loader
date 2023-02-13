#pragma once
#include"vmd_buffer.hpp"

namespace mmdl
{
	template<typename T>
	struct vmd_header_traits
	{
		static T construct(vmd_header_buffer const&);
	};

	template<typename T>
	struct vmd_frame_data_traits
	{
		static T construct(std::size_t size);

		static void emplace_back(T& frame_data, vmd_frame_data_buffer const&);
	};

}