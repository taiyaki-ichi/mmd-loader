#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"

namespace mmdl
{
	// �w�b�_�̓ǂݍ���
	// pmx2.0�ȍ~��Data�̃T�C�Y��1byte�Ȃ̂Ńf�t�H���g��uint8_t
	template<typename Data = std::uint8_t>
	inline pmx_header<Data> load_header(std::istream& in)
	{
		pmx_header<Data> result;

		// �ŏ���4�����͂���Ȃ��uPmx �v�̕���
		in.seekg(4);

		read_from_istream(in, &result.version);

		// �ȍ~�̃f�[�^�̑傫���̓ǂݍ���
		std::uint8_t size;
		read_from_istream(in, &size);
		// bit����byte�֕ϊ�
		// WARNING: ��������bit���ۂ�?
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