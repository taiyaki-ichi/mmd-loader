#pragma once
#include<array>

namespace mmdl
{
	struct pmx_header_buffer
	{
		// バージョン
		float version{};

		// エンコード
		//  0:UTF16 1:UTF8
		std::uint8_t encode{};

		// 追加UVの数
		// 0-4
		std::uint8_t additional_uv_num{};

		// 頂点のインデックスのサイズ
		// 1,2,4のいずれか
		std::uint8_t vertex_index_size{};

		// テクスチャのインデックスのサイズ
		std::uint8_t texture_index_size{};

		// テクスチャのインデックスのサイズ
		// 1,2,4のいずれか
		std::uint8_t material_index_size{};

		// ボーンのインデックスのサイズ
		// 1,2,4のいずれか
		std::uint8_t bone_index_size{};

		// モーフのインデックスの大きさ
		// 1,2,4のいずれか
		std::uint8_t morph_index_size{};

		// 剛体のインデックスの大きさ
		std::uint8_t rigid_body_index_size{};
	};


	template<typename CharType, std::size_t CharBufferSize>
	struct pmx_info_buffer
	{
		// モデルの名前
		std::int32_t model_name_size{};
		std::array<CharType, CharBufferSize> model_name{};

		// 英語のモデルの名前
		std::int32_t english_model_name_size{};
		std::array<CharType, CharBufferSize> english_model_name{};

		// コメント
		std::int32_t comment_size{};
		std::array<CharType, CharBufferSize> comment{};

		// 英語のコメント
		std::int32_t english_comment_size{};
		std::array<CharType, CharBufferSize> english_comment{};
	};
}