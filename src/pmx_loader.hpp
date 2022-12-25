#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>

namespace mmdl
{
	// ヘッダの読み込み
	// pmx2.0以降はDataのサイズは1byteなのでデフォルトはuint8_t
	template<std::integral HeaderData = std::uint8_t>
	pmx_header<HeaderData> load_header(std::istream& in)
	{
		pmx_header<HeaderData> result;

		// 最初の4文字はいらない「Pmx 」の文字
		in.seekg(4);

		read_from_istream(in, &result.version);

		// 以降のデータの大きさの読み込み
		std::uint8_t size;
		read_from_istream(in, &size);
		// bitからbyteへ変換
		// WARNING: ここだけbitっぽい?
		size /= 8;

		HeaderData encode;
		read_intanger_from_istream<HeaderData>(in, &encode, size);
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

	template<typename Str>
	requires resizable_container<Str, decltype(0)>
		pmx_info<Str> load_info(std::istream& in, encode_type encode)
	{
		pmx_info<Str> result;

		std::int32_t len;
		std::int32_t char_size = static_cast<std::int32_t>(encode);

		// モデル名の取得
		read_from_istream(in, &len);
		result.model_name.resize(len / char_size);
		read_str_from_istream(in, &result.model_name, len / char_size, char_size);

		// モデルの英語名の取得
		read_from_istream(in, &len);
		result.english_mode_name.resize(len / char_size);
		read_str_from_istream(in, &result.english_mode_name, len / char_size, char_size);

		// コメントの取得
		read_from_istream(in, &len);
		result.comment.resize(len / char_size);
		read_str_from_istream(in, &result.comment, len / char_size, char_size);

		// 英語のコメントの取得
		read_from_istream(in, &len);
		result.english_comment.resize(len / char_size);
		read_str_from_istream(in, &result.english_comment, len / char_size, char_size);

		return result;
	}


	// 頂点情報の読み込み
	template<template<typename> typename Container, constructible_vec2 Vec2, constructible_vec3 Vec3, constructible_vec4 Vec4, typename BoneIndex = std::int32_t, typename  HeaderData = std::uint8_t>
	requires resizable_container<Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>, BoneIndex>&& resizable_container<Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>, std::int32_t>
		Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> load_vertex(std::istream& in, HeaderData add_uv_number, HeaderData bone_index_size)
	{
		Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> result;

		// 頂点の数を取得
		std::int32_t num;
		read_from_istream(in, &num);
		result.resize(num);

		// それぞれの頂点の取得
		for (std::size_t i = 0; i < result.size(); i++)
		{
			read_vec3_from_istream(in, &result[i].position);
			read_vec3_from_istream(in, &result[i].normal);
			read_vec2_from_istream(in, &result[i].uv);

			// 追加uvの取得
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				read_vec4_from_istream(in, &result[i].additional_uv[j]);
			}

			// ウェイト変形方式の取得
			std::uint8_t weight_type;
			read_from_istream(in, &weight_type);

			switch (weight_type)
			{
				// BDEF1の場合
			case 0:
				read_intanger_from_istream(in, &result[i].bone[0], bone_index_size);
				// 単一のボーンの重みが1であることを示す
				result[i].weight[0] = 1.f;
				break;

				// BDEF2の場合
			case 1:
				read_intanger_from_istream(in, &result[i].bone[0], bone_index_size);
				read_intanger_from_istream(in, &result[i].bone[1], bone_index_size);
				read_from_istream(in, &result[i].weight[0]);
				// 2本のボーンの重みは合計1になる
				result[i].weight[1] = 1.f - result[i].weight[0];
				break;

				// BDEF4の場合
			case 2:
				// 4つのボーンのインデックスの取得
				for (std::size_t j = 0; j < 4; j++)
					read_intanger_from_istream(in, &result[i].bone[j], bone_index_size);
				// 4つのボーンの重みの取得
				for (std::size_t j = 0; j < 4; j++)
					read_from_istream(in, &result[i].weight[j]);
				break;

				// SDEFの倍
			case 3:
				read_intanger_from_istream(in, &result[i].bone[0], bone_index_size);
				read_intanger_from_istream(in, &result[i].bone[1], bone_index_size);
				read_from_istream(in, &result[i].weight[0]);
				// ここまでBDEF2と同じ
				std::array<Vec3, 3> sdef;
				for (std::size_t j = 0; j < 3; j++)
					read_from_istream(in, &sdef[j]);
				result[i].sdef = std::move(sdef);
				break;
			}

			// エッジ倍率の取得
			read_from_istream(in, &result[i].edge_magnification);

		}

		return result;

	}

}