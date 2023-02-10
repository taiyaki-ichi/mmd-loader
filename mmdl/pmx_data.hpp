#pragma once
#include<utility>
#include<array>
#include<optional>
#include<bitset>
#include"generics_type.hpp"
#include<vector>

namespace mmdl
{
	// エンコードのタイプ
	// 対応する整数は1文字の大きさ
	enum class encode_type
	{
		utf16 = 2,
		utf8 = 1,
	};

	// pmxヘッダ
	// Dataはpmx2.0ではサイズは8固定
	template<typename HeaderDataType = std::int64_t>
	struct pmx_header
	{
		// バージョン
		float version;

		// エンコードの方式
		encode_type encode;

		// 頂点の追加UVの数
		// 1,2,4のいずれか
		HeaderDataType add_uv_number;

		// 頂点のインデックスの大きさ
		// 1,2,4のいずれか
		HeaderDataType vertex_index_size;

		// テクスチャのインデックスの大きさ
		// 1,2,4のいずれか
		HeaderDataType texture_index_size;

		// マテリアルのインデックスの大きさ
		// 1.2.4のいずれか
		HeaderDataType material_index_size;

		// ボーンのインデックスの大きさ
		// 1,2,4のいずれか
		HeaderDataType bone_index_size;

		// モー負のインデックスの大きさ
		// 1,2,4のいずれか
		HeaderDataType morph_index_size;

		// リジットボディのインデックスの大きさ
		// 1,2,4のいずれか
		HeaderDataType rigid_body_index_size;

		// 「1,2,4のいずれか」について
		// 1: 1byteのunsigned
		// 2: 2byteのunsigned
		// 4: 4byteのsigned
		// となるので注意

	};

	template<typename HeaderDataType>
	struct pmx_header_traits<pmx_header<HeaderDataType>>
	{
		static pmx_header<HeaderDataType> construct(float version, std::uint8_t encode, std::size_t add_uv_num, std::size_t vertex_index_size, std::size_t texture_index_size,
			std::size_t material_index_size, std::size_t bone_index_size, std::size_t morph_index_size, std::size_t rigid_body_index_size)
		{
			auto result = pmx_header<HeaderDataType>{
				version,
				encode == 0 ? encode_type::utf16 : encode_type::utf8,
				static_cast<HeaderDataType>(add_uv_num),
				static_cast<HeaderDataType>(vertex_index_size),
				static_cast<HeaderDataType>(texture_index_size),
				static_cast<HeaderDataType>(material_index_size),
				static_cast<HeaderDataType>(bone_index_size),
				static_cast<HeaderDataType>(morph_index_size),
				static_cast<HeaderDataType>(rigid_body_index_size)
			};

			return result;
		}
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

	template<typename Str>
	struct pmx_info_traits<pmx_info<Str>>
	{
		using char_type = Str::value_type;

		static pmx_info<Str> construct(
			char_type* model_name_str, std::size_t mode_name_str_size,
			char_type* english_model_name_str, std::size_t english_mode_name_str_size,
			char_type* comment_str, std::size_t comment_str_size,
			char_type* english_comment_str, std::size_t english_comment_str_size
		)
		{
			return {
				Str(model_name_str,mode_name_str_size),
				Str(english_model_name_str,english_mode_name_str_size),
				Str(comment_str,comment_str_size),
				Str(english_comment_str,english_comment_str_size)
			};
		}
	};

	// 1頂点情報
	// ボーンのインデックスの大きさについて、pmx_header.bone_index_sizeは1,2,4の値をとる
	// その最大の値である4の大きさに対応したstd::int32_tをデフォルトとして使用する
	template<typename Vec2, typename Vec3, typename Vec4, typename BoneIndex = std::int32_t>
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

	template<typename Vec2, typename Vec3, typename Vec4, typename BoneIndex>
	struct pmx_vertex_traits<std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>>
	{
		// サイズを指定して構築
		static std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> construct(std::size_t size) {

			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> result{};
			result.reserve(size);
			return result;
		}

