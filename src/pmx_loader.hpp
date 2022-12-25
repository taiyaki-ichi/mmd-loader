#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>

namespace mmdl
{
	// �w�b�_�̓ǂݍ���
	// pmx2.0�ȍ~��Data�̃T�C�Y��1byte�Ȃ̂Ńf�t�H���g��uint8_t
	template<std::integral Data = std::uint8_t>
	pmx_header<Data> load_header(std::istream& in)
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

	template<typename Str>
	requires resizable_container<Str, decltype(0)>
	pmx_info<Str> load_info(std::istream& in)
	{
		pmx_info<Str> result;

		std::int32_t len;

		// ���f�����̎擾
		read_from_istream(in, &len);
		result.model_name.resize(len);
		read_from_istream(in, &result.model_name[0], len);

		// ���f���̉p�ꖼ�̎擾
		read_from_istream(in, &len);
		result.model_name.resize(len);
		read_from_istream(in, &result.english_mode_name[0], len);

		// �R�����g�̎擾
		read_from_istream(in, &len);
		result.comment.resize(len);
		read_from_istream(in, &result.comment[0], len);

		// �p��̃R�����g�̎擾
		read_from_istream(in, &len);
		result.english_comment.resize(len);
		read_from_istream(in, &result.english_comment[0], len);

		return result;
	}


	// ���_���̓ǂݍ���
	template<template<typename> typename Container, typename Vec2, typename Vec3, typename Vec4,
		typename BoneIndex = std::int32_t,typename  Data= std::uint8_t>
		requires resizable_container<Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>, BoneIndex> && resizable_container<Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>, std::int32_t>
		Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> load_vertex(std::istream& in, Data add_uv_number, Data bone_index_size)
	{
		Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> result;

		// ���_�̐����擾
		std::int32_t num;
		read_from_istream(in, &num);
		result.resize(num);

		// ���ꂼ��̒��_�̎擾
		for (std::size_t i = 0; i < result.size(); i++)
		{
			// TODO: �^��������鎞�ɏ��������@���l����
			read_from_istream(in, &result[i].position);
			read_from_istream(in, &result[i].normal);
			read_from_istream(in, &result[i].uv);

			// �ǉ�uv�̎擾
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				// TODO: �^��������鎞�ɏ��������@���l����
				read_from_istream(in, &result[i].additional_uv[j]);
			}

			// �E�F�C�g�ό`�����̎擾
			std::uint8_t weight_type;
			read_from_istream(in, &weight_type);

			switch (weight_type)
			{
			// BDEF1�̏ꍇ
			case 0:
				read_intanger(in, &result[i].bone[0], bone_index_size);
				// �P��̃{�[���̏d�݂�1�ł��邱�Ƃ�����
				result[i].weight[0] = 1.f;
				break;

			// BDEF2�̏ꍇ
			case 1:
				read_intanger(in, &result[i].bone[0], bone_index_size);
				read_intanger(in, &result[i].bone[1], bone_index_size);
				read_from_istream(in, &result[i].weight[0]);
				// 2�{�̃{�[���̏d�݂͍��v1�ɂȂ�
				result[i].weight[1] = 1.f - result[i].weight[0];
				break;

			// BDEF4�̏ꍇ
			case 2:
				// 4�̃{�[���̃C���f�b�N�X�̎擾
				for (std::size_t j = 0; j < 4; j++)
					read_intanger(in, &result[i].bone[j], bone_index_size);
				// 4�̃{�[���̏d�݂̎擾
				for (std::size_t j = 0; j < 4; j++)
					read_from_istream(in, &result[i].weight[j]);
				break;

			// SDEF�̔{
			case 3:
				read_intanger(in, &result[i].bone[0], bone_index_size);
				read_intanger(in, &result[i].bone[1], bone_index_size);
				read_from_istream(in, &result[i].weight[0]);
				// �����܂�BDEF2�Ɠ���
				std::array<Vec3, 3> sdef;
				for (std::size_t j = 0; j < 3; j++)
					read_from_istream(in, &sdef[j]);
				result[i].sdef = std::move(sdef);
				break;
			}

			// �G�b�W�{���̎擾
			read_from_istream(in, &result[i].edge_magnification);

		}

		return result;

	}

}