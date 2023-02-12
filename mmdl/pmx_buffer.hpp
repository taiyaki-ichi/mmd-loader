#pragma once
#include<array>

namespace mmdl
{
	struct pmx_header_buffer
	{
		// �o�[�W����
		float version{};

		// �G���R�[�h
		//  0:UTF16 1:UTF8
		std::uint8_t encode{};

		// �ǉ�UV�̐�
		// 0-4
		std::uint8_t additional_uv_num{};

		// ���_�̃C���f�b�N�X�̃T�C�Y
		// 1,2,4�̂����ꂩ
		std::uint8_t vertex_index_size{};

		// �e�N�X�`���̃C���f�b�N�X�̃T�C�Y
		std::uint8_t texture_index_size{};

		// �e�N�X�`���̃C���f�b�N�X�̃T�C�Y
		// 1,2,4�̂����ꂩ
		std::uint8_t material_index_size{};

		// �{�[���̃C���f�b�N�X�̃T�C�Y
		// 1,2,4�̂����ꂩ
		std::uint8_t bone_index_size{};

		// ���[�t�̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		std::uint8_t morph_index_size{};

		// ���̂̃C���f�b�N�X�̑傫��
		std::uint8_t rigid_body_index_size{};
	};


	template<typename CharType, std::size_t CharBufferSize>
	struct pmx_info_buffer
	{
		// ���f���̖��O
		std::int32_t model_name_size{};
		std::array<CharType, CharBufferSize> model_name{};

		// �p��̃��f���̖��O
		std::int32_t english_model_name_size{};
		std::array<CharType, CharBufferSize> english_model_name{};

		// �R�����g
		std::int32_t comment_size{};
		std::array<CharType, CharBufferSize> comment{};

		// �p��̃R�����g
		std::int32_t english_comment_size{};
		std::array<CharType, CharBufferSize> english_comment{};
	};


	struct pmx_vertex_buffer
	{
		// �ʒu
		std::array<float, 3> position{};

		// �@��
		std::array<float, 3> normal{};

		// UV
		std::array<float, 2> uv{};

		// �ǉ�UV
		std::array<std::array<float, 4>, 4> additional_uv{};

		// �E�F�C�g�ό`�`��
		// 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF
		std::uint8_t weight_type{};

		// �{�[���̃C���f�b�N�X
		std::array<std::size_t, 4> bone_index{};

		// �{�[���̃E�F�C�g
		std::array<float, 4> bone_weight{};

		// SDEF-C�l
		std::array<float, 3> sdef_c{};

		// SDEF-R0�l
		std::array<float, 3> sdef_r0{};

		// SDEF-R1�l
		std::array<float, 3> sdef_r1{};

		// �G�b�W�{��
		float edge_factor{};
	};


	template<typename CharType, std::size_t CharBufferSize>
	struct pmx_material_buffer
	{
		// �}�e���A���̖��O
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// �p��̃}�e���A���̖��O
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// �f�B�t���[�Y
		std::array<float, 4> diffuse{};
		// �X�؃L����
		std::array<float, 3> specular{};
		// �X�؃L�����W��
		float specularity{};
		// �A���r�G���g
		std::array<float, 3> ambient{};

		/*
		�`��t���O
			0x01:���ʕ`��
			0x02:�n�ʉe
			0x04:�Z���t�V���h�E�}�b�v�ւ̕`��
			0x08:�Z���t�V���h�E�̕`��
			0x10:�G�b�W�`��
		*/
		std::uint8_t bit_flag{};

		// �G�b�W�̐F
		std::array<float, 4> edge_color{};
		// �G�b�W�̑傫��
		float edge_size{};

		// �ʏ�̃e�N�X�`���̃C���f�b�N�X
		std::size_t texture_index{};
		// �X�t�B�A�e�N�X�`���̃C���f�b�N�X
		std::size_t sphere_texture_index{};

		/*
		�X�t�B�A���[�h
			0:����
			1:��Z(sph)
			2:���Z(spa) 3:�T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
		*/
		std::uint8_t sphere_mode{};

