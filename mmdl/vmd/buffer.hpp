#pragma once
#include<array>

namespace mmdl::vmd
{
	struct header_buffer
	{
		// 名前
		// サイズは固定
		std::array<char, 20> name{};

		// フレームデータ数
		std::uint32_t frame_data_num{};
	};

	struct frame_buffer
	{
		// 名前
		// サイズは固定
		std::array<char, 15> name{};

		// フレーム番号
		std::uint32_t frame_num{};

		// 位置
		std::array<float, 3> transform{};

		// クオータニオン
		std::array<float, 4> quaternion{};

		// 補完パラメータ
		std::array<char, 64> complement_parameter{};
	};

	struct morph_buffer
	{
		// 名前
		// サイズは固定
		std::array<char, 15> name{};

		// フレーム番号
		std::uint32_t frame_num{};

		// 表情スライダーの値
		float weight{};
	};
}