		// BDEF1形式のボーンの追加
		static void emplace_back_BDEF1(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ウェイト1.0の単一ボーン
			std::size_t bone_index_1,
			// エッジ倍率
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.weight[0] = 1.f;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

		// BDEF2形式のボーンの追加
		static void emplace_back_BDEF2(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ボーン1のインデックス
			std::size_t bone_index_1,
			// ボーン2のインデックス
			std::size_t bone_index_2,
			// ボーン1のウェイト(ボーン2のウェイトは 1.0 - ボーン1のウェイト )
			float bone_weight_1,
			// エッジ倍率
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.bone[1] = bone_index_2;
			v.weight[0] = bone_weight_1;
			v.weight[1] = 1.f - bone_weight_1;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

		// BDEF4形式のボーンの追加
		static void emplace_back_BDEF4(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ボーン1のインデックス
			std::size_t bone_index_1,
			// ボーン2のインデックス
			std::size_t bone_index_2,
			// ボーン3のインデックス
			std::size_t bone_index_3,
			// ボーン4のインデックス
			std::size_t bone_index_4,
			// ボーン1のウェイト
			float bone_weight_1,
			// ボーン2のウェイト
			float bone_weight_2,
			// ボーン3のウェイト
			float bone_weight_3,
			// ボーン4のウェイト（ウェイトの合計について1の保証はない）
			float bone_weight_4,
			// エッジ倍率
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.bone[1] = bone_index_2;
			v.bone[2] = bone_index_3;
			v.bone[3] = bone_index_4;
			auto weight_sum = bone_weight_1 + bone_index_2 + bone_index_3 + bone_index_4;
			v.weight[0] = bone_weight_1 / weight_sum;
			v.weight[1] = bone_weight_2 / weight_sum;
			v.weight[2] = bone_weight_3 / weight_sum;
			v.weight[3] = bone_weight_4 / weight_sum;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

		// SDEF形式のボーンの追加
		static void emplace_back_SDEF(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ボーン1のインデックス
			std::size_t bone_index_1,
			// ボーン2のインデックス
			std::size_t bone_index_2,
			// ボーン1のウェイト(ボーン2のウェイトは 1.0 - ボーン1のウェイト )
			float bone_weight_1,
			// SDEF-C値(x,y,z)
			std::array<float, 3> const& sdef_c,
			// SDEF-R0値(x,y,z)
			std::array<float, 3> const& sdef_r0,
			// SDEF-R1値(x,y,z) ※修正値を要計算
			std::array<float, 3> const& sdef_r1,
			// エッジ倍率
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.bone[1] = bone_index_2;
			v.weight[0] = bone_weight_1;
			v.weight[1] = 1.f - bone_weight_1;

			std::array<Vec3, 3> sdef{ {
				{sdef_c[0],sdef_c[1],sdef_c[2]},
				{sdef_r0[0],sdef_r0[1],sdef_r0[2]},
				{sdef_r1[0],sdef_r1[1],sdef_r1[2]}
			} };
			v.sdef = sdef;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

	};



	// 面を構成する頂点の情報
	// 3つで一つの三角形の面を表す
	// pmx_header.vertex_index_sizeは1,2,4の値をとるので、最大値に対応したint32_tをデフォルトに指定
	// WARNING: いらないかも
	template<typename VertexIndex = std::int32_t>
	using pmx_surface = VertexIndex;

	template<typename VertexIndex>
	struct pmx_surface_traits<std::vector<pmx_surface<VertexIndex>>>
	{
		// サイズを指定して構築
		static std::vector<pmx_surface<VertexIndex>> construct(std::size_t size)
		{
			std::vector<pmx_surface<VertexIndex>> result{};
			result.reserve(size);

			return result;
		}

		// 要素を追加
		static void emplace_back(std::vector<pmx_surface<VertexIndex>>& surface, std::size_t index)
		{
			surface.emplace_back(index);
		}
	};

	// pmx_materialで使用
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

	// pmx_materialで使用
	enum class toon_type
	{
		// 共有
		shared,

		// 個別
		unshared,
	};

	// マテリアル
	// pmx_header.texture_index_sizeは1,2,4の値をとるので、最大値に対応したint32_tをデフォルトに指定
	template<typename Str, typename Vec3, typename Vec4, typename TextureIndex = std::int32_t>
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
		TextureIndex texture_index;
		// スフィアテクスチャのテクスチャテーブルの参照インデックス
		TextureIndex sphere_texture_index;

		sphere_mode sphere_mode_value;


		// toon_type::unsharedの時、toon_textureはToonテクスチャのテクスチャテーブルの参照インデックス
		// toon_type::sharedの時、共有テクスチャのインデックス（0から9までの値）（0ならtoon01.bmp,1ならtoon02.bmpのように対応）
		toon_type toon_type_value;
		TextureIndex toon_texture;

		// メモ、自由欄
		Str memo;

		// 材質に対応する頂点の数
		// 必ず3の倍数になる
		std::int32_t vertex_number;

	};

	enum class bone_flag
	{
		// 接続先法事方法
		// false: 座標オフセットで指定
		// true: ボーンで指定
		access_point = 0,

		// 回転可能かどうか
		rotatable = 1,

		// 移動可能かどうか
		movable = 2,

		// 表示
		// WARNING; 表示可能かどうかという意味か?
		display = 3,

		// 操作可能かどうか
		operable = 4,

		// IK使うかどうか
		ik = 5,

		// ローカル付与
		// false: ユーザー変形値／IKリンク／多重付与
		// true: 親のローカル変形量
		local_grant = 7,

		// 回転付与
		rotation_grant = 8,

		// 移動付与
		move_grant = 9,

		// 軸固定かどうか
		fix_axis = 10,

		// ローカル軸かどうか
		local_axis = 11,

		// 物理後変形かどうか
		post_physical_deformation = 12,

		// 外部親変形かどうか
		external_parent_deformation = 13,

	};

	template<typename Vec3, typename BoneIndex>
	struct ik_link {
		// リンクしているボーン
		BoneIndex bone;

		// 角度制限を行う場合の最小、最大の角度の制限
		std::optional<std::pair<Vec3, Vec3>> min_max_angle_limit;
	};

	// ボーン
	// pmx_header.bone_index_sizeは1,2,4の値をとるので、最大値に対応したint32_tをデフォルトに指定
	template<typename Str, typename Vec3, template<typename> typename Container, typename BoneIndex = std::int32_t>
	struct pmx_bone
	{
		// ボーンの名前
		Str name;

		// ボーンの英語の名前
		Str english_name;

		// 位置
		Vec3 position;
		// 親ボーンのインデックス
		BoneIndex parent_index;
		// 変形階層
		std::int32_t trannsformation_level;

		// WARNING: マジックナンバー
		std::bitset<16> bone_flag_bits;

		// bone_flag_bits[access_point]=falseのとき使用
		// ボーンの位置からの相対的な位置を表す
		Vec3 access_point_offset;
		// bone_flag_bits[access_point]=tureのとき使用
		// 接続先のボーンのインデックスを表す
		BoneIndex access_point_index;

		// bone_flag_bits[rotation_grant]=trueまたは bone_flag_bits[move_grant]=trueのとき使用
		// 対象のボーンを表す
		BoneIndex grant_index;
		// 付与率
		float grant_rate;

		// bone_flag_bits[fix_axis]=trueの時に使用
		// 固定された軸の方向ベクトル
		Vec3 fix_axis_direction;

		// bone_flag_bits[local_axis]=trueの時に使用
		// x軸の方向ベクトル
		Vec3 local_axis_x;
		// z軸の方向ベクトル
		Vec3 local_axis_z;

		// bone_flag_bits[external_parent_deformation]=trueの時に使用
		// Keyの値
		std::int32_t external_parent_deformation_key;

		// bone_flag_bits[ik]=trueの時に使用
		//IKのターゲットのボーン
		BoneIndex ik_target_bone;
		// IKのループ回数
		std::int32_t ik_roop_number;
		// IKのループを行う際の1回当たりの制限角度（ラジアン）
		float ik_rook_angle;

		Container<ik_link<Vec3, BoneIndex>> ik_link;


		// TODO: 「 bone_flag_bits[-]=trueの時に使用」って書いてあるとこはOptionalでイイかも
	};

}