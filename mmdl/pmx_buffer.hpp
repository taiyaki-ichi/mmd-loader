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


	struct pmx_vertex_buffer
	{
		// 位置
		std::array<float, 3> position{};

		// 法線
		std::array<float, 3> normal{};

		// UV
		std::array<float, 2> uv{};

		// 追加UV
		std::array<std::array<float, 4>, 4> additional_uv{};

		// ウェイト変形形式
		// 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF
		std::uint8_t weight_type{};

		// ボーンのインデックス
		std::array<std::size_t, 4> bone_index{};

		// ボーンのウェイト
		std::array<float, 4> bone_weight{};

		// SDEF-C値
		std::array<float, 3> sdef_c{};

		// SDEF-R0値
		std::array<float, 3> sdef_r0{};

		// SDEF-R1値
		std::array<float, 3> sdef_r1{};

		// エッジ倍率
		float edge_factor{};
	};


	template<typename CharType, std::size_t CharBufferSize>
	struct pmx_material_buffer
	{
		// マテリアルの名前
		std::int32_t name_size{};
		std::array<CharType,CharBufferSize> name{};

		// 英語のマテリアルの名前
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// ディフューズ
		std::array<float, 4> diffuse{};
		// スぺキュラ
		std::array<float, 3> specular{};
		// スぺキュラ係数
		float specularity{};
		// アンビエント
		std::array<float, 3> ambient{};

		/*
		描画フラグ 
			0x01:両面描画 
			0x02:地面影
			0x04:セルフシャドウマップへの描画
			0x08:セルフシャドウの描画
			0x10:エッジ描画
		*/
		std::uint8_t bit_flag{};

		// エッジの色
		std::array<float, 4> edge_color{};
		// エッジの大きさ
		float edge_size{};

		// 通常のテクスチャのインデックス
		std::size_t texture_index{};
		// スフィアテクスチャのインデックス
		std::size_t sphere_texture_index{};
		
		/*
		スフィアモード 
			0:無効 
			1:乗算(sph) 
			2:加算(spa) 3:サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
		*/
		std::uint8_t sphere_mode{};

		// 共有トゥーンフラグ 
		std::uint8_t toon_flag{};
		// 共有トゥーンフラグが0: テクスチャのインデックス
		// 共有トゥーンフラグが1: 0..9 -> toon01.bmp...toon10.bmp に対応
		std::size_t toon_index{};

		// メモ
		std::int32_t memo_size{};
		std::array<CharType, CharBufferSize> memo{};

		// マテリアルの対応する頂点の数
		//  (必ず3の倍数になる)
		std::int32_t vertex_num{};
	};
}