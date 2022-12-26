#pragma once
#include<utility>
#include<array>
#include<optional>
#include<bitset>

namespace mmdl
{
	// �G���R�[�h�̃^�C�v
	// �Ή����鐮����1�����̑傫��
	enum class encode_type
	{
		utf16 = 2,
		utf8 = 1,
	};

	// pmx�w�b�_
	// Data��pmx2.0�ł̓T�C�Y��8�Œ�
	template<typename HeaderData = std::int64_t>
	struct pmx_header
	{
		// �o�[�W����
		float version;

		// �G���R�[�h�̕���
		encode_type encode;

		// ���_�̒ǉ�UV�̐�
		// 1,2,4�̂����ꂩ
		HeaderData add_uv_number;

		// ���_�̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderData vertex_index_size;

		// �e�N�X�`���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderData texture_index_size;

		// �}�e���A���̃C���f�b�N�X�̑傫��
		// 1.2.4�̂����ꂩ
		HeaderData material_index_size;

		// �{�[���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderData bone_index_size;

		// ���[���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderData morph_index_size;

		// ���W�b�g�{�f�B�̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderData rigid_body_index_size;

		// �u1,2,4�̂����ꂩ�v�ɂ���
		// 1: 1byte��unsigned
		// 2: 2byte��unsigned
		// 4: 4byte��signed
		// �ƂȂ�̂Œ���

	};


	// ���f���̖��O�Ƃ��̑��̏��
	template<typename Str>
	struct pmx_info
	{
		// ���f���̖��O
		Str model_name;

		// ���f���̉p�ꖼ
		Str english_mode_name;

		// �R�����g
		Str comment;

		// �p��̃R�����g
		Str english_comment;

	};



	// 1���_���
	// �{�[���̃C���f�b�N�X�̑傫���ɂ��āApmx_header.bone_index_size��1,2,4�̒l���Ƃ�
	// ���̍ő�̒l�ł���4�̑傫���ɑΉ�����std::int32_t���f�t�H���g�Ƃ��Ďg�p����
	template<typename Vec2, typename Vec3, typename Vec4, typename BoneIndex = std::int32_t>
	struct pmx_vertex
	{
		// ���_���W
		Vec3 position;

		// �@��
		Vec3 normal;

		// uv
		Vec2 uv;

		// �ǉ���uv
		// pmx_header.add_uv_number�̐������g�p����
		std::array<Vec4, 4> additional_uv{};

		// �e�����󂯂�{�[���̐���weight���Ή����Ă���
		// BDEF1�Ȃ�weight[0]=1�A���̂ق���0
		// BDEF2�Ȃ�weight[0]!=0,weight[1]!=0�A���̂ق���0�i���Ȃ݂�weight[0]+weight[1]=1�j
		// BDEF4�Ȃ�weight�͑S��0�łȂ�
		std::array<BoneIndex, 4> bone{ 0,0,0,0 };
		std::array<float, 4> weight{ 0.f,0.f,0.f,0.f };

		// SDEF�p�̕␳�s��
		// SDEF��BDEF2�ƕ␳�s��ɂ���Čv�Z�����
		// sdef[0]�FSDEF-C�l
		// sdef[1]�FSDEF-R0�l
		// sdef[2]�FSDEF-R1�l
		std::optional<std::array<Vec3, 3>> sdef = std::nullopt;

		// �G�b�W�{��
		// �ގ��̃G�b�W�T�C�Y�ɑ΂��Ă̔{���l
		float edge_magnification;

	};


	// �ʂ��\�����钸�_�̏��
	// 3�ň�̎O�p�`�̖ʂ�\��
	// pmx_header.vertex_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����int32_t���f�t�H���g�Ɏw��
	// WARNING: ����Ȃ�����
	template<typename VertexIndex = std::int32_t>
	using pmx_surface = VertexIndex;


	// pmx_material�Ŏg�p
	enum class sphere_mode
	{
		// �����Ȃ�
		none,

		// ��Z
		sph,

		// ���Z
		spa,

		// �T�u�e�N�X�`��
		// �ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��
		subtexture,
	};

	// pmx_material�Ŏg�p
	enum class toon_type
	{
		// ���L
		shared,

		// ��
		unshared,
	};

	// �}�e���A��
	// pmx_header.texture_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����int32_t���f�t�H���g�Ɏw��
	template<typename Str, typename Vec3, typename Vec4, typename TextureIndex = std::int32_t>
	struct pmx_material
	{
		// �}�e���A���̖��O
		Str name;

		// �}�e���A���̉p��̖��O
		Str english_name;

		// �f�B�t���[�Y
		Vec4 diffuse;
		// �X�؃L����
		Vec3 specular;
		// �X�؃L�����W��
		float specularity;
		// �A���r�G���g
		Vec3 ambient;

		enum class draw_flag
		{
			// �Жʂ̕`����s�����ǂ���
			single_sided,

			// �n�ʂɉe�𗎂Ƃ����ǂ���
			grond_shadow,

