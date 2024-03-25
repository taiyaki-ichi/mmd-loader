#pragma once
#include"buffer.hpp"

namespace mmdl::vmd
{
	template<typename T>
	struct header_traits
	{
		static T construct(header_buffer const&);
	};

	template<typename T>
	struct frame_data_traits
	{
		static T construct(std::size_t size);

		static void emplace_back(T& frame_data, frame_buffer const&);
	};

	template<typename T>
	struct morph_data_traits
	{
		static T construct(std::size_t size);

		static void emplace_back(T&, morph_buffer&);
	};
}