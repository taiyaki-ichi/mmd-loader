#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>

namespace mmdl
{
	// ヘッダの読み込み
	// pmx2.0以降はDataのサイズは1byteなのでデフォルトはuint8_t
	template<typename T, typename traits = pmx_header_traits<T>>
	T load_header(std::istream& in)
	{
		// 最初の4文字はいらない「Pmx 」の文字
		in.seekg(4);

		float version;
		read_from_istream(in, &version);

		// 以降のデータの大きさの読み込み
		std::uint8_t size;
		read_from_istream(in, &size);
		// bitからbyteへ変換
		// WARNING: ここだけbitっぽい?
		size /= 8;

		std::uint8_t encode;
		read_intanger_from_istream(in, &encode, size);
		std::size_t add_uv_number;
		read_intanger_from_istream(in, &add_uv_number, size);

		std::size_t vertex_index_size;
		read_intanger_from_istream(in, &vertex_index_size, size);
		std::size_t texture_index_size;
		read_intanger_from_istream(in, &texture_index_size, size);
		std::size_t material_index_size;
		read_intanger_from_istream(in, &material_index_size, size);
		std::size_t bone_index_size;
		read_intanger_from_istream(in, &bone_index_size, size);
		std::size_t morph_index_size;
		read_intanger_from_istream(in, &morph_index_size, size);
		std::size_t rigid_body_index_size;
		read_intanger_from_istream(in, &rigid_body_index_size, size);

		return traits::construct(
			version, encode, add_uv_number, vertex_index_size, texture_index_size, material_index_size, bone_index_size, morph_index_size, rigid_body_index_size
		);
	}

	template<typename T, typename traits = pmx_info_traits<T>, std::size_t BufferSize = 512>
	T load_info(std::istream& in)
	{
		using char_type = typename traits::char_type;

		std::int32_t model_name_size{};
		char_type model_name_buffer[BufferSize]{};

		std::int32_t english_model_name_size{};
		char_type english_model_name_buffer[BufferSize]{};

		std::int32_t comment_size{};
		char_type comment_buffer[BufferSize]{};

		std::int32_t english_comment_size{};
		char_type english_comment_name_buffer[BufferSize]{};


		// モデル名の取得
		read_from_istream(in, &model_name_size);
		read_from_istream(in, model_name_buffer, model_name_size);

		// モデルの英語名の取得
		read_from_istream(in, &english_model_name_size);
		read_from_istream(in, english_model_name_buffer, english_model_name_size);

		// コメントの取得
		read_from_istream(in, &comment_size);
		read_from_istream(in, comment_buffer, comment_size);

		// 英語のコメントの取得
		read_from_istream(in, &english_comment_size);
		read_from_istream(in, english_comment_name_buffer, english_comment_size);

		return traits::construct(
			model_name_buffer, static_cast<std::size_t>(model_name_size),
			english_model_name_buffer, static_cast<std::size_t>(english_model_name_size),
			comment_buffer, static_cast<std::size_t>(comment_size),
			english_comment_name_buffer, static_cast<std::size_t>(english_comment_size)
		);
	}


	// 頂点情報の読み込み
	template<typename T, typename traits = pmx_vertex_traits<T>>
	T load_vertex(std::istream& in, std::size_t add_uv_number, std::size_t bone_index_size)
	{
		// 頂点の数を取得
		std::int32_t num;
		read_from_istream(in, &num);

		auto result = traits::construct(static_cast<std::size_t>(num));

		std::array<float, 3> position{};
		std::array<float, 3> normal{};
		std::array<float, 2> uv{};
		std::array<std::array<float, 4>, 4> additional_uv{};
		std::size_t bone_index_1{}, bone_index_2{}, bone_index_3{}, bone_index_4{};
		float bone_weight_1{}, bone_weight_2{}, bone_weight_3{}, bone_weight_4{};
		std::array<float, 3> sdef_c{};
		std::array<float, 3> sdef_r0{};
		std::array<float, 3> sdef_r1{};
		float edge_factor{};

		// それぞれの頂点の取得
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_from_istream(in, &position);
			read_from_istream(in, &normal);
			read_from_istream(in, &uv);

			// 追加uvの取得
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				read_from_istream(in, &additional_uv[i]);
			}

			// ウェイト変形方式の取得
			std::uint8_t weight_type;
			read_from_istream(in, &weight_type);

			switch (weight_type)
			{
				// BDEF1の場合
			case 0:
				read_intanger_from_istream(in, &bone_index_1, bone_index_size);

				read_from_istream(in, &edge_factor);
				traits::emplace_back_BDEF1(result, position, normal, uv, &additional_uv[0], add_uv_number,
					bone_index_1, edge_factor);
				break;

				// BDEF2の場合
			case 1:
				read_intanger_from_istream(in, &bone_index_1, bone_index_size);
				read_intanger_from_istream(in, &bone_index_2, bone_index_size);
				read_from_istream(in, &bone_weight_1);

				read_from_istream(in, &edge_factor);
				traits::emplace_back_BDEF2(result, position, normal, uv, &additional_uv[0], add_uv_number,
					bone_index_1, bone_index_2, bone_weight_1, edge_factor);
				break;

				// BDEF4の場合
			case 2:
				read_intanger_from_istream(in, &bone_index_1, bone_index_size);
				read_intanger_from_istream(in, &bone_index_2, bone_index_size);
				read_intanger_from_istream(in, &bone_index_3, bone_index_size);
				read_intanger_from_istream(in, &bone_index_4, bone_index_size);

				read_from_istream(in, &bone_weight_1);
				read_from_istream(in, &bone_weight_2);
				read_from_istream(in, &bone_weight_3);
				read_from_istream(in, &bone_weight_4);

				read_from_istream(in, &edge_factor);
				traits::emplace_back_BDEF4(result, position, normal, uv, &additional_uv[0], add_uv_number,
					bone_index_1, bone_index_2, bone_index_3, bone_index_4, bone_weight_1, bone_weight_2, bone_weight_3, bone_weight_4, edge_factor);
				break;

				// SDEFの倍
			case 3:
				read_intanger_from_istream(in, &bone_index_1, bone_index_size);
				read_intanger_from_istream(in, &bone_index_2, bone_index_size);
				read_from_istream(in, &bone_weight_1);
				// ここまでBDEF2と同じ


				read_from_istream(in, &sdef_c);
				read_from_istream(in, &sdef_r0);
				read_from_istream(in, &sdef_r1);

				read_from_istream(in, &edge_factor);
				traits::emplace_back_SDEF(result, position, normal, uv, &additional_uv[0], add_uv_number,
					bone_index_1, bone_index_2, bone_weight_1, sdef_c, sdef_r0, sdef_r1, edge_factor);

				break;
			}
		}

		return result;
	}

	// 面情報の読み込み
	template<typename T, typename traits = pmx_surface_traits<T>>
	T load_surface(std::istream& in, std::size_t vertex_index_size)
	{
		// 面の数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ設定
		auto result = traits::construct(static_cast<std::size_t>(num));

		std::size_t index{};
		// それぞれの情報を取得
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_intanger_from_istream(in, &index, vertex_index_size);
			traits::emplace_back(result, index);
		}

		return result;
	}

	// テクスチャパスの読み込み
	template<typename T, typename traits = pmx_texture_path_traits<T>, std::size_t BufferSize = 64>
	T load_texture_path(std::istream& in)
	{
		using char_type = traits::char_type;

		// コンテナのサイズの指定
		std::int32_t num;
		read_from_istream(in, &num);
		auto result = traits::construct(static_cast<std::size_t>(num));

		std::int32_t texture_path_size{};
		char_type texture_path_buffer[BufferSize]{};

		// 1文字列ごとに取得していく
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 長さの取得
			read_from_istream(in, &texture_path_size);

			// 文字列の読み込み
			read_from_istream(in, texture_path_buffer, texture_path_size);

			// 文字列を格納
			traits::emplace_back(result, texture_path_buffer, static_cast<std::size_t>(texture_path_size));

			// 初期化しておく
			std::fill(std::begin(texture_path_buffer), std::end(texture_path_buffer), 0);
		}

		return result;
	}

	// マテリアルの読み込み
	template<template<typename>typename Container, typename Str, constructible_vec3 Vec3, constructible_vec4 Vec4,
		typename TextureIndex = std::int32_t, typename HeaderDataType = std::uint8_t, typename ContianerSizeType = std::size_t, typename StrSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_material<Str, Vec3, Vec4, TextureIndex>>, ContianerSizeType>,
		typename StrTraits = count_construct_container_traits<Str, StrSizeType>>
		Container<pmx_material<Str, Vec3, Vec4, TextureIndex>> load_material(std::istream& in, encode_type encode, HeaderDataType texture_index_size)
	{

		// マテリアルの数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ指定し構築
		auto result = ContainerTraits::construct(num);

		// 文字の大きさ
		auto const char_size = static_cast<StrTraits::size_type>(encode);

		// それぞれのマテリアルの読み込み
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 読み込むマテリアル
			auto& material = ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i));

			// 文字列を読み込む際に使用
			std::int32_t len;

			// 名前
			read_from_istream(in, &len);
			material.name = StrTraits::construct(len);
			read_array_from_istream<StrTraits>(in, &material.name, len / char_size, char_size);

			// 英語の名前
			read_from_istream(in, &len);
			material.english_name = StrTraits::construct(len);
			read_array_from_istream<StrTraits>(in, &material.english_name, len / char_size, char_size);

			// 色情報
			read_vec4_from_istream(in, &material.diffuse);
			read_vec3_from_istream(in, &material.specular);
			read_from_istream(in, &material.specularity);
			read_vec3_from_istream(in, &material.ambient);

			// 描画フラグ
			std::uint8_t flag;
			read_from_istream(in, &flag);
			material.draw_flag_bits = { flag };

			// エッジ
			read_vec4_from_istream(in, &material.edge_color);
			read_from_istream(in, &material.edge_size);

			// テクスチャ
			read_intanger_from_istream(in, &material.texture_index, texture_index_size);
			read_intanger_from_istream(in, &material.sphere_texture_index, texture_index_size);

			// スフィアモード
			std::uint8_t sphere;
			read_from_istream(in, &sphere);
			switch (sphere)
			{
				// 無効
			case 0:
				material.sphere_mode_value = sphere_mode::none;
				break;

				// 乗算
			case 1:
				material.sphere_mode_value = sphere_mode::sph;
				break;

				// 加算
			case 2:
				material.sphere_mode_value = sphere_mode::spa;
				break;

				// サブテクスチャ
			case 3:
				material.sphere_mode_value = sphere_mode::subtexture;
				break;
			}

			// トゥーン
			std::uint8_t toon;
			read_from_istream(in, &toon);

			switch (toon)
			{
				// 個別
			case 0:
				material.toon_type_value = toon_type::unshared;
				read_from_istream(in, &material.toon_texture, texture_index_size);
				break;

				// 共有
			case 1:
				material.toon_type_value = toon_type::shared;
				// 共有の場合のインデックスの場合のサイズは1byte
				read_from_istream(in, &material.toon_texture, 1);
				break;
			}

			// メモ
			read_from_istream(in, &len);
			material.memo = StrTraits::construct(len);
			read_array_from_istream<StrTraits>(in, &material.memo, len / char_size, char_size);

			// 面の数
			read_from_istream(in, &material.vertex_number);

		}

		return result;

	}

	// ボーンの読み込み
	template<template<typename>typename Container, typename Str, typename Vec3, template<typename> typename IKContainer, typename BoneIndex = std::int32_t, typename HeaderDataType = std::uint8_t,
		typename ContianerSizeType = std::size_t, typename StrSizeType = std::size_t, typename IKContainerSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_bone<Str, Vec3, IKContainer, BoneIndex>>, ContianerSizeType>,
		typename StrTraits = count_construct_container_traits<Str, StrSizeType>,
		typename IKContainerTraits = count_construct_container_traits<IKContainer<ik_link<Vec3, BoneIndex>>, IKContainerSizeType>>
		Container<pmx_bone<Str, Vec3, IKContainer, BoneIndex>> load_bone(std::istream& in, encode_type encode, HeaderDataType bone_index_size)
	{
		// ボーンの数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ指定し構築
		auto result = ContainerTraits::construct(num);

		// 文字の大きさ
		auto const char_size = static_cast<StrTraits::size_type>(encode);

		// それぞれのボーンの読み込み
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 読み込むボーン
			auto& bone = ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i));

			// 文字列を読み込む際に使用
			std::int32_t len;

			// 名前
			read_from_istream(in, &len);
			bone.name = StrTraits::construct(static_cast<StrTraits::size_type>(len / char_size));
			read_array_from_istream<StrTraits>(in, &bone.name, len / char_size, char_size);

			// 英語の名前
			read_from_istream(in, &len);
			bone.english_name = StrTraits::construct(static_cast<StrTraits::size_type>(len / char_size));
			read_array_from_istream<StrTraits>(in, &bone.english_name, len / char_size, char_size);

			// 位置
			read_vec3_from_istream(in, &bone.position);

			// 親ボーン
			read_intanger_from_istream(in, &bone.parent_index, bone_index_size);

			// 変形階層
			read_from_istream(in, &bone.trannsformation_level);

			// ボーンフラグ
			std::uint16_t bone_flag{};
			read_from_istream(in, &bone_flag);
			bone.bone_flag_bits = { bone_flag };

			// 接続
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::access_point)])
			{
				read_intanger_from_istream(in, &bone.access_point_index, bone_index_size);
			}
			else
			{
				read_vec3_from_istream(in, &bone.access_point_offset);
			}

			// 回転付与または移動付与
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::rotation_grant)] ||
				bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::move_grant)])
			{
				read_intanger_from_istream(in, &bone.grant_index, bone_index_size);
				read_from_istream(in, &bone.grant_rate);
			}

			// 軸固定
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::fix_axis)])
			{
				read_vec3_from_istream(in, &bone.fix_axis_direction);
			}

			// ローカル軸
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::local_axis)])
			{
				read_vec3_from_istream(in, &bone.local_axis_x);
				read_vec3_from_istream(in, &bone.local_axis_z);
			}

			// 外部親変形
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::external_parent_deformation)])
			{
				read_from_istream(in, &bone.external_parent_deformation_key);
			}

			// IK
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::ik)])
			{
				read_intanger_from_istream(in, &bone.ik_target_bone, bone_index_size);
				read_from_istream(in, &bone.ik_roop_number);
				read_from_istream(in, &bone.ik_rook_angle);

				// IK数の取得
				std::int32_t ik_num;
				read_from_istream(in, &ik_num);

				// コンテナの大きさ指定
				bone.ik_link = IKContainerTraits::construct(ik_num);

				for (std::size_t j = 0; j < ik_num; j++)
				{
					read_intanger_from_istream(in, &bone.ik_link[j].bone, bone_index_size);

					// 角度制限
					std::uint8_t is_angle_limit;
					read_from_istream(in, &is_angle_limit);

					if (is_angle_limit == 1)
					{
						// 下限
						Vec3 min;
						read_vec3_from_istream(in, &min);

						// 上限
						Vec3 max;
						read_vec3_from_istream(in, &max);

						bone.ik_link[j].min_max_angle_limit = { std::move(min),std::move(max) };
					}
				}
			}
		}

		return result;
	}
}