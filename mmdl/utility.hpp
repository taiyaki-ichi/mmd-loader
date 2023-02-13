#pragma once
#include<istream>
#include<Windows.h>

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

	template<typename Traits,typename T, typename U>
	void read_array_from_istream(std::istream& in, T* out, U size, U stride)
	{
		for (U i = 0; i < size; i++)
		{
			read_from_istream(in, &Traits::get_reference(*out, i), stride);
		}
	}

	// utf16����utf8�ւ̕ϊ�
	template<typename DstStr, typename SrcStr>
	DstStr utf16_to_utf8(SrcStr const& src)
	{
		// �ϊ���̑傫���̎擾
		auto dst_size = WideCharToMultiByte(CP_UTF8, 0, src.data(), -1, nullptr, 0, nullptr, nullptr);

		// �I�_������ǉ����Ȃ�
		dst_size--;

		// �T�C�Y�̕ύX
		DstStr result;
		result.resize(dst_size);

		// �ϊ�
		WideCharToMultiByte(CP_UTF8, 0, src.data(), -1, result.data(), dst_size, nullptr, nullptr);

		return result;
	}

	template<typename DstStr, typename SrcStr>
	DstStr utf8_to_utf16(SrcStr const& src)
	{
		// �ԊҌ�̑傫���̎擾
		auto dst_size = MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, nullptr, 0);

		// �I�_������ǉ����Ȃ�
		dst_size--;

		// �T�C�Y�̕ύX
		DstStr result;
		result.resize(dst_size);

		// �ϊ�
		MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, result.data(), dst_size);

		return result;
	}

	template<typename DstStr, typename SrcStr>
	DstStr ansi_to_utf16(SrcStr const& src)
	{
		// �ԊҌ�̑傫���̎擾
		auto dst_size = MultiByteToWideChar(CP_ACP, 0, src.data(), -1, nullptr, 0);

		// �I�_������ǉ����Ȃ�
		dst_size--;

		// �T�C�Y�̕ύX
		DstStr result;
		result.resize(dst_size);

		// �ϊ�
		MultiByteToWideChar(CP_ACP, 0, src.data(), -1, result.data(), dst_size);

		return result;
	}
}