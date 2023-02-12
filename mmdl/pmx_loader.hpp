#pragma once
#include"pmx_data.hpp"
#include"utility.hpp"
#include"generics_type.hpp"
#include<concepts>
#include"pmx_buffer.hpp"

namespace mmdl
{

	// �w�b�_�̓ǂݍ���
	// pmx2.0�ȍ~��Data�̃T�C�Y��1byte�Ȃ̂Ńf�t�H���g��uint8_t
	template<typename T, typename traits = pmx_header_traits<T>>
	T load_header(std::istream& in)
	{
		pmx_header_buffer buffer{};

		// �ŏ���4�����͂���Ȃ��uPmx �v�̕���
		in.seekg(4);

		read_from_istream(in, &buffer.version);

		// �ȍ~�̃f�[�^�̑傫���̓ǂݍ���
		// �uPMX2.0�� 8 �ŌŒ�v�܂�Abit�P��?
		std::uint8_t size;
		read_from_istream(in, &size);
		// bit����byte�֕ϊ�
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

		// ���f�����̎擾
		read_from_istream(in, &buffer.model_name_size);
		read_from_istream(in, &buffer.model_name[0], buffer.model_name_size);

		// ���f���̉p�ꖼ�̎擾
		read_from_istream(in, &buffer.english_model_name_size);
		read_from_istream(in, &buffer.english_model_name[0], buffer.english_model_name_size);

		// �R�����g�̎擾
		read_from_istream(in, &buffer.comment_size);
		read_from_istream(in, &buffer.comment[0], buffer.comment_size);

		// �p��̃R�����g�̎擾
		read_from_istream(in, &buffer.english_comment_size);
		read_from_istream(in, &buffer.english_comment[0], buffer.english_comment_size);

		return traits::construct(buffer);
	}


	// ���_���̓ǂݍ���
	template<typename T, typename traits = pmx_vertex_traits<T>>
	T load_vertex(std::istream& in, std::size_t add_uv_number, std::size_t bone_index_size)
	{
		// ���_�̐����擾
		std::int32_t num;
		read_from_istream(in, &num);

		auto result = traits::construct(static_cast<std::size_t>(num));

		pmx_vertex_buffer buffer{};

		// ���ꂼ��̒��_�̎擾
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_from_istream(in, &buffer.position);
			read_from_istream(in, &buffer.normal);
			read_from_istream(in, &buffer.uv);

			// �ǉ�uv�̎擾
			for (std::size_t j = 0; j < add_uv_number; j++)
			{
				read_from_istream(in, &buffer.additional_uv[i]);
			}

			// �E�F�C�g�ό`�����̎擾
			read_from_istream(in, &buffer.weight_type);

			switch (buffer.weight_type)
			{
				// BDEF1�̏ꍇ
			case 0:
				read_intanger_from_istream(in, &buffer.bone_index[0], bone_index_size);

				break;

				// BDEF2�̏ꍇ
			case 1:
				read_intanger_from_istream(in, &buffer.bone_index[0], bone_index_size);
				read_intanger_from_istream(in, &buffer.bone_index[1], bone_index_size);
				read_from_istream(in, &buffer.bone_weight[0]);

				break;

				// BDEF4�̏ꍇ
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

				// SDEF�̔{
			case 3:
				read_intanger_from_istream(in, &buffer.bone_index[0], bone_index_size);
				read_intanger_from_istream(in, &buffer.bone_index[1], bone_index_size);
				read_from_istream(in, &buffer.bone_weight[0]);
				// �����܂�BDEF2�Ɠ���

				read_from_istream(in, &buffer.sdef_c);
				read_from_istream(in, &buffer.sdef_r0);
				read_from_istream(in, &buffer.sdef_r1);

				break;
			}

			read_from_istream(in, &buffer.edge_factor);

			traits::emplace_back(result, buffer, add_uv_number);

			// �ꉞ�A���������Ă���
			buffer.bone_index = {};
			buffer.bone_weight = {};
			buffer.sdef_c = {};
			buffer.sdef_r0 = {};
			buffer.sdef_r1 = {};
		}

