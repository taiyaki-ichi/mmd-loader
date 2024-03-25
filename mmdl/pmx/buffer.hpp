#pragma once
#include<array>
#include<variant>

namespace mmdl::pmx
{
	struct header_buffer
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
	struct info_buffer
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


	struct vertex_buffer
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
	struct material_buffer
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
	struct bone_buffer
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


	// ���_���[�t
	// ���_�̃I�t�Z�b�g
	struct vertex_morph_buffer
	{
		// �Ώۂ̒��_�̃C���f�b�N�X
		std::size_t index{};

		// ���W�̃I�t�Z�b�g�l
		std::array<float, 3> offset{};
	};

	// uv���[�t
	// uv�I�t�Z�b�g�l
	// �ʏ�UV��z,w���s�v���ڂɂȂ邪���[�t�Ƃ��Ẵf�[�^�l�͋L�^���Ă���
	struct uv_morph_buffer
	{
		// �Ώۂ̒��_�̃C���f�b�N�X
		std::size_t index{};

		// uv�̃I�t�Z�b�g�l
		std::array<float, 4> offset{};
	};

	// �{�[�����[�t
	struct bone_morph_buffer
	{
		// �Ώۂ̃{�[���̃C���f�b�N�X
		std::size_t index{};

		// �ړ���
		std::array<float, 3> transform{};

		// ��]�ʁi�N�H�[�^�j�I���j
		std::array<float, 4> quaternion{};
	};

	// �ގ����[�t
	struct material_morph_buffer
	{
		// �Ώۂ̃}�e���A���̃C���f�b�N�X
		std::size_t index{};

		// �I�t�Z�b�g�`��
		//  0:��Z, 1:���Z
		std::uint8_t offset_type{};

		// �f�B�t���[�Y
		std::array<float, 4> diffuse{};
		// �X�؃L����
		std::array<float, 3> specular{};
		// �X�؃L�����W��
		float specularity{};
		// �A���r�G���g
		std::array<float, 3> ambient{};

		// �G�b�W�̐F
		std::array<float, 4> edge_color{};
		// �G�b�W�̑傫��
		float edge_size{};

		// �e�X�N�`���W��
		std::array<float, 4> texture_factor{};

		// �X�t�B�A�e�N�X�`���W��
		std::array<float, 4> sphere_texture_factor{};

		// �g�D�[���e�N�X�`���W��
		std::array<float, 4> toon_texture_factor{};

	};

	// �O���[�v���[�t
	struct group_morph_buffer
	{
		// �Ώۂ̃��[�t�̃C���f�b�N�X
		std::size_t index{};

		// ���[�t��
		// �O���[�v���[�t�̃��[�t�l * ���[�t�� = �Ώۃ��[�t�̃��[�t�l
		float morph_factor{};
	};

	// ���[�t�̃o�b�t�@
	template<typename CharType, std::size_t CharBufferSize,std::size_t MorphBufferNum>
	struct morph_buffer
	{
		// ���O
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// �p��̖��O
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// ����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��
		std::uint8_t control_panel_option{};

		// ���[�t��� - 0:�O���[�v, 1 : ���_, 2 : �{�[��, 3 : UV, 4 : �ǉ�UV1, 5 : �ǉ�UV2, 6 : �ǉ�UV3, 7 : �ǉ�UV4, 8 : �ގ�
		std::uint8_t morph_type{};

		// �ȍ~�̃��[�t�̃f�[�^�̐�
		std::int32_t morph_data_num{};

		// ���[�t�̎�ނ��Ƃ̃f�[�^
		using morph_type_variant = std::variant<vertex_morph_buffer, uv_morph_buffer, bone_morph_buffer,
			material_morph_buffer, group_morph_buffer>;
		std::array<morph_type_variant, MorphBufferNum> morph_data{};

		static constexpr std::size_t VERTEX_MORPH_INDEX = 0;
		static constexpr std::size_t UV_MORPH_INDEX = 1;
		static constexpr std::size_t BONE_MORPH_INDEX = 2;
		static constexpr std::size_t MATERIAL_MORPH_INDEX = 3;
		static constexpr std::size_t GROUP_MORPH_INDEX = 4;
	};

	// ���̂̃o�b�t�@
	template<typename CharType, std::size_t CharBufferSize>
	struct rigidbody_buffer
	{
		// ���O
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// �p��̖��O
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// �֘A�{�[���̃C���f�b�N�X
		// �֘A�i�V�̏ꍇ��-1
		std::size_t bone_index{};

		// �O���[�v
		std::uint8_t group{};
		// ��Փ˃O���[�v�t���O
		std::uint16_t non_collision_group{};

		// �`��
		// 0:�� 1:�� 2:�J�v�Z��
		std::uint8_t shape{};
		// �T�C�Y
		// (x,y,z)�H�H�H�H
		std::array<float, 3> size{};

		// �ʒu
		std::array<float, 3> position{};
		// ��]�i���W�A���p�j
		std::array<float, 3> rotation{};

		// ����
		float mass{};
		// �ړ�����
		float liner_damping{};
		// ��]����
		float angular_damping{};
		// ������
		float restitution{};
		// ���C��
		float friction{};

		// ���̂̕������Z
		// 0:�{�[���Ǐ](static) 1:�������Z(dynamic) 2:�������Z + Bone�ʒu���킹
		std::uint8_t rigidbody_type{};
	};

	// �W���C���g�̃o�b�t�@
	template<typename CharType, std::size_t CharBufferSize>
	struct joint_buffer
	{
		// ���O
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// �p��̖��O
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// �W���C���g�̎��
		// 0:�X�v�����O6DOF | PMX2.0�ł� 0 �̂�(�g���p)
		std::uint8_t type{};

		// �ȉ��Atype==0 �̏ꍇ�Ɏg�p
		// �������APMX2.0�ł�type==0�ł���̂Ŏg�����ƂɂȂ�

		// �֘A����A�̃C���f�b�N�X�i�����Ȃ��̏ꍇ��-1�j
		std::int32_t rigidbody_a{};

		// �֘A����B�̃C���f�b�N�X�i�����Ȃ��̏ꍇ��-1�j
		std::int32_t rigidbody_b{};

		// �ʒu
		std::array<float, 3> position{};
		// ��]�i���W�A���p�j
		std::array<float, 3> rotation{};

		// �ړ������F����
		std::array<float, 3> move_lower_limit{};
		// �ړ������F���
		std::array<float, 3> move_upper_limit{};
		// ��]�����i���W�A���p�j�F����
		std::array<float, 3> rotation_lower_limit{};
		// ��]�����i���W�A���p�j�F���
		std::array<float, 3> rotation_upper_limit{};

		// �΂˒萔�F�ړ�
		std::array<float, 3> move_spring_constant{};
		// �΂˒萔�F��]
		std::array<float, 3> rotation_spring_constant{};

	};

}
