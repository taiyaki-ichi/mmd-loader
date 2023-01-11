#pragma once
#include<istream>
#include<Windows.h>
#include"generics_type.hpp"

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

	template<typename T, typename U>
		requires resizable_container<T, U>
	void read_array_from_istream(std::istream& in, T* out, U size, U stride)
	{
		for (U i = 0; i < size; i++)
		{
			read_from_istream(in, &resizable_container_traits<T, U>::get(*out, i), stride);
		}
	}

	template<constructible_vec2 T>
	void read_vec2_from_istream(std::istream& in, T* out)
	{
		float x, y;
		read_from_istream(in, &x);
		read_from_istream(in, &y);
		*out = constructible_vec2_traits<T>::construct(x, y);
	}

	template<constructible_vec3 T>
	void read_vec3_from_istream(std::istream& in, T* out)
	{
		float x, y, z;
		read_from_istream(in, &x);
		read_from_istream(in, &y);
		read_from_istream(in, &z);
		*out = constructible_vec3_traits<T>::construct(x, y, z);
	}

	template<constructible_vec4 T>
	void read_vec4_from_istream(std::istream& in, T* out)
	{
		float x, y, z, w;
		read_from_istream(in, &x);
		read_from_istream(in, &y);
		read_from_istream(in, &z);
		read_from_istream(in, &w);
		*out = constructible_vec4_traits<T>::construct(x, y, z, w);
	}

	// utf16からutf8への変換
	template<typename DstStr, typename SrcStr>
	DstStr utf16_to_utf8(SrcStr const& src)
	{
		// 変換後の大きさの取得
		auto dst_size = WideCharToMultiByte(CP_UTF8, 0, src.data(), -1, nullptr, 0, nullptr, nullptr);

		// サイズの変更
		DstStr result;
		result.resize(dst_size);

		// 変換
		WideCharToMultiByte(CP_UTF8, 0, src.data(), -1, result.data(), dst_size, nullptr, nullptr);

		return result;
	}

	template<typename DstStr, typename SrcStr>
	DstStr utf8_to_utf16(SrcStr const& src)
	{
		// 返還後の大きさの取得
		auto dst_size = MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, nullptr, 0);

		// サイズの変更
		DstStr result;
		result.resize(dst_size);

		// 変換
		MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, result.data(), dst_size);

		return result;
	}

	template<typename DstStr, typename SrcStr>
	DstStr ansi_to_utf16(SrcStr const& src)
	{
		// 返還後の大きさの取得
		auto dst_size = MultiByteToWideChar(CP_ACP, 0, src.data(), -1, nullptr, 0);

		// サイズの変更
		DstStr result;
		result.resize(dst_size);

		// 変換
		MultiByteToWideChar(CP_ACP, 0, src.data(), -1, result.data(), dst_size);

		return result;
	}
}