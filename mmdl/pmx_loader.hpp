#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>

namespace mmdl
{
	// �w�b�_�̓ǂݍ���
	// pmx2.0�ȍ~��Data�̃T�C�Y��1byte�Ȃ̂Ńf�t�H���g��uint8_t
	template<std::integral HeaderData = std::uint8_t>
	pmx_header<HeaderData> load_header(std::istream& in)
	{
		pmx_header<HeaderData> result;

		// �ŏ���4�����͂���Ȃ��uPmx �v�̕���
		in.seekg(4);

		read_from_istream(in, &result.version);

		// �ȍ~�̃f�[�^�̑傫���̓ǂݍ���
		std::uint8_t size;
		read_from_istream(in, &size);
		// bit����byte�֕ϊ�
		// WARNING: ��������bit���ۂ�?
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

	template<typename Str, typename StrSizeType = std::size_t>
		requires resizable_container<Str, StrSizeType>
	pmx_info<Str> load_info(std::istream& in, encode_type encode)
	{
		pmx_info<Str> result;

		std::int32_t len;
		std::int32_t char_size = static_cast<std::int32_t>(encode);

		using str_traits = resizable_container_traits<Str, StrSizeType>;

		// ���f�����̎擾
		read_from_istream(in, &len);
		str_traits::resize(result.model_name, static_cast<str_traits::size_type>(len / char_size));
		read_array_from_istream(in, &result.model_name, len / char_size, char_size);

		// ���f���̉p�ꖼ�̎擾
		read_from_istream(in, &len);
		str_traits::resize(result.english_mode_name, static_cast<str_traits::size_type>(len / char_size));
		read_array_from_istream(in, &result.english_mode_name, len / char_size, char_size);

		// �R�����g�̎擾
		read_from_istream(in, &len);
		str_traits::resize(result.comment, static_cast<str_traits::size_type>(len / char_size));
		read_array_from_istream(in, &result.comment, len / char_size, char_size);

		// �p��̃R�����g�̎擾
		read_from_istream(in, &len);
		str_traits::resize(result.english_comment, static_cast<str_traits::size_type>(len / char_size));
		read_array_from_istream(in, &result.english_comment, len / char_size, char_size);

		return result;
	}


	// ���_���̓ǂݍ���
	template<template<typename> typename Container, constructible_vec2 Vec2, constructible_vec3 Vec3, constructible_vec4 Vec4,
		typename BoneIndex = std::int32_t, typename  HeaderData = std::uint8_t, typename ContainterSizeType = std::size_t>
		requires resizable_container<Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>, std::size_t>
	Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> load_vertex(std::istream& in, HeaderData add_uv_number, HeaderData bone_index_size)
	{
		using result_type = Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>;
		using container_traits = resizable_container_traits<result_type, ContainterSizeType>;
		using size_type = container_traits::size_type;

		result_type result;

		// ���_�̐����擾
		std::int32_t num;
		read_from_istream(in, &num);
		container_traits::resize(result, static_cast<size_type>(num));

		// ���ꂼ��̒��_�̎擾
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_vec3_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).position);
			read_vec3_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).normal);
			read_vec2_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).uv);

			// �ǉ�uv�̎擾
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				read_vec4_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).additional_uv[j]);
			}

			// �E�F�C�g�ό`�����̎擾
			std::uint8_t weight_type;
			read_from_istream(in, &weight_type);

			switch (weight_type)
			{
				// BDEF1�̏ꍇ
			case 0:
				read_intanger_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).bone[0], bone_index_size);
				// �P��̃{�[���̏d�݂�1�ł��邱�Ƃ�����
				container_traits::get(result, static_cast<size_type>(i)).weight[0] = 1.f;
				break;

				// BDEF2�̏ꍇ
			case 1:
				read_intanger_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).bone[0], bone_index_size);
				read_intanger_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).bone[1], bone_index_size);
				read_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).weight[0]);
				// 2�{�̃{�[���̏d�݂͍��v1�ɂȂ�
				container_traits::get(result, static_cast<size_type>(i)).weight[1] = 1.f - container_traits::get(result, static_cast<size_type>(i)).weight[0];
				break;

				// BDEF4�̏ꍇ
			case 2:
				// 4�̃{�[���̃C���f�b�N�X�̎擾
				for (std::size_t j = 0; j < 4; j++)
					read_intanger_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).bone[j], bone_index_size);
				// 4�̃{�[���̏d�݂̎擾
				for (std::size_t j = 0; j < 4; j++)
					read_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).weight[j]);
				break;

				// SDEF�̔{
			case 3:
				read_intanger_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).bone[0], bone_index_size);
				read_intanger_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).bone[1], bone_index_size);
				read_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).weight[0]);
				// �����܂�BDEF2�Ɠ���
				std::array<Vec3, 3> sdef;
				for (std::size_t j = 0; j < 3; j++)
					read_from_istream(in, &sdef[j]);
				container_traits::get(result, static_cast<size_type>(i)).sdef = std::move(sdef);
				break;
			}

			// �G�b�W�{���̎擾
			read_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)).edge_magnification);

		}

		return result;

	}

	// �ʏ��̓ǂݍ���
	template<template<typename> typename Container, typename VertexIndex = std::int32_t, typename HeaderData = std::uint8_t, typename ContainerSizeType = std::size_t>
		requires resizable_container<Container<pmx_surface<VertexIndex>>, ContainerSizeType>
	Container<pmx_surface<VertexIndex>> load_surface(std::istream& in, HeaderData vertex_index_size)
	{
		using result_type = Container<pmx_surface<VertexIndex>>;
		using container_traits = resizable_container_traits<result_type, ContainerSizeType>;
		using size_type = container_traits::size_type;

		result_type result;

		// �ʂ̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���ݒ�
		container_traits::resize(result, static_cast<size_type>(num));

		// ���ꂼ��̏����擾
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_intanger_from_istream(in, &container_traits::get(result, static_cast<size_type>(i)), vertex_index_size);
		}

		return result;
	}

	// �e�N�X�`���p�X�̓ǂݍ���
	template<template<typename> typename Container, typename Str, typename ContainerSizeType = std::size_t, typename StrSizeType = std::size_t>
		requires resizable_container<Container<Str>, ContainerSizeType>&& resizable_container<Str, StrSizeType>
	Container<Str> load_texture_path(std::istream& in, encode_type encode)
	{
		using container_traits = resizable_container_traits<Container<Str>, ContainerSizeType>;
		using str_traits = resizable_container_traits<Str, StrSizeType>;

		Container<Str> result;

		// �R���e�i�̃T�C�Y�̎w��
		std::int32_t num;
		read_from_istream(in, &num);
		container_traits::resize(result, static_cast<container_traits::size_type>(num));

		// �����̑傫��
		auto char_size = static_cast<str_traits::size_type>(encode);

		// 1�����񂲂ƂɎ擾���Ă���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// ��������i�[����Q��
			auto& str = container_traits::get(result, static_cast<container_traits::size_type>(i));

			// �����̎擾
			std::int32_t len;
			read_from_istream(in, &len);

			// ������̓ǂݍ���
			str_traits::resize(str, static_cast<str_traits::size_type>(len / char_size));
			read_array_from_istream(in, &str, len / char_size, char_size);

		}

		return result;
	}

	// �}�e���A���̓ǂݍ���
	template<template<typename>typename Container, typename Str, constructible_vec3 Vec3, constructible_vec4 Vec4,
		typename TextureIndex = std::int32_t, typename HeaderData = std::uint8_t, typename ContianerSizeType = std::size_t, typename StrSizeType = std::size_t>
		requires resizable_container<Container<Str>, ContianerSizeType>&& resizable_container<Str, StrSizeType>
	Container<pmx_material<Str, Vec3, Vec4, TextureIndex>> load_material(std::istream& in, encode_type encode, HeaderData texture_index_size)
	{
		using container_type = Container<pmx_material<Str, Vec3, Vec4, TextureIndex>>;
		using container_traits = resizable_container_traits<container_type, ContianerSizeType>;
		using str_traits = resizable_container_traits<Str, StrSizeType>;

		container_type result;

		// �}�e���A���̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���w��
		container_traits::resize(result, static_cast<container_traits::size_type>(num));

		// �����̑傫��
		auto const char_size = static_cast<str_traits::size_type>(encode);

		// ���ꂼ��̃}�e���A���̓ǂݍ���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// �ǂݍ��ރ}�e���A��
			auto& material = container_traits::get(result, static_cast<container_traits::size_type>(i));

			// �������ǂݍ��ލۂɎg�p
			std::int32_t len;

			// ���O
			read_from_istream(in, &len);
			str_traits::resize(material.name, static_cast<str_traits::size_type>(len / char_size));
			read_array_from_istream(in, &material.name, len / char_size, char_size);

			// �p��̖��O
			read_from_istream(in, &len);
			str_traits::resize(material.english_name, static_cast<str_traits::size_type>(len / char_size));
			read_array_from_istream(in, &material.english_name, len / char_size, char_size);

			// �F���
			read_vec4_from_istream(in, &material.diffuse);
			read_vec3_from_istream(in, &material.specular);
			read_from_istream(in, &material.specularity);
			read_vec3_from_istream(in, &material.ambient);

			// �`��t���O
			std::uint8_t flag;
			read_from_istream(in, &flag);
			material.draw_flag_bits = { flag };

			// �G�b�W
			read_vec4_from_istream(in, &material.edge_color);
			read_from_istream(in, &material.edge_size);

			// �e�N�X�`��
			read_intanger_from_istream(in, &material.texture_index, texture_index_size);
			read_intanger_from_istream(in, &material.sphere_texture_index, texture_index_size);

			// �X�t�B�A���[�h
			std::uint8_t sphere;
			read_from_istream(in, &sphere);
			switch (sphere)
			{
			// ����
			case 0:
				material.sphere_mode_value = sphere_mode::none;
				break;

			// ��Z
			case 1:
				material.sphere_mode_value = sphere_mode::sph;
				break;

			// ���Z
			case 2:
				material.sphere_mode_value = sphere_mode::spa;
				break;

			// �T�u�e�N�X�`��
			case 3:
				material.sphere_mode_value = sphere_mode::subtexture;
				break;
			}

			// �g�D�[��
			std::uint8_t toon;
			read_from_istream(in, &toon);
			
			switch (toon)
			{
			// ��
			case 0:
				material.toon_type_value = toon_type::unshared;
				read_from_istream(in, &material.toon_texture, texture_index_size);
				break;

			// ���L
			case 1:
				material.toon_type_value = toon_type::shared;
				// ���L�̏ꍇ�̃C���f�b�N�X�̏ꍇ�̃T�C�Y��1byte
				read_from_istream(in, &material.toon_texture, 1);
				break;
			}

			// ����
			read_from_istream(in, &len);
			str_traits::resize(material.memo, static_cast<str_traits::size_type>(len / char_size));
			read_array_from_istream(in, &material.memo, len / char_size, char_size);

			// �ʂ̐�
			read_from_istream(in, &material.vertex_number);

		}

		return result;

	}
}