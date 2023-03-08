#pragma once
#include"utility.hpp"
#include<istream>
#include"vmd_loader_traits.hpp"

namespace mmdl
{
	template<typename T, typename traits = vmd_header_traits<T>>
	T load_vmd_header(std::istream& in)
	{
		vmd_header_buffer buffer{};

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

	template<typename T, typename traits = vmd_frame_data_traits<T>>
	T load_vmd_frame_data(std::istream& in, std::size_t size)
	{
		auto result = traits::construct(size);

		vmd_frame_data_buffer buffer{};

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

	template<typename T, typename traits = vmd_morph_data_traits<T>>
	T load_vmd_morph_data(std::istream& in)
	{
		std::uint32_t size{};
		read_from_istream(in, &size);

		auto result = traits::construct(size);

		vmd_morph_data_buffer buffer{};

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