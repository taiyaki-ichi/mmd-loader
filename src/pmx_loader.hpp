#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"

namespace mmdl
{
	// ヘッダの読み込み
	// pmx2.0以降はDataのサイズは1byteなのでデフォルトはuint8_t
	template<typename Data = std::uint8_t>
	inline pmx_header<Data> load_header(std::istream& in)
	{
		pmx_header<Data> result;

		// 最初の4文字はいらない「Pmx 」の文字
		in.seekg(4);

		read_from_istream(in, &result.version);

		// 以降のデータの大きさの読み込み
		std::uint8_t size;
		read_from_istream(in, &size);
		// bitからbyteへ変換
		// WARNING: ここだけbitっぽい?
		size /= 8;

		Data encode;
		read_intanger<Data>(in, &encode, size);
		result.encode = encode == 0 ? encode_type::utf16 : encode_type::utf8;
		read_intanger(in, &result.add_uv_number, size);

		read_intanger(in, &result.vertex_index_size, size);
		read_intanger(in, &result.texture_index_size, size);
		read_intanger(in, &result.material_index_size, size);
		read_intanger(in, &result.bone_index_size, size);
		read_intanger(in, &result.morph_index_size, size);
		read_intanger(in, &result.rigid_body_index_size, size);

		return result;
	}

	template<typename Str>
	inline pmx_info<Str> load_info(std::istream& in)
	{
		pmx_info<Str> result;

		std::int32_t len;

		// モデル名の取得
		read_from_istream(in, &len);
		result.model_name.resize(len);
		read_from_istream(in, &result.model_name[0], len);

		// モデルの英語名の取得
		read_from_istream(in, &len);
		result.model_name.resize(len);
		read_from_istream(in, &result.english_mode_name[0], len);

		// コメントの取得
		read_from_istream(in, &len);
		result.comment.resize(len);
		read_from_istream(in, &result.comment[0], len);

		// 英語のコメントの取得
		read_from_istream(in, &len);
		result.english_comment.resize(len);
		read_from_istream(in, &result.english_comment[0], len);

		return result;
	}
}