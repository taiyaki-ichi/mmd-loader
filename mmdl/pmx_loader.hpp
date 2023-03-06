#pragma once
#include"pmx_buffer.hpp"
#include"pmx_loader_traits.hpp"
#include"utility.hpp"

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

	template<typename T, typename traits = pmx_info_traits<T>, std::size_t CharBufferSize = 512>
	T load_info(std::istream& in)
	{
		using char_type = typename traits::char_type;

		pmx_info_buffer<char_type, CharBufferSize> buffer{};

		// モデル名の取得
		read_from_istream(in, &buffer.model_name_size);
		read_from_istream(in, &buffer.model_name[0], buffer.model_name_size);

		// モデルの英語名の取得
		read_from_istream(in, &buffer.english_model_name_size);
		read_from_istream(in, &buffer.english_model_name[0], buffer.english_model_name_size);

		// コメントの取得
		read_from_istream(in, &buffer.comment_size);
		read_from_istream(in, &buffer.comment[0], buffer.comment_size);

		// 英語のコメントの取得
		read_from_istream(in, &buffer.english_comment_size);
		read_from_istream(in, &buffer.english_comment[0], buffer.english_comment_size);

		return traits::construct(buffer);
	}


	// 頂点情報の読み込み
	template<typename T, typename traits = pmx_vertex_traits<T>>
	T load_vertex(std::istream& in, std::size_t add_uv_number, std::size_t bone_index_size)
	{
		// 頂点の数を取得
		std::int32_t num;
		read_from_istream(in, &num);

		auto result = traits::construct(static_cast<std::size_t>(num));

		pmx_vertex_buffer buffer{};

		// それぞれの頂点の取得
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_from_istream(in, &buffer.position);
			read_from_istream(in, &buffer.normal);
			read_from_istream(in, &buffer.uv);

			// 追加uvの取得
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				read_from_istream(in, &buffer.additional_uv[i]);
			}

			// ウェイト変形方式の取得
			read_from_istream(in, &buffer.weight_type);

			switch (buffer.weight_type)
			{
				// BDEF1の場合
			case 0:
				read_intanger_from_istream(in, &buffer.bone_index[0], bone_index_size);

				break;

				// BDEF2の場合
			case 1:
				read_intanger_from_istream(in, &buffer.bone_index[0], bone_index_size);
				read_intanger_from_istream(in, &buffer.bone_index[1], bone_index_size);
				read_from_istream(in, &buffer.bone_weight[0]);

				break;

				// BDEF4の場合
			case 2:
				read_intanger_from_istream(in, &buffer.bone_index[0], bone_index_size);
				read_intanger_from_istream(in, &buffer.bone_index[1], bone_index_size);
				read_intanger_from_istream(in, &buffer.bone_index[2], bone_index_size);
				read_intanger_from_istream(in, &buffer.bone_index[3], bone_index_size);

				read_from_istream(in, &buffer.bone_weight[0]);
				read_from_istream(in, &buffer.bone_weight[1]);
				read_from_istream(in, &buffer.bone_weight[2]);
				read_from_istream(in, &buffer.bone_weight[3]);

				break;

				// SDEFの倍
			case 3:
				read_intanger_from_istream(in, &buffer.bone_index[0], bone_index_size);
				read_intanger_from_istream(in, &buffer.bone_index[1], bone_index_size);
				read_from_istream(in, &buffer.bone_weight[0]);
				// ここまでBDEF2と同じ

				read_from_istream(in, &buffer.sdef_c);
				read_from_istream(in, &buffer.sdef_r0);
				read_from_istream(in, &buffer.sdef_r1);

				break;
			}

			read_from_istream(in, &buffer.edge_factor);

			traits::emplace_back(result, buffer, add_uv_number);

			// 一応、初期化しておく
			buffer.bone_index = {};
			buffer.bone_weight = {};
			buffer.sdef_c = {};
			buffer.sdef_r0 = {};
			buffer.sdef_r1 = {};
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
	template<typename T, typename traits = pmx_texture_path_traits<T>, std::size_t CharBufferSize = 64>
	T load_texture_path(std::istream& in)
	{
		using char_type = traits::char_type;

		// コンテナのサイズの指定
		std::int32_t num;
		read_from_istream(in, &num);
		auto result = traits::construct(static_cast<std::size_t>(num));

		std::int32_t texture_path_size{};
		std::array<char_type, CharBufferSize> texture_path_buffer{};

		// 1文字列ごとに取得していく
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 長さの取得
			read_from_istream(in, &texture_path_size);

			// 文字列の読み込み
			read_from_istream(in, &texture_path_buffer[0], texture_path_size);

			// 文字列を格納
			traits::emplace_back(result,
				texture_path_size % 2 == 0 ? texture_path_size / sizeof(char_type) : (texture_path_size + 1) / sizeof(char_type), texture_path_buffer);

			// 初期化しておく
			std::fill(std::begin(texture_path_buffer), std::end(texture_path_buffer), 0);
		}

		return result;
	}

	// マテリアルの読み込み
	template<typename T, typename traits = pmx_material_traits<T>, std::size_t CharBufferSize = 64>
	T load_material(std::istream& in, std::size_t texture_index_size)
	{
		using char_type = traits::char_type;

		// マテリアルの数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ指定し構築
		auto result = traits::construct(static_cast<std::size_t>(num));

		pmx_material_buffer<char_type, CharBufferSize> buffer{};

		// それぞれのマテリアルの読み込み
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 名前
			read_from_istream(in, &buffer.name_size);
			read_from_istream(in, &buffer.name[0], buffer.name_size);

			// 英語の名前
			read_from_istream(in, &buffer.english_name_size);
			read_from_istream(in, &buffer.english_name[0], buffer.english_name_size);

			// 色情報
			read_from_istream(in, &buffer.diffuse);
			read_from_istream(in, &buffer.specular);
			read_from_istream(in, &buffer.specularity);
			read_from_istream(in, &buffer.ambient);

			// 描画フラグ
			read_from_istream(in, &buffer.bit_flag);

			// エッジ
			read_from_istream(in, &buffer.edge_color);
			read_from_istream(in, &buffer.edge_size);

			// テクスチャ
			read_intanger_from_istream(in, &buffer.texture_index, texture_index_size);
			read_intanger_from_istream(in, &buffer.sphere_texture_index, texture_index_size);

			// スフィアモード
			read_from_istream(in, &buffer.sphere_mode);

			// トゥーン
			read_from_istream(in, &buffer.toon_flag);

			// 個別トゥーンの場合
			if (buffer.toon_flag == 0) {
				read_intanger_from_istream(in, &buffer.toon_index, texture_index_size);
			}
			//共有トゥーンの場合
			else {
				read_intanger_from_istream(in, &buffer.toon_index, 1);
			}

			// メモ
			read_from_istream(in, &buffer.memo_size);
			read_from_istream(in, &buffer.memo[0], buffer.memo_size);

			// 面の数
			read_from_istream(in, &buffer.vertex_num);

			// 要素を追加
			traits::emplace_back(result, buffer);


			// 初期化しておく
			std::fill(std::begin(buffer.name), std::end(buffer.name), 0);
			std::fill(std::begin(buffer.english_name), std::end(buffer.english_name), 0);
			std::fill(std::begin(buffer.memo), std::end(buffer.memo), 0);
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

		pmx_bone_buffer<char_type, BufferNum, IKLinkBufferNum> buffer{};

		// それぞれのボーンの読み込み
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 名前
			read_from_istream(in, &buffer.name_size);
			read_from_istream(in, &buffer.name[0], buffer.name_size);
			// 大きさから要素数へ変更
			buffer.name_size = buffer.name_size % 2 == 0 ? buffer.name_size / sizeof(char_type) : (buffer.name_size + 1) / sizeof(char_type);

			// 英語の名前
			read_from_istream(in, &buffer.english_name_size);
			read_from_istream(in, &buffer.english_name[0], buffer.english_name_size);
			// 大きさ可rあ要素数へ変更
			buffer.english_name_size = buffer.english_name_size % 2 == 0 ? buffer.english_name_size / sizeof(char_type) : (buffer.english_name_size + 1) / sizeof(char_type);

			// 位置
			read_from_istream(in, &buffer.position);

			// 親ボーン
			read_from_istream(in, &buffer.parent_bone_index, bone_index_size);

			// 変形階層
			read_from_istream(in, &buffer.transformation_level);

			// ボーンフラグ
			read_from_istream(in, &buffer.bone_flag);

			// 接続先
			if ((buffer.bone_flag & 0x0001) == 0)
			{
				read_from_istream(in, &buffer.access_point_offset);
			}
			else
			{
				read_intanger_from_istream(in, &buffer.access_bone_index, bone_index_size);
			}

			// 回転付与または移動付与
			if ((buffer.bone_flag & 0x0100) != 0 || (buffer.bone_flag & 0x0200) != 0)
			{
				read_intanger_from_istream(in, &buffer.assign_bone_index, bone_index_size);
				read_from_istream(in, &buffer.assign_rate);
			}

			// 軸固定
			if ((buffer.bone_flag & 0x0400) != 0)
			{
				read_from_istream(in, &buffer.fixed_axis_direction);
			}

			// ローカル軸
			if ((buffer.bone_flag & 0x0800) != 0)
			{
				read_from_istream(in, &buffer.local_x_axis_direction);
				read_from_istream(in, &buffer.local_z_axis_direction);
			}

			// 外部親変形
			if ((buffer.bone_flag & 0x2000) != 0)
			{
				read_from_istream(in, &buffer.key_value);
			}

			// IK
			if ((buffer.bone_flag & 0x0020) != 0)
			{
				read_intanger_from_istream(in, &buffer.ik_target_bone, bone_index_size);
				read_from_istream(in, &buffer.ik_loop_num);
				read_from_istream(in, &buffer.ik_angle_limit_par_process);

				// IK数の取得
				read_from_istream(in, &buffer.ik_link_size);

				for (std::size_t j = 0; j < buffer.ik_link_size; j++)
				{
					auto& [ik_bone_index, ik_is_angle_limit, ik_angle_limit_bottom, ik_angle_limit_top] = buffer.ik_link[j];

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
			traits::emplace_back(result, buffer);

			// 一応、初期化しておく
			std::fill(std::begin(buffer.name), std::end(buffer.name), 0);
			std::fill(std::begin(buffer.english_name), std::end(buffer.english_name), 0);
			buffer.ik_link = {};
		}

		return result;
	}

	// モーフの読み込み
	template<typename T, typename traits = pmx_morph_traits<T>, std::size_t CharBufferSize = 64, std::size_t MorphDataSize = 2048>
	T load_morph(std::istream& in, std::size_t vertex_index_size, std::size_t bone_index_size, std::size_t material_index_size, std::size_t morph_index_size)
	{
		using char_type = traits::char_type;

		// モーフの数の取得
		std::int32_t num;
		read_from_istream(in, &num);

		// コンテナの大きさ指定し構築
		auto result = traits::construct(num);

		std::int32_t morph_element_num{};
		pmx_morph_buffer<char_type, CharBufferSize, MorphDataSize> buffer{};

		pmx_vertex_morph_buffer vertex_morph{};
		pmx_uv_morph_buffer uv_morph{};
		pmx_bone_morph_buffer bone_morph{};
		pmx_material_morph_buffer material_morph{};
		pmx_group_morph_buffer group_morph{};

		// それぞれのモーフの読み込み
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// 名前
			read_from_istream(in, &buffer.name_size);
			read_from_istream(in, &buffer.name[0], buffer.name_size);
			// 大きさから要素数へ変更
			buffer.name_size = buffer.name_size % 2 == 0 ? buffer.name_size / sizeof(char_type) : (buffer.name_size + 1) / sizeof(char_type);

			// 英語の名前
			read_from_istream(in, &buffer.english_name_size);
			read_from_istream(in, &buffer.english_name[0], buffer.english_name_size);
			// 大きさから要素数へ変更
			buffer.english_name_size = buffer.english_name_size % 2 == 0 ? buffer.english_name_size / sizeof(char_type) : (buffer.english_name_size + 1) / sizeof(char_type);

			read_from_istream(in, &buffer.control_panel_option);

			read_from_istream(in, &buffer.morph_type);

			read_from_istream(in, &buffer.morph_data_num);

			switch (buffer.morph_type)
			{
				// グループ
			case 0:
				
				for (std::size_t j = 0; j < static_cast<std::size_t>(buffer.morph_data_num); j++)
				{
					read_intanger_from_istream(in, &group_morph.index, morph_index_size);
					read_from_istream(in, &group_morph.morph_factor);

					buffer.morph_data[j] = group_morph;
				}

				break;


				// 頂点
			case 1:
				for (std::size_t j = 0; j < static_cast<std::size_t>(buffer.morph_data_num); j++)
				{
					read_intanger_from_istream(in, &vertex_morph.index, vertex_index_size);
					read_from_istream(in, &vertex_morph.offset);

					buffer.morph_data[j] = vertex_morph;
				}

				break;


				// ボーン
			case 2:
				for (std::size_t j = 0; j < static_cast<std::size_t>(buffer.morph_data_num); j++)
				{
					read_intanger_from_istream(in, &bone_morph.index, bone_index_size);
					read_from_istream(in, &bone_morph.transform);
					read_from_istream(in, &bone_morph.quaternion);

					buffer.morph_data[j] = bone_morph;
				}

				break;


				// uv
			case 3:
				// 追加uv1-4
			case 4:
			case 5:
			case 6:
			case 7:
				for (std::size_t j = 0; j < static_cast<std::size_t>(buffer.morph_data_num); j++)
				{
					read_intanger_from_istream(in, &uv_morph.index, vertex_index_size);
					read_from_istream(in, &uv_morph.offset);

					buffer.morph_data[j] = uv_morph;
				}

				break;


				// マテリアル
			case 8:
				for (std::size_t j = 0; j < static_cast<std::size_t>(buffer.morph_data_num); j++)
				{
					read_intanger_from_istream(in, &material_morph.index, material_index_size);
					read_from_istream(in, &material_morph.offset_type);
					read_from_istream(in, &material_morph.diffuse);
					read_from_istream(in, &material_morph.specular);
					read_from_istream(in, &material_morph.specularity);
					read_from_istream(in, &material_morph.ambient);
					read_from_istream(in, &material_morph.edge_color);
					read_from_istream(in, &material_morph.edge_size);
					read_from_istream(in, &material_morph.texture_factor);
					read_from_istream(in, &material_morph.sphere_texture_factor);
					read_from_istream(in, &material_morph.toon_texture_factor);

					buffer.morph_data[j] = material_morph;
				}

				break;
			}

			traits::emplace_back(result, buffer);


			std::fill(buffer.name.begin(), buffer.name.end(), 0);
			std::fill(buffer.english_name.begin(), buffer.english_name.end(), 0);

		}

		return result;
	}
}