			// �Z���t�V���h�E�}�b�v�֕`�悷�邩�ǂ���
			// WARNING: ���O������ɂ���
			draw_to_self_shadow_map,

			// �Z���t�V���h�E��`�悷�邩�ǂ���
			// WARNING: ���O������ɂ���
			draw_self_shadow,

			// �G�b�W��`�悷�邩�ǂ���
			draw_edge,
		};

		// �`��̃I�v�V����
		// WARNING: �}�W�b�N�i���o�[
		std::bitset<5> draw_flag_bits;

		// �G�b�W�̐F
		Vec4 edge_color;
		// �G�b�W�̑傫��
		float edge_size;

		// �ʏ�̃e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		TextureIndex texture_index;
		// �X�t�B�A�e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		TextureIndex sphere_texture_index;

		sphere_mode sphere_mode_value;


		// toon_type::unshared�̎��Atoon_texture��Toon�e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		// toon_type::shared�̎��A���L�e�N�X�`���̃C���f�b�N�X�i0����9�܂ł̒l�j�i0�Ȃ�toon01.bmp,1�Ȃ�toon02.bmp�̂悤�ɑΉ��j
		toon_type toon_type_value;
		TextureIndex toon_texture;

		// �����A���R��
		Str memo;

		// �ގ��ɑΉ����钸�_�̐�
		// �K��3�̔{���ɂȂ�
		std::int32_t vertex_number;

	};

	// �{�[��
	// pmx_header.bone_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����int32_t���f�t�H���g�Ɏw��
	template<typename Str, typename Vec3, template<typename> typename Container, typename BoneIndex = std::int32_t>
	struct pmx_bone
	{
		// �{�[���̖��O
		Str name;

		// �{�[���̉p��̖��O
		Str english_name;

		// �ʒu
		Vec3 position;
		// �e�{�[���̃C���f�b�N�X
		BoneIndex parent_index;
		// �ό`�K�w
		std::int32_t trannsformation_level;

		enum class bone_flag
		{
			// �ڑ���@�����@
			// false: ���W�I�t�Z�b�g�Ŏw��
			// true: �{�[���Ŏw��
			access_point,

			// ��]�\���ǂ���
			rotatable,

			// �ړ��\���ǂ���
			movable,

			// �\��
			// WARNING; �\���\���ǂ����Ƃ����Ӗ���?
			display,

			// ����\���ǂ���
			operable,

			// IK�g�����ǂ���
			ik,

			// ���[�J���t�^
			// false: ���[�U�[�ό`�l�^IK�����N�^���d�t�^
			// true: �e�̃��[�J���ό`��
			local_grant,

			// ��]�t�^
			rotation_grant,

			// �ړ��t�^
			move_grant,

			// ���Œ肩�ǂ���
			fix_axis,

			// ���[�J�������ǂ���
			local_axis,

			// ������ό`���ǂ���
			post_physical_deformation,

			// �O���e�ό`���ǂ���
			external_parent_deformation,

		};

		// WARNING: �}�W�b�N�i���o�[
		std::bitset<16> bone_flag_bits;

		// bone_flag_bits[access_point]=false�̂Ƃ��g�p
		// �{�[���̈ʒu����̑��ΓI�Ȉʒu��\��
		Vec3 access_point_offset;
		// bone_flag_bits[access_point]=ture�̂Ƃ��g�p
		// �ڑ���̃{�[���̃C���f�b�N�X��\��
		BoneIndex access_point_index;

		// bone_flag_bits[rotation_grant]=true�܂��� bone_flag_bits[move_grant]=true�̂Ƃ��g�p
		// �Ώۂ̃{�[����\��
		BoneIndex grant_index;
		// �t�^��
		Vec3 grant_rate;

		// bone_flag_bits[fix_axis]=true�̎��Ɏg�p
		// �Œ肳�ꂽ���̕����x�N�g��
		Vec3 fix_axis_direction;

		// bone_flag_bits[local_axis]=true�̎��Ɏg�p
		// x���̕����x�N�g��
		Vec3 local_axis_x;
		// z���̕����x�N�g��
		Vec3 local_axis_z;

		// bone_flag_bits[external_parent_deformation]=true�̎��Ɏg�p
		// Key�̒l
		std::int16_t external_parent_deformation_key;

		// bone_flag_bits[ik]=true�̎��Ɏg�p
		//IK�̃^�[�Q�b�g�̃{�[��
		BoneIndex ik_target_bone;
		// IK�̃��[�v��
		std::int16_t ik_roop_number;
		// IK�̃��[�v���s���ۂ�1�񓖂���̐����p�x�i���W�A���j
		float ik_rook_angle;

		struct ik_link {
			// �����N���Ă���{�[��
			BoneIndex bone;

			// �p�x�������s���ꍇ�̍ŏ��A�ő�̊p�x�̐���
			std::optional<std::pair<Vec3, Vec3>> min_max_angle_limit;
		};

		Container<ik_link> ik_link;


		// TODO: �u bone_flag_bits[-]=true�̎��Ɏg�p�v���ď����Ă���Ƃ���Optional�ŃC�C����
	};

}