#pragma once
#include<utility>
#include<array>
#include<optional>
#include<bitset>

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


	// 面を構成する頂点の情報
	// 3つで一つの三角形の面を表す
	// pmx_header.vertex_index_sizeは1,2,4の値をとるので、最大値に対応したuint32_tをデフォルトに指定
	// WARNING: いらないかも
	template<typename VertexIndex = std::uint32_t>
	using pmx_surface = VertexIndex;


	// マテリアル
	// pmx_header.texture_index_sizeは1,2,4の値をとるので、最大値に対応したuint32_tをデフォルトに指定
	template<typename Str, typename Vec3, typename Vec4, typename TextureIndex = std::uint32_t>
	struct pmx_material
	{
		// マテリアルの名前
		Str name;

		// マテリアルの英語の名前
		Str english_name;

		// ディフューズ
		Vec4 diffuse;
		// スぺキュラ
		Vec3 specular;
		// スぺキュラ係数
		float specularity;
		// アンビエント
		Vec3 ambient;

		enum class draw_flag
		{
			// 片面の描画を行うかどうか
			single_sided,

			// 地面に影を落とすかどうか
			grond_shadow,

			// セルフシャドウマップへ描画するかどうか
			// WARNING: 名前分かりにくい
			draw_to_self_shadow_map,

			// セルフシャドウを描画するかどうか
			// WARNING: 名前分かりにくい
			draw_self_shadow,

			// エッジを描画するかどうか
			draw_edge,
		};

		// 描画のオプション
		// WARNING: マジックナンバー
		std::bitset<5> draw_flag_bits;

		// エッジの色
		Vec4 edge_color;
		// エッジの大きさ
		float edge_size;

		// 通常のテクスチャのテクスチャテーブルの参照インデックス
		TextureIndex normal_texture_index;
		// スフィアテクスチャのテクスチャテーブルの参照インデックス
		TextureIndex sphere_texture_index;

		enum class sphere_mode
		{
			// 何もなし
			none,

			// 乗算
			sph,

			// 加算
			spa,

			// サブテクスチャ
			// 追加UV1のx,yをUV参照して通常テクスチャ描画を行う
			subtexture,
		};

		sphere_mode sphere_mode_value;


		// toon_flag=0の時、toon_textureはToonテクスチャのテクスチャテーブルの参照インデックス
		// toon_flag=1の時、共有テクスチャのインデックス（0から9までの値）（0ならtoon01.bmp,1ならtoon02.bmpのように対応）
		std::uint8_t toon_flag;
		TextureIndex toon_texture;

		// メモ、自由欄
		Str memo;

		// 材質に対応する頂点の数
		// 必ず3の倍数になる
		std::int32_t vertex_number;

	};

}