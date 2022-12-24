#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"

namespace mmdl
{
	template<typename Data = std::int64_t>
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
}