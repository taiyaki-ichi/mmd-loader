#pragma once
#include<utility>
#include<array>
#include<optional>
#include<bitset>

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
	template<typename HeaderData = std::int64_t>
	struct pmx_header
	{
		// バージョン
		float version;

		// エンコードの方式
		encode_type encode;

		// 頂点の追加UVの数
		// 1,2,4のいずれか
		HeaderData add_uv_number;

		// 頂点のインデックスの大きさ
		// 1,2,4のいずれか
		HeaderData vertex_index_size;

		// テクスチャのインデックスの大きさ
		// 1,2,4のいずれか
		HeaderData texture_index_size;

		// マテリアルのインデックスの大きさ
		// 1.2.4のいずれか
		HeaderData material_index_size;

		// ボーンのインデックスの大きさ
		// 1,2,4のいずれか
		HeaderData bone_index_size;

		// モー負のインデックスの大きさ
		// 1,2,4のいずれか
		HeaderData morph_index_size;

		// リジットボディのインデックスの大きさ
		// 1,2,4のいずれか
		HeaderData rigid_body_index_size;

		// 「1,2,4のいずれか」について
		// 1: 1byteのunsigned
		// 2: 2byteのunsigned
		// 4: 4byteのsigned
		// となるので注意

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


	// 面を構成する頂点の情報
	// 3つで一つの三角形の面を表す
	// pmx_header.vertex_index_sizeは1,2,4の値をとるので、最大値に対応したint32_tをデフォルトに指定
	// WARNING: いらないかも
	template<typename VertexIndex = std::int32_t>
	using pmx_surface = VertexIndex;


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

		enum class bone_flag
		{
			// 接続先法事方法
			// false: 座標オフセットで指定
			// true: ボーンで指定
			access_point,

			// 回転可能かどうか
			rotatable,

			// 移動可能かどうか
			movable,

			// 表示
			// WARNING; 表示可能かどうかという意味か?
			display,

			// 操作可能かどうか
			operable,

			// IK使うかどうか
			ik,

			// ローカル付与
			// false: ユーザー変形値／IKリンク／多重付与
			// true: 親のローカル変形量
			local_grant,

			// 回転付与
			rotation_grant,

			// 移動付与
			move_grant,

			// 軸固定かどうか
			fix_axis,

			// ローカル軸かどうか
			local_axis,

			// 物理後変形かどうか
			post_physical_deformation,

			// 外部親変形かどうか
			external_parent_deformation,

		};

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
		Vec3 grant_rate;

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
		std::int16_t external_parent_deformation_key;

		// bone_flag_bits[ik]=trueの時に使用
		//IKのターゲットのボーン
		BoneIndex ik_target_bone;
		// IKのループ回数
		std::int16_t ik_roop_number;
		// IKのループを行う際の1回当たりの制限角度（ラジアン）
		float ik_rook_angle;

		struct ik_link {
			// リンクしているボーン
			BoneIndex bone;

			// 角度制限を行う場合の最小、最大の角度の制限
			std::optional<std::pair<Vec3, Vec3>> min_max_angle_limit;
		};

		Container<ik_link> ik_link;


		// TODO: 「 bone_flag_bits[-]=trueの時に使用」って書いてあるとこはOptionalでイイかも
	};

}