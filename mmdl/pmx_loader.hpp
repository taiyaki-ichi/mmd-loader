#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>
#include"pmx_buffer.hpp"

namespace mmdl
{
	
	// ヘッダの読み込み
	// pmx2.0以降はDataのサイズは1byteなのでデフォルトはuint8_t
	template<typename T, typename traits = pmx_header_traits<T>>
	T load_header(std::istream& in)
	{
		pmx_header_buffer buffer{};

		// 最初の4文字はいらない「Pmx 」の文字
		in.seekg(4);

		read_from_istream(in, &buffer.version);

		// 以降のデータの大きさの読み込み
		// 「PMX2.0は 8 で固定」つまり、bit単位?
		std::uint8_t size;
		read_from_istream(in, &size);
		// bitからbyteへ変換
		size /= 8;

		read_intanger_from_istream(in, &buffer.encode, size);
		read_intanger_from_istream(in, &buffer.additional_uv_num, size);

		read_intanger_from_istream(in, &buffer.vertex_index_size, size);
		read_intanger_from_istream(in, &buffer.texture_index_size, size);
		read_intanger_from_istream(in, &buffer.material_index_size, size);
		read_intanger_from_istream(in, &buffer.bone_index_size, size);
		read_intanger_from_istream(in, &buffer.morph_index_size, size);
		read_intanger_from_istream(in, &buffer.rigid_body_index_size, size);

		return traits::construct(buffer);
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
			traits::emplace_back(result, texture_path_buffer, 
				texture_path_size % 2 == 0 ? texture_path_size / sizeof(char_type) : (texture_path_size + 1) / sizeof(char_type));

			// 初期化しておく
			std::fill(std::begin(texture_path_buffer), std::end(texture_path_buffer), 0);
		}

