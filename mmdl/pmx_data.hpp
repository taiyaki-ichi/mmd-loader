#pragma once
#include<utility>
#include<array>
#include<optional>
#include<bitset>
#include"generics_type.hpp"
#include<vector>
#include<string>

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
		static pmx_header<HeaderDataType> construct(pmx_header_buffer const& buffer)
		{
			auto result = pmx_header<HeaderDataType>{
				buffer.version,
				buffer.encode == 0 ? encode_type::utf16 : encode_type::utf8,
				static_cast<HeaderDataType>(buffer.additional_uv_num),
				static_cast<HeaderDataType>(buffer.vertex_index_size),
				static_cast<HeaderDataType>(buffer.texture_index_size),
				static_cast<HeaderDataType>(buffer.material_index_size),
				static_cast<HeaderDataType>(buffer.bone_index_size),
				static_cast<HeaderDataType>(buffer.morph_index_size),
				static_cast<HeaderDataType>(buffer.rigid_body_index_size)
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

		template<typename CharType, std::size_t CharBufferSize>
		static pmx_info<Str> construct(pmx_info_buffer<CharType, CharBufferSize> const& buffer)
		{
			return {
				Str(&buffer.model_name[0],buffer.model_name_size),
				Str(&buffer.english_model_name[0],buffer.english_model_name_size),
				Str(&buffer.comment[0],buffer.comment_size),
				Str(&buffer.english_comment[0],buffer.english_comment_size)
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

		static void emplace_back(std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex, pmx_vertex_buffer const& buffer, std::uint8_t additional_uv_num)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { buffer.position[0],buffer.position[1] ,buffer.position[2] };
			v.normal = { buffer.normal[0],buffer.normal[1] ,buffer.normal[2] };
			v.uv = { buffer.uv[0],buffer.uv[1] };

			for (std::size_t i = 0; i < additional_uv_num; i++)
			{
				v.additional_uv[i] = { buffer.additional_uv[i][0],buffer.additional_uv[i][1] ,buffer.additional_uv[i][2] ,buffer.additional_uv[i][3] };
			}

			auto weight_sum = buffer.bone_weight[0] + buffer.bone_weight[1] + buffer.bone_weight[2] + buffer.bone_weight[3];

			switch (buffer.weight_type)
			{
				// BDEF1
			case 0:
				v.bone[0] = buffer.bone_index[0];
				v.weight[0] = 1.f;

				break;

				// BDEF2
			case 1:
				v.bone[0] = buffer.bone_index[0];
				v.bone[1] = buffer.bone_index[1];
				v.weight[0] = buffer.bone_weight[0];
				v.weight[1] = 1.f - buffer.bone_weight[0];

				break;

				// BDEF4
			case 2:
				v.bone[0] = buffer.bone_index[0];
				v.bone[1] = buffer.bone_index[1];
				v.bone[2] = buffer.bone_index[2];
				v.bone[3] = buffer.bone_index[3];
				v.weight[0] = buffer.bone_weight[0] / weight_sum;
				v.weight[1] = buffer.bone_weight[1] / weight_sum;
				v.weight[2] = buffer.bone_weight[2] / weight_sum;
				v.weight[3] = buffer.bone_weight[3] / weight_sum;

				break;

				// SDEF
			case 3:
				v.bone[0] = buffer.bone_index[0];
				v.bone[1] = buffer.bone_index[1];
				v.weight[0] = buffer.bone_weight[0];
				v.weight[1] = 1.f - buffer.bone_weight[0];

				std::array<Vec3, 3> sdef{ {
					{buffer.sdef_c[0],buffer.sdef_c[1],buffer.sdef_c[2]},
					{buffer.sdef_r0[0],buffer.sdef_r0[1],buffer.sdef_r0[2]},
					{buffer.sdef_r1[0],buffer.sdef_r1[1],buffer.sdef_r1[2]}
				} };
				v.sdef = sdef;

				break;
			}

			v.edge_magnification = buffer.edge_factor;

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


	template<>
	struct pmx_texture_path_traits<std::vector<std::wstring>>
	{
		using char_type = std::wstring::value_type;

		// サイズを指定して構築
		static std::vector<std::wstring> construct(std::size_t size) {
			std::vector<std::wstring> result{};
			result.reserve(size);
			return result;
		}

		// 要素を追加
		static void emplace_back(std::vector<std::wstring>& texture_path, char_type const* str, std::size_t size)
		{
			texture_path.emplace_back(str, size);
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

	template<typename Str, typename Vec3, typename Vec4, typename TextureIndex>
	struct pmx_material_traits<std::vector<pmx_material<Str, Vec3, Vec4, TextureIndex>>>
	{
		using char_type = Str::value_type;

		// サイズを指定して構築
		static std::vector<pmx_material<Str, Vec3, Vec4, TextureIndex>> construct(std::size_t size) {
			std::vector<pmx_material<Str, Vec3, Vec4, TextureIndex>> result{};
			result.reserve(size);
			return result;
		}

		// 要素を追加
		template<typename CharType, std::size_t CharBufferSize>
		static void emplace_back(std::vector<pmx_material<Str, Vec3, Vec4, TextureIndex>>& material, pmx_material_buffer<CharType, CharBufferSize> const& buffer)
		{
			auto sm = [&buffer]() {
				switch (buffer.sphere_mode) {
				case 1:
					return sphere_mode::sph;
				case 2:
					return sphere_mode::spa;
				case 3:
					return sphere_mode::subtexture;
				}

				return sphere_mode::none;
			}();

			pmx_material<Str, Vec3, Vec4, TextureIndex> m{
				Str(&buffer.name[0],buffer.name_size),
				Str(&buffer.english_name[0],buffer.english_name_size),
				{buffer.diffuse[0],buffer.diffuse[1],buffer.diffuse[2],buffer.diffuse[3]},
				{buffer.specular[0],buffer.specular[1],buffer.specular[2]},
				buffer.specularity,
				{buffer.ambient[0],buffer.ambient[1],buffer.ambient[2]},
				std::bitset<5>{buffer.bit_flag},
				{buffer.edge_color[0],buffer.edge_color[1],buffer.edge_color[2],buffer.edge_color[3]},
				buffer.edge_size,
				static_cast<TextureIndex>(buffer.texture_index),
				static_cast<TextureIndex>(buffer.sphere_texture_index),
				sm,
				buffer.toon_flag == 0 ? toon_type::unshared : toon_type::shared,
				buffer.toon_index,
				Str(&buffer.memo[0], buffer.memo_size),
				static_cast<std::int32_t>(buffer.vertex_num),
			};

			material.push_back(std::move(m));
		}
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

	template<typename Str, typename Vec3, template<typename> typename Container, typename BoneIndex>
	struct pmx_bone_traits<std::vector<pmx_bone<Str, Vec3, Container, BoneIndex>>>
	{
		using char_type = Str::value_type;

		// サイズを指定して構築
		static std::vector<pmx_bone<Str, Vec3, Container, BoneIndex>> construct(std::size_t size) {
			std::vector<pmx_bone<Str, Vec3, Container, BoneIndex>> result{};
			result.reserve(size);
			return result;
		}

		// 要素を追加
		static void emplace_back(std::vector<pmx_bone<Str, Vec3, Container, BoneIndex>>& bone,
			char_type* name, std::size_t name_size,
			char_type* english_name, std::size_t english_name_size,
			std::array<float, 3> const& position,
			std::size_t parent_bone_index,
			std::size_t transformation_level,

			std::uint16_t bone_flag,
			// 接続先のビットが0の場合に使用する座標オフセット
			std::array<float, 3> const& access_point_offset,
			// 接続先のビットが1の場合に使用する接続先のボーンのインデックス
			std::size_t access_bone_index,
			// 回転付与または移動付与のビットが1の場合に使用する付与親のボーンのインデックス
			std::size_t assign_bone_index,
			// 回転付与または移動付与のビットが1の場合に使用する付与率
			float assign_rate,
			// 軸固定のビットが1の場合に使用する軸の方向ベクトル
			std::array<float, 3> const& fixed_axis_direction,
			// ローカル軸のビットが1の場合に使用するX軸の方向ベクトル
			std::array<float, 3> const& local_x_axis_direction,
			// ローカル軸のビットが1の場合に使用するZ軸の方向ベクトル
			std::array<float, 3> const& local_z_axis_direction,
			// 外部親変形のビットが1の場合に使用するKey値
			std::size_t key_value,
			// 以下、IKのビットが1の場合に使用する
			// IKターゲットのボーン
			std::size_t ik_target_bone,
			// IKのループの回数
			std::size_t ik_loop_num,
			// IKの1回あたりの角度制限
			float ik_angle_limit_par_process,
			// IKリンクの数
			std::size_t ik_link_size,
			// IKリンク
			// ボーンのインデックス、角度制限の有無、角度制限の下限、角度制限の上限、、、の順
			std::tuple<std::size_t, std::uint8_t, std::array<float, 3>, std::array<float, 3>>* ik_link
		)
		{
			Container<mmdl::ik_link<Vec3, BoneIndex>> il(ik_link_size);
			for (std::size_t i = 0; i < ik_link_size; i++)
			{
				auto const& [bone_index, is_limit, bottom, top] = ik_link[i];
				il[i].bone = static_cast<BoneIndex>(bone_index);
				if (is_limit == 1) {
					il[i].min_max_angle_limit = std::make_pair(Vec3{ bottom[0],bottom[1] ,bottom[2] }, Vec3{ top[0],top[1] ,top[2] });
				}

			}

			pmx_bone<Str, Vec3, Container, BoneIndex> b{
				Str(name,name_size),
				Str(english_name,english_name_size),
				{position[0],position[1],position[2]},
				static_cast<BoneIndex>(parent_bone_index),
				static_cast<std::int32_t>(transformation_level),
				std::bitset<16>{bone_flag},
				{access_point_offset[0],access_point_offset[1],access_point_offset[2]},
				static_cast<BoneIndex>(access_bone_index),
				static_cast<BoneIndex>(assign_bone_index),
				assign_rate,
				{fixed_axis_direction[0],fixed_axis_direction[1],fixed_axis_direction[2]},
				{local_x_axis_direction[0],local_x_axis_direction[1],local_x_axis_direction[2]},
				{local_z_axis_direction[0],local_z_axis_direction[1],local_z_axis_direction[2]},
				static_cast<std::size_t>(key_value),
				static_cast<BoneIndex>(ik_target_bone),
				static_cast<std::int32_t>(ik_loop_num),
				ik_angle_limit_par_process,
				std::move(il)
			};

			bone.push_back(std::move(b));
		}
	};

}