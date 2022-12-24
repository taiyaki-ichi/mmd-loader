#pragma once
#include<utility>
#include<array>
#include<optional>

namespace mmdl
{
	enum class encode_type
	{
		utf16,
		utf8,
	};

	// pmxヘッダ
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


	// モデルの名前とその他の情報
	template<typename Str>
	struct pmx_info
	{
		// モデルの名前
		Str model_name;

		// モデルの英語名
		Str english_mode_name;

		// コメント
		Str comment;

		// 英語のコメント
		Str english_comment;

	};



	// 1頂点情報
	// ボーンのインデックスの大きさについて、pmx_header.bone_index_sizeは1,2,4の値をとる
	// その最大の値である4の大きさに対応したstd::uint32_tをデフォルトとして使用する
	template<typename Vec2, typename Vec3, typename Vec4, template<typename> typename Container,
		typename BoneIndex = std::uint32_t>
		struct pmx_vertex
	{
		// 頂点座標
		Vec3 position;

		// 法線
		Vec3 normal;

		// uv
		Vec2 uv;

		// 追加のuv
		// pmx_header.add_uv_numberの数だけ使用する
		std::array<Vec4, 4> additional_uv{};

		// 影響を受けるボーンの数とweightが対応している
		// BDEF1ならweight[0]=1、そのほかは0
		// BDEF2ならweight[0]!=0,weight[1]!=0、そのほかは0（ちなみにweight[0]+weight[1]=1）
		// BDEF4ならweightは全て0でない
		std::array<BoneIndex, 4> bone{ 0,0,0,0 };
		std::array<float, 4> weight{ 0.f,0.f,0.f,0.f };

		// SDEF用の補正行列
		// SDEFはBDEF2と補正行列によって計算される
		// sdef[0]：SDEF-C値
		// sdef[1]：SDEF-R0値
		// sdef[2]：SDEF-R1値
		std::optional<std::array<Vec3, 3>> sdef = std::nullopt;

		// エッジ倍率
		// 材質のエッジサイズに対しての倍率値
		float edge_magnification;

	};
}