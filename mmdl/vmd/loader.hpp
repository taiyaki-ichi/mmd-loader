#pragma once
#include"../utility/io_helper.hpp"
#include<istream>
#include"loader_traits.hpp"

namespace mmdl::vmd
{
	template<typename T, typename traits = header_traits<T>>
	T load_header(std::istream& in)
	{
		header_buffer buffer{};

		// マジックナンバー
		char magic[30];
		in.read(magic, sizeof(magic));

		// 名前
		read_from_istream(in, &buffer.name);

		// フレームデータ数
		// TODO: load_vmd_frame_dataで読み込むようにする
		read_from_istream(in, &buffer.frame_data_num);

		return traits::construct(buffer);
	}

	template<typename T, typename traits = frame_data_traits<T>>
	T load_frame_data(std::istream& in, std::size_t size)
	{
		auto result = traits::construct(size);

		frame_buffer buffer{};

		for (std::size_t i = 0; i < size; i++)
		{

			// ボーン名
			read_from_istream(in, &buffer.name);

			// フレーム番号
			read_from_istream(in, &buffer.frame_num);

			// 移動量
			read_from_istream(in, &buffer.transform);

			// クオータニオン
			read_from_istream(in, &buffer.quaternion);

			// 補完パラメータ
			read_from_istream(in, &buffer.complement_parameter);

			// 要素を追加
			traits::emplace_back(result, buffer);
		}

		return result;
	}

	template<typename T, typename traits = morph_data_traits<T>>
	T load_morph_data(std::istream& in)
	{
		std::uint32_t size{};
		read_from_istream(in, &size);

		auto result = traits::construct(size);

		morph_buffer buffer{};

		for (std::size_t i = 0; i < size; i++)
		{
			// モーフ名
			read_from_istream(in, &buffer.name);

			// フレーム番号
			read_from_istream(in, &buffer.frame_num);

			// 重み
			read_from_istream(in, &buffer.weight);

			traits::emplace_back(result, buffer);
		}

		return result;
	}
}