		// ���L�g�D�[���t���O 
		std::uint8_t toon_flag{};
		// ���L�g�D�[���t���O��0: �e�N�X�`���̃C���f�b�N�X
		// ���L�g�D�[���t���O��1: 0..9 -> toon01.bmp...toon10.bmp �ɑΉ�
		std::size_t toon_index{};

		// ����
		std::int32_t memo_size{};
		std::array<CharType, CharBufferSize> memo{};

		// �}�e���A���̑Ή����钸�_�̐�
		//  (�K��3�̔{���ɂȂ�)
		std::int32_t vertex_num{};
	};


	template<typename CharType, std::size_t CharBufferNum, std::size_t IKLinkBufferNum>
	struct pmx_bone_buffer
	{
		// ���O
		std::int32_t name_size{};
		std::array<CharType, CharBufferNum> name{};

		// �p��̖��O
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferNum> english_name{};

		// �ʒu
		std::array<float, 3> position{};

		// �e�{�[���̃C���f�b�N�X
		std::size_t parent_bone_index{};

		// �ό`�K�w
		std::int32_t transformation_level{};

		/*
		�{�[���t���O
			0x0001  : �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0:���W�I�t�Z�b�g�Ŏw�� 1:�{�[���Ŏw��

			0x0002  : ��]�\
			0x0004  : �ړ��\
			0x0008  : �\��
			0x0010  : �����

			0x0020  : IK

			0x0080  : ���[�J���t�^ | �t�^�Ώ� 0:���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1:�e�̃��[�J���ό`��
			0x0100  : ��]�t�^
			0x0200  : �ړ��t�^

			0x0400  : ���Œ�
			0x0800  : ���[�J����

			0x1000  : ������ό`
			0x2000  : �O���e�ό`
		*/
		std::uint16_t bone_flag{};

		// �ڑ���̃r�b�g��0�̏ꍇ�Ɏg�p������W�I�t�Z�b�g
		std::array<float, 3> access_point_offset{};
		// �ڑ���̃r�b�g��1�̏ꍇ�Ɏg�p����ڑ���̃{�[���̃C���f�b�N�X
		std::size_t access_bone_index{};
		// ��]�t�^�܂��͈ړ��t�^�̃r�b�g��1�̏ꍇ�Ɏg�p����t�^�e�̃{�[���̃C���f�b�N�X
		std::size_t assign_bone_index{};
		// ��]�t�^�܂��͈ړ��t�^�̃r�b�g��1�̏ꍇ�Ɏg�p����t�^��
		float assign_rate{};
		// ���Œ�̃r�b�g��1�̏ꍇ�Ɏg�p���鎲�̕����x�N�g��
		std::array<float, 3> fixed_axis_direction{};
		// ���[�J�����̃r�b�g��1�̏ꍇ�Ɏg�p����X���̕����x�N�g��
		std::array<float, 3> local_x_axis_direction{};
		// ���[�J�����̃r�b�g��1�̏ꍇ�Ɏg�p����Z���̕����x�N�g��
		std::array<float, 3> local_z_axis_direction{};
		// �O���e�ό`�̃r�b�g��1�̏ꍇ�Ɏg�p����Key�l
		std::int32_t key_value{};

		// �ȉ��AIK�̃r�b�g��1�̏ꍇ�Ɏg�p����
		// IK�^�[�Q�b�g�̃{�[��
		std::size_t ik_target_bone{};
		// IK�̃��[�v��
		std::int32_t ik_loop_num{};
		// IK��1�񓖂���̊p�x����
		float ik_angle_limit_par_process{};
		// IK�����N�̐�
		std::int32_t ik_link_size{};
		// ik�����N�̌^
		// �{�[���̃C���f�b�N�X�A�p�x�����̗L���A�p�x�����̉����A�p�x�����̏���A�A�A�̏�
		using ik_link_type = std::tuple<std::size_t, std::uint8_t, std::array<float, 3>, std::array<float, 3>>;
		// ik�����N
		std::array<ik_link_type, IKLinkBufferNum> ik_link{};
	};
}