		return result;
	}

	// �ʏ��̓ǂݍ���
	template<typename T, typename traits = pmx_surface_traits<T>>
	T load_surface(std::istream& in, std::size_t vertex_index_size)
	{
		// �ʂ̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���ݒ�
		auto result = traits::construct(static_cast<std::size_t>(num));

		std::size_t index{};
		// ���ꂼ��̏����擾
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			read_intanger_from_istream(in, &index, vertex_index_size);
			traits::emplace_back(result, index);
		}

		return result;
	}

	// �e�N�X�`���p�X�̓ǂݍ���
	template<typename T, typename traits = pmx_texture_path_traits<T>, std::size_t CharBufferSize = 64>
	T load_texture_path(std::istream& in)
	{
		using char_type = traits::char_type;

		// �R���e�i�̃T�C�Y�̎w��
		std::int32_t num;
		read_from_istream(in, &num);
		auto result = traits::construct(static_cast<std::size_t>(num));

		std::int32_t texture_path_size{};
		char_type texture_path_buffer[CharBufferSize]{};

		// 1�����񂲂ƂɎ擾���Ă���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// �����̎擾
			read_from_istream(in, &texture_path_size);

			// ������̓ǂݍ���
			read_from_istream(in, texture_path_buffer, texture_path_size);

			// ��������i�[
			traits::emplace_back(result, texture_path_buffer,
				texture_path_size % 2 == 0 ? texture_path_size / sizeof(char_type) : (texture_path_size + 1) / sizeof(char_type));

			// ���������Ă���
			std::fill(std::begin(texture_path_buffer), std::end(texture_path_buffer), 0);
		}

		return result;
	}

	// �}�e���A���̓ǂݍ���
	template<typename T, typename traits = pmx_material_traits<T>, std::size_t CharBufferSize = 64>
	T load_material(std::istream& in, std::size_t texture_index_size)
	{
		using char_type = traits::char_type;

		// �}�e���A���̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���w�肵�\�z
		auto result = traits::construct(static_cast<std::size_t>(num));

		pmx_material_buffer<char_type, CharBufferSize> buffer{};

		// ���ꂼ��̃}�e���A���̓ǂݍ���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// ���O
			read_from_istream(in, &buffer.name_size);
			read_from_istream(in, &buffer.name[0], buffer.name_size);

			// �p��̖��O
			read_from_istream(in, &buffer.english_name_size);
			read_from_istream(in, &buffer.english_name[0], buffer.english_name_size);

			// �F���
			read_from_istream(in, &buffer.diffuse);
			read_from_istream(in, &buffer.specular);
			read_from_istream(in, &buffer.specularity);
			read_from_istream(in, &buffer.ambient);

			// �`��t���O
			read_from_istream(in, &buffer.bit_flag);

			// �G�b�W
			read_from_istream(in, &buffer.edge_color);
			read_from_istream(in, &buffer.edge_size);

			// �e�N�X�`��
			read_intanger_from_istream(in, &buffer.texture_index, texture_index_size);
			read_intanger_from_istream(in, &buffer.sphere_texture_index, texture_index_size);

			// �X�t�B�A���[�h
			read_from_istream(in, &buffer.sphere_mode);

			// �g�D�[��
			read_from_istream(in, &buffer.toon_flag);

			// �ʃg�D�[���̏ꍇ
			if (buffer.toon_flag == 0) {
				read_intanger_from_istream(in, &buffer.toon_index, texture_index_size);
			}
			//���L�g�D�[���̏ꍇ
			else {
				read_intanger_from_istream(in, &buffer.toon_index, 1);
			}

			// ����
			read_from_istream(in, &buffer.memo_size);
			read_from_istream(in, &buffer.memo[0], buffer.memo_size);

			// �ʂ̐�
			read_from_istream(in, &buffer.vertex_num);

			// �v�f��ǉ�
			traits::emplace_back(result, buffer);


			// ���������Ă���
			std::fill(std::begin(buffer.name), std::end(buffer.name), 0);
			std::fill(std::begin(buffer.english_name), std::end(buffer.english_name), 0);
			std::fill(std::begin(buffer.memo), std::end(buffer.memo), 0);
		}

		return result;

	}

	// �{�[���̓ǂݍ���
	template<typename T, typename traits = pmx_bone_traits<T>, std::size_t BufferNum = 64, std::size_t IKLinkBufferNum = 16>
	T load_bone(std::istream& in, std::size_t bone_index_size)
	{
		using char_type = traits::char_type;

		// �{�[���̐��̎擾
		std::int32_t num;
		read_from_istream(in, &num);

		// �R���e�i�̑傫���w�肵�\�z
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


		// ���ꂼ��̃{�[���̓ǂݍ���
		for (std::size_t i = 0; i < static_cast<std::size_t>(num); i++)
		{
			// ���O
			read_from_istream(in, &name_size);
			read_from_istream(in, name, name_size);

			// �p��̖��O
			read_from_istream(in, &english_name_size);
			read_from_istream(in, english_name, english_name_size);

			// �ʒu
			read_from_istream(in, &position);

			// �e�{�[��
			read_from_istream(in, &parent_bone_index, bone_index_size);

			// �ό`�K�w
			read_from_istream(in, &transformation_level);

			// �{�[���t���O
			read_from_istream(in, &bone_flag);

			// �ڑ���
			if ((bone_flag & 0x0001) == 0)
			{
				read_from_istream(in, &access_point_offset);
			}
			else
			{
				read_intanger_from_istream(in, &access_bone_index, bone_index_size);
			}

			// ��]�t�^�܂��͈ړ��t�^
			if ((bone_flag & 0x0100) != 0 || (bone_flag & 0x0200) != 0)
			{
				read_intanger_from_istream(in, &assign_bone_index, bone_index_size);
				read_from_istream(in, &assign_rate);
			}

			// ���Œ�
			if ((bone_flag & 0x0400) != 0)
			{
				read_from_istream(in, &fixed_axis_direction);
			}

			// ���[�J����
			if ((bone_flag & 0x0800) != 0)
			{
				read_from_istream(in, &local_x_axis_direction);
				read_from_istream(in, &local_z_axis_direction);
			}

			// �O���e�ό`
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

				// IK���̎擾
				read_from_istream(in, &ik_link_size);

				for (std::size_t j = 0; j < ik_link_size; j++)
				{
					auto& [ik_bone_index, ik_is_angle_limit, ik_angle_limit_bottom, ik_angle_limit_top] = ik_link[j];

					read_intanger_from_istream(in, &ik_bone_index, bone_index_size);

					// �p�x����
					read_from_istream(in, &ik_is_angle_limit);

					if (ik_is_angle_limit == 1)
					{
						// ����
						read_from_istream(in, &ik_angle_limit_bottom);

						// ���
						read_from_istream(in, &ik_angle_limit_top);
					}
				}
			}

			// �v�f�̒ǉ�
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

			// ���������Ă���
			std::fill(std::begin(name), std::end(name), 0);
			std::fill(std::begin(english_name), std::end(english_name), 0);
		}

		return result;
	}
}