		return result;
	}

	// マテリアルの読み込み
	template<typename T, typename traits = pmx_material_traits<T>, std::size_t BufferSize = 64>
	T load_material(std::istream& in, std::size_t texture_index_size)
	{
		using char_type = traits::char_type;

		// マテリアルの数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ指定し構築
		auto result = traits::construct(static_cast<std::size_t>(num));


		std::int32_t name_size{};
		char_type name[BufferSize]{};
		std::int32_t english_name_size{};
		char_type english_name[BufferSize]{};
		std::array<float, 4> diffuse{};
		std::array<float, 3> specular{};
		float specularity{};
		std::array<float, 3> ambient{};
		std::uint8_t bit_flag{};
		std::array<float, 4> edge_color{};
		float edge_size{};
		std::size_t texture_index{};
		std::size_t sphere_texture_index{};
		std::uint8_t sphere_mode{};
		std::uint8_t toon_flag{};
		std::size_t toon_index{};
		std::int32_t memo_size{};
		char_type memo[BufferSize]{};
		std::int32_t vertex_num{};

		// それぞれのマテリアルの読み込み
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 名前
			read_from_istream(in, &name_size);
			read_from_istream(in, name, name_size);

			// 英語の名前
			read_from_istream(in, &english_name_size);
			read_from_istream(in, english_name, english_name_size);

			// 色情報
			read_from_istream(in, &diffuse);
			read_from_istream(in, &specular);
			read_from_istream(in, &specularity);
			read_from_istream(in, &ambient);

			// 描画フラグ
			read_from_istream(in, &bit_flag);

			// エッジ
			read_from_istream(in, &edge_color);
			read_from_istream(in, &edge_size);

			// テクスチャ
			read_intanger_from_istream(in, &texture_index, texture_index_size);
			read_intanger_from_istream(in, &sphere_texture_index, texture_index_size);

			// スフィアモード
			read_from_istream(in, &sphere_mode);

			// トゥーン
			read_from_istream(in, &toon_flag);

			// 個別トゥーンの場合
			if (toon_flag == 0) {
				read_intanger_from_istream(in, &toon_index, texture_index_size);
			}
			//共有トゥーンの場合
			else {
				read_intanger_from_istream(in, &toon_index, 1);
			}

			// メモ
			read_from_istream(in, &memo_size);
			read_from_istream(in, memo, memo_size);

			// 面の数
			read_from_istream(in, &vertex_num);

			// 要素を追加
			traits::emplace_back(result, name, name_size % 2 == 0 ? name_size / sizeof(char_type) : (name_size + 1) / sizeof(char_type),
				english_name, english_name_size % 2 == 0 ? english_name_size / sizeof(char_type) : (english_name_size + 1) / sizeof(char_type),
				diffuse, specular, specularity, ambient, bit_flag, edge_color, edge_size, texture_index, sphere_texture_index,
				sphere_mode, toon_flag, toon_index, memo, memo_size, vertex_num);


			// 初期化しておく
			std::fill(std::begin(name), std::end(name), 0);
			std::fill(std::begin(english_name), std::end(english_name), 0);
			std::fill(std::begin(memo), std::end(memo), 0);
		}

		return result;

	}

	// ボーンの読み込み
	template<typename T, typename traits = pmx_bone_traits<T>, std::size_t BufferNum = 64, std::size_t IKLinkBufferNum = 16>
	T load_bone(std::istream& in, std::size_t bone_index_size)
	{
		using char_type = traits::char_type;

		// ボーンの数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ指定し構築
		auto result = traits::construct(num);

		std::int32_t name_size{};
		char_type name[BufferNum]{};
		std::int32_t english_name_size{};
		char_type english_name[BufferNum]{};
		std::array<float, 3> position{};
		std::size_t parent_bone_index{};
		std::int32_t transformation_level{};
		std::uint16_t bone_flag{};
		std::array<float, 3> access_point_offset{};
		std::size_t access_bone_index{};
		std::size_t assign_bone_index{};
		float assign_rate{};
		std::array<float, 3> fixed_axis_direction{};
		std::array<float, 3> local_x_axis_direction{};
		std::array<float, 3> local_z_axis_direction{};
		std::int32_t key_value{};
		std::size_t ik_target_bone{};
		std::int32_t ik_loop_num{};
		float ik_angle_limit_par_process{};
		std::int32_t ik_link_size{};
		std::tuple<std::size_t, std::uint8_t, std::array<float, 3>, std::array<float, 3>> ik_link[IKLinkBufferNum]{};


		// それぞれのボーンの読み込み
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 名前
			read_from_istream(in, &name_size);
			read_from_istream(in, name, name_size);

			// 英語の名前
			read_from_istream(in, &english_name_size);
			read_from_istream(in, english_name, english_name_size);

			// 位置
			read_from_istream(in, &position);

			// 親ボーン
			read_from_istream(in, &parent_bone_index, bone_index_size);

			// 変形階層
			read_from_istream(in, &transformation_level);

			// ボーンフラグ
			read_from_istream(in, &bone_flag);

			// 接続先
			if ((bone_flag & 0x0001) == 0)
			{
				read_from_istream(in, &access_point_offset);
			}
			else
			{
				read_intanger_from_istream(in, &access_bone_index, bone_index_size);
			}

			// 回転付与または移動付与
			if ((bone_flag & 0x0100) != 0 || (bone_flag & 0x0200) != 0)
			{
				read_intanger_from_istream(in, &assign_bone_index, bone_index_size);
				read_from_istream(in, &assign_rate);
			}

			// 軸固定
			if ((bone_flag & 0x0400) != 0)
			{
				read_from_istream(in, &fixed_axis_direction);
			}

			// ローカル軸
			if ((bone_flag & 0x0800) != 0)
			{
				read_from_istream(in, &local_x_axis_direction);
				read_from_istream(in, &local_z_axis_direction);
			}

			// 外部親変形
			if ((bone_flag & 0x2000) != 0)
			{
				read_from_istream(in, &key_value);
			}

			// IK
			if ((bone_flag & 0x0020) != 0)
			{
				read_intanger_from_istream(in, &ik_target_bone, bone_index_size);
				read_from_istream(in, &ik_loop_num);
				read_from_istream(in, &ik_angle_limit_par_process);

				// IK数の取得
				read_from_istream(in, &ik_link_size);

				for (std::size_t j = 0; j < ik_link_size; j++)
				{
					auto& [ik_bone_index, ik_is_angle_limit, ik_angle_limit_bottom, ik_angle_limit_top] = ik_link[j];

					read_intanger_from_istream(in, &ik_bone_index, bone_index_size);

					// 角度制限
					read_from_istream(in, &ik_is_angle_limit);

					if (ik_is_angle_limit == 1)
					{
						// 下限
						read_from_istream(in, &ik_angle_limit_bottom);

						// 上限
						read_from_istream(in, &ik_angle_limit_top);
					}
				}
			}

			// 要素の追加
			traits::emplace_back(result,
				name, name_size % 2 == 0 ? name_size / sizeof(char_type) : (name_size + 1) / sizeof(char_type),
				english_name, english_name_size % 2 == 0 ? english_name_size / sizeof(char_type) : (english_name_size + 1) / sizeof(char_type),
				position,
				parent_bone_index,
				transformation_level,
				bone_flag,
				access_point_offset,
				access_bone_index,
				assign_bone_index,
				assign_rate,
				fixed_axis_direction,
				local_x_axis_direction,
				local_z_axis_direction,
				key_value,
				ik_target_bone,
				ik_loop_num,
				ik_angle_limit_par_process,
				ik_link_size,
				ik_link
			);

			// 初期化しておく
			std::fill(std::begin(name), std::end(name), 0);
			std::fill(std::begin(english_name), std::end(english_name), 0);
		}

		return result;
	}
}