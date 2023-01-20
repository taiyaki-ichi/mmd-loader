#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>

namespace mmdl
{
	// �w�b�_�̓ǂݍ���
	// pmx2.0�ȍ~��Data�̃T�C�Y��1byte�Ȃ̂Ńf�t�H���g��uint8_t
	template<std::integral HeaderDataType = std::uint8_t>
	pmx_header<HeaderDataType> load_header(std::istream& in)
	{
		pmx_header<HeaderDataType> result;

		// �ŏ���4�����͂���Ȃ��uPmx �v�̕���
		in.seekg(4);

		read_from_istream(in, &result.version);

		// �ȍ~�̃f�[�^�̑傫���̓ǂݍ���
		std::uint8_t size;
		read_from_istream(in, &size);
		// bit����byte�֕ϊ�
		// WARNING: ��������bit���ۂ�?
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

		// ���f�����̎擾
		read_from_istream(in, &len);
		result.model_name = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.model_name, len / char_size, char_size);

		// ���f���̉p�ꖼ�̎擾
		read_from_istream(in, &len);
		result.english_mode_name = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.english_mode_name, len / char_size, char_size);

		// �R�����g�̎擾
		read_from_istream(in, &len);
		result.comment = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.comment, len / char_size, char_size);

		// �p��̃R�����g�̎擾
		read_from_istream(in, &len);
		result.english_comment = StrTraits::construct(static_cast<StrTraits::size_type>(len));
		read_array_from_istream<StrTraits>(in, &result.english_comment, len / char_size, char_size);

		return result;
	}


	// ���_���̓ǂݍ���
	template<template<typename> typename Container, constructible_vec2 Vec2, constructible_vec3 Vec3, constructible_vec4 Vec4,
		typename BoneIndex = std::int32_t, typename  HeaderDataType = std::uint8_t, typename ContainterSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>, ContainterSizeType>>
		Container<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> load_vertex(std::istream& in, HeaderDataType add_uv_number, HeaderDataType bone_index_size)
	{
		// ���_�̐����擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̐���
		auto result = ContainerTraits::construct(static_cast<ContainerTraits::size_type>(num));

		// ���ꂼ��̒��_�̎擾
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_vec3_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).position);
			read_vec3_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).normal);
			read_vec2_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).uv);

			// �ǉ�uv�̎擾
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				read_vec4_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).additional_uv[j]);
			}

			// �E�F�C�g�ό`�����̎擾
			std::uint8_t weight_type;
			read_from_istream(in, &weight_type);

			switch (weight_type)
			{
				// BDEF1�̏ꍇ
			case 0:
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[0], bone_index_size);
				// �P��̃{�[���̏d�݂�1�ł��邱�Ƃ�����
				ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0] = 1.f;
				break;

				// BDEF2�̏ꍇ
			case 1:
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[0], bone_index_size);
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[1], bone_index_size);
				read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0]);
				// 2�{�̃{�[���̏d�݂͍��v1�ɂȂ�
				ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[1] = 1.f - ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0];
				break;

				// BDEF4�̏ꍇ
			case 2:
				// 4�̃{�[���̃C���f�b�N�X�̎擾
				for (std::size_t j = 0; j < 4; j++)
					read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[j], bone_index_size);
				// 4�̃{�[���̏d�݂̎擾
				for (std::size_t j = 0; j < 4; j++)
					read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[j]);
				break;

				// SDEF�̔{
			case 3:
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[0], bone_index_size);
				read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).bone[1], bone_index_size);
				read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).weight[0]);
				// �����܂�BDEF2�Ɠ���
				std::array<Vec3, 3> sdef;
				for (std::size_t j = 0; j < 3; j++)
					read_from_istream(in, &sdef[j]);
				ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).sdef = std::move(sdef);
				break;
			}

			// �G�b�W�{���̎擾
			read_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)).edge_magnification);

		}

		return result;

	}

	// �ʏ��̓ǂݍ���
	template<template<typename> typename Container, typename VertexIndex = std::int32_t, typename HeaderDataType = std::uint8_t, typename ContainerSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_surface<VertexIndex>>, ContainerSizeType>>
		Container<pmx_surface<VertexIndex>> load_surface(std::istream& in, HeaderDataType vertex_index_size)
	{
		// �ʂ̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���ݒ�
		auto result = ContainerTraits::construct(static_cast<ContainerTraits::size_type>(num));

		// ���ꂼ��̏����擾
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_intanger_from_istream(in, &ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i)), vertex_index_size);
		}

		return result;
	}

	// �e�N�X�`���p�X�̓ǂݍ���
	template<template<typename> typename Container, typename Str, typename ContainerSizeType = std::size_t, typename StrSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<Str>, ContainerSizeType>, typename StrTraits = count_construct_container_traits<Str, ContainerSizeType>>
		Container<Str> load_texture_path(std::istream& in, encode_type encode)
	{
		// �R���e�i�̃T�C�Y�̎w��
		std::int32_t num;
		read_from_istream(in, &num);
		auto result = ContainerTraits::construct(num);

		// �����̑傫��
		auto const char_size = static_cast<StrTraits::size_type>(encode);

		// 1�����񂲂ƂɎ擾���Ă���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// �����̎擾
			std::int32_t len;
			read_from_istream(in, &len);

			// ������̓ǂݍ���
			auto str = StrTraits::construct(static_cast<StrTraits::size_type>(len / char_size));
			read_array_from_istream<StrTraits>(in, &str, len / char_size, char_size);

			// ��������i�[
			ContainerTraits::get_reference(result, static_cast<StrTraits::size_type>(i)) = std::move(str);
		}

		return result;
	}

	// �}�e���A���̓ǂݍ���
	template<template<typename>typename Container, typename Str, constructible_vec3 Vec3, constructible_vec4 Vec4,
		typename TextureIndex = std::int32_t, typename HeaderDataType = std::uint8_t, typename ContianerSizeType = std::size_t, typename StrSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_material<Str, Vec3, Vec4, TextureIndex>>, ContianerSizeType>,
		typename StrTraits = count_construct_container_traits<Str, StrSizeType>>
		Container<pmx_material<Str, Vec3, Vec4, TextureIndex>> load_material(std::istream& in, encode_type encode, HeaderDataType texture_index_size)
	{

		// �}�e���A���̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���w�肵�\�z
		auto result = ContainerTraits::construct(num);

		// �����̑傫��
		auto const char_size = static_cast<StrTraits::size_type>(encode);

		// ���ꂼ��̃}�e���A���̓ǂݍ���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// �ǂݍ��ރ}�e���A��
			auto& material = ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i));

			// �������ǂݍ��ލۂɎg�p
			std::int32_t len;

			// ���O
			read_from_istream(in, &len);
			material.name = StrTraits::construct(len);
			read_array_from_istream<StrTraits>(in, &material.name, len / char_size, char_size);

			// �p��̖��O
			read_from_istream(in, &len);
			material.english_name = StrTraits::construct(len);
			read_array_from_istream<StrTraits>(in, &material.english_name, len / char_size, char_size);

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
			material.memo = StrTraits::construct(len);
			read_array_from_istream<StrTraits>(in, &material.memo, len / char_size, char_size);

			// �ʂ̐�
			read_from_istream(in, &material.vertex_number);

		}

		return result;

	}

	// �{�[���̓ǂݍ���
	template<template<typename>typename Container, typename Str, typename Vec3, template<typename> typename IKContainer, typename BoneIndex = std::int32_t, typename HeaderDataType = std::uint8_t,
		typename ContianerSizeType = std::size_t, typename StrSizeType = std::size_t, typename IKContainerSizeType = std::size_t,
		typename ContainerTraits = count_construct_container_traits<Container<pmx_bone<Str, Vec3, IKContainer, BoneIndex>>, ContianerSizeType>,
		typename StrTraits = count_construct_container_traits<Str, StrSizeType>,
		typename IKContainerTraits = count_construct_container_traits<IKContainer<ik_link<Vec3, BoneIndex>>, IKContainerSizeType>>
		Container<pmx_bone<Str, Vec3, IKContainer, BoneIndex>> load_bone(std::istream& in, encode_type encode, HeaderDataType bone_index_size)
	{
		// �{�[���̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���w�肵�\�z
		auto result = ContainerTraits::construct(num);

		// �����̑傫��
		auto const char_size = static_cast<StrTraits::size_type>(encode);

		// ���ꂼ��̃{�[���̓ǂݍ���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// �ǂݍ��ރ{�[��
			auto& bone = ContainerTraits::get_reference(result, static_cast<ContainerTraits::size_type>(i));

			// �������ǂݍ��ލۂɎg�p
			std::int32_t len;

			// ���O
			read_from_istream(in, &len);
			bone.name = StrTraits::construct(static_cast<StrTraits::size_type>(len / char_size));
			read_array_from_istream<StrTraits>(in, &bone.name, len / char_size, char_size);

			// �p��̖��O
			read_from_istream(in, &len);
			bone.english_name = StrTraits::construct(static_cast<StrTraits::size_type>(len / char_size));
			read_array_from_istream<StrTraits>(in, &bone.english_name, len / char_size, char_size);

			// �ʒu
			read_vec3_from_istream(in, &bone.position);

			// �e�{�[��
			read_intanger_from_istream(in, &bone.parent_index, bone_index_size);

			// �ό`�K�w
			read_from_istream(in, &bone.trannsformation_level);

			// �{�[���t���O
			std::uint16_t bone_flag{};
			read_from_istream(in, &bone_flag);
			bone.bone_flag_bits = { bone_flag };

			// �ڑ�
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::access_point)])
			{
				read_intanger_from_istream(in, &bone.access_point_index, bone_index_size);
			}
			else
			{
				read_vec3_from_istream(in, &bone.access_point_offset);
			}

			// ��]�t�^�܂��͈ړ��t�^
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::rotation_grant)] ||
				bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::move_grant)])
			{
				read_intanger_from_istream(in, &bone.grant_index, bone_index_size);
				read_from_istream(in, &bone.grant_rate);
			}

			// ���Œ�
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::fix_axis)])
			{
				read_vec3_from_istream(in, &bone.fix_axis_direction);
			}

			// ���[�J����
			if (bone.bone_flag_bits[static_cast<std::size_t>(bone_flag::local_axis)])
			{
				read_vec3_from_istream(in, &bone.local_axis_x);
				read_vec3_from_istream(in, &bone.local_axis_z);
			}

			// �O���e�ό`
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

				// IK���̎擾
				std::int32_t ik_num;
				read_from_istream(in, &ik_num);

				// �R���e�i�̑傫���w��
				bone.ik_link = IKContainerTraits::construct(ik_num);

				for (std::size_t j = 0; j < ik_num; j++)
				{
					read_intanger_from_istream(in, &bone.ik_link[j].bone, bone_index_size);

					// �p�x����
					std::uint8_t is_angle_limit;
					read_from_istream(in, &is_angle_limit);

					if (is_angle_limit == 1)
					{
						// ����
						Vec3 min;
						read_vec3_from_istream(in, &min);

						// ���
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