#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>

namespace mmdl
{
	// ヘッダの読み込み
	// pmx2.0以降はDataのサイズは1byteなのでデフォルトはuint8_t
	template<std::integral HeaderDataType = std::uint8_t>
	pmx_header<HeaderDataType> load_header(std::istream& in)
	{
		pmx_header<HeaderDataType> result;

		// 最初の4文字はいらない「Pmx 」の文字
		in.seekg(4);

		read_from_istream(in, &result.version);

		// 以降のデータの大きさの読み込み
		std::uint8_t size;
		read_from_istream(in, &size);
		// bitからbyteへ変換
		// WARNING: ここだけbitっぽい?
		size /= 8;

		HeaderDataType encode;
		read_intanger_from_istream<HeaderDataType>(in, &encode, size);
		result.encode = encode == 0 ? encode_type::utf16 : encode_type::utf8;
		read_intanger_from_istream(in, &result.add_uv_number, size);

		read_intanger_from_istream(in, &result.vertex_index_size, size);
		read_intanger_from_istream(in, &result.texture_index_size, size);
		read_intanger_from_istream(in, &result.material_index_size, size);
		read_intanger_from_istream(in, &result.bone_index_size, size);
		read_intanger_from_istream(in, &result.morph_index_size, size);
		read_intanger_from_istream(in, &result.rigid_body_index_size, size);

		return result;
	}

	template<typename Str, typename StrSizeType = std::size_t, typename StrTraits = count_construct_container_traits<Str, StrSizeType>>
	pmx_info<Str> load_info(std::istream& in, encode_type encode)
	{
		pmx_info<Str> result;

		std::int32_t len;
		auto const char_size = static_cast<std::int32_t>(encode);

		// モデル名の取得
		read_from_istream(in, &len);
		result.model_name = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.model_name, len / char_size, char_size);

		// モデルの英語名の取得
		read_from_istream(in, &len);
		result.english_mode_name = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.english_mode_name, len / char_size, char_size);

		// コメントの取得
		read_from_istream(in, &len);
		result.comment = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.comment, len / char_size, char_size);

		// 英語のコメントの取得
		read_from_istream(in, &len);
		result.english_comment = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.english_comment, len / char_size, char_size);

		return result;
	}


	// 頂点情報の読み込み
	template<template<typename> typename Container, constructible_vec2 Vec2, constructible_vec3 Vec3, constructible_vec4 Vec4,
		typename BoneIndex = std::int32_t, typename  HeaderDataType = std::uint8_t, typename ContainterSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>, ContainterSizeType>>
		Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> load_vertex(std::istream& in, HeaderDataType add_uv_number, HeaderDataType bone_index_size)
	{
		// 頂点の数を取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの生成
		auto result = ContainerTraits::construct(static_cast<ContainerTraits::size_type>(num));

		// それぞれの頂点の取得
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_vec3_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).position);
			read_vec3_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).normal);
			read_vec2_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).uv);

			// 追加uvの取得
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				read_vec4_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).additional_uv[j]);
			}

			// ウェイト変形方式の取得
			std::uint8_t weight_type;
			read_from_istream(in, &weight_type);

			switch (weight_type)
			{
				// BDEF1の場合
			case 0:
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[0], bone_index_size);
				// 単一のボーンの重みが1であることを示す
				ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0] = 1.f;
				break;

				// BDEF2の場合
			case 1:
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[0], bone_index_size);
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[1], bone_index_size);
				read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0]);
				// 2本のボーンの重みは合計1になる
				ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[1] = 1.f - ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0];
				break;

				// BDEF4の場合
			case 2:
				// 4つのボーンのインデックスの取得
				for (std::size_t j = 0; j < 4; j++)
					read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[j], bone_index_size);
				// 4つのボーンの重みの取得
				for (std::size_t j = 0; j < 4; j++)
					read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[j]);
				break;

				// SDEFの倍
			case 3:
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[0], bone_index_size);
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[1], bone_index_size);
				read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0]);
				// ここまでBDEF2と同じ
				std::array<Vec3, 3> sdef;
				for (std::size_t j = 0; j < 3; j++)
					read_from_istream(in, &sdef[j]);
				ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).sdef = std::move(sdef);
				break;
			}

			// エッジ倍率の取得
			read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).edge_magnification);

		}

		return result;

	}

	// 面情報の読み込み
	template<template<typename> typename Container, typename VertexIndex = std::int32_t, typename HeaderDataType = std::uint8_t, typename ContainerSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_surface<VertexIndex>>, ContainerSizeType>>
		Container<pmx_surface<VertexIndex>> load_surface(std::istream& in, HeaderDataType vertex_index_size)
	{
		// 面の数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ設定
		auto result = ContainerTraits::construct(static_cast<ContainerTraits::size_type>(num));

		// それぞれの情報を取得
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)), vertex_index_size);
		}

		return result;
	}

	// テクスチャパスの読み込み
	template<template<typename> typename Container, typename Str, typename ContainerSizeType = std::size_t, typename StrSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<Str>, ContainerSizeType>, typename StrTraits = count_construct_container_traits<Str, ContainerSizeType>>
		Container<Str> load_texture_path(std::istream& in, encode_type encode)
	{
		// コンテナのサイズの指定
		std::int32_t num;
		read_from_istream(in, &num);
		auto result = ContainerTraits::construct(num);

		// 文字の大きさ
		auto const char_size = static_cast<StrTraits::size_type>(encode);

		// 1文字列ごとに取得していく
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 長さの取得
			std::int32_t len;
			read_from_istream(in, &len);

			// 文字列の読み込み
			auto str = StrTraits::construct(static_cast<StrTraits::size_type>(len / char_size));
			read_array_from_istream<StrTraits>(in, &str, len / char_size, char_size);

			// 文字列を格納
			ContainerTraits::get_reference(result, static_cast<StrTraits::size_type>(i)) = std::move(str);
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