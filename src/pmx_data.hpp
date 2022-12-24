#pragma once
#include<utility>

namespace mmdl
{
	enum class encode_type
	{
		utf16,
		utf8,
	};

	// PMXヘッダ
	struct pmx_header
	{
		// バージョン
		float version;

		// エンコードの方式
		encode_type encode;

		// 頂点の追加UVの数
		// 1,2,4のいずれか
		std::uint8_t add_uv_number;

		// 頂点のインデックスの大きさ
		// 1,2,4のいずれか
		std::uint8_t vertex_index_size;

		// テクスチャのインデックスの大きさ
		// 1,2,4のいずれか
		std::uint8_t texture_index_size;

		// マテリアルのインデックスの大きさ
		// 1.2.4のいずれか
		std::uint8_t material_index_size;

		// ボーンのインデックスの大きさ
		// 1,2,4のいずれか
		std::uint8_t bone_index_size;

		// リジットボディのインデックスの大きさ
		// 1,2,4のいずれか
		std::uint8_t rigid_body_index_size;

	};

}