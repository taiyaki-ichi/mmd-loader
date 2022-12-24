#pragma once
#include<utility>
#include<array>
#include<optional>
#include<bitset>

namespace mmdl
{
	enum class encode_type
	{
		utf16,
		utf8,
	};

	// pmx�w�b�_
	struct pmx_header
	{
		// �o�[�W����
		float version;

		// �G���R�[�h�̕���
		encode_type encode;

		// ���_�̒ǉ�UV�̐�
		// 1,2,4�̂����ꂩ
		std::uint8_t add_uv_number;

		// ���_�̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		std::uint8_t vertex_index_size;

		// �e�N�X�`���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		std::uint8_t texture_index_size;

		// �}�e���A���̃C���f�b�N�X�̑傫��
		// 1.2.4�̂����ꂩ
		std::uint8_t material_index_size;

		// �{�[���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		std::uint8_t bone_index_size;

		// ���W�b�g�{�f�B�̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		std::uint8_t rigid_body_index_size;

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
	// ���̍ő�̒l�ł���4�̑傫���ɑΉ�����std::uint32_t���f�t�H���g�Ƃ��Ďg�p����
	template<typename Vec2, typename Vec3, typename Vec4, template<typename> typename Container,
		typename BoneIndex = std::uint32_t>
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
	// pmx_header.vertex_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����uint32_t���f�t�H���g�Ɏw��
	// WARNING: ����Ȃ�����
	template<typename VertexIndex = std::uint32_t>
	using pmx_surface = VertexIndex;


	// �}�e���A��
	// pmx_header.texture_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����uint32_t���f�t�H���g�Ɏw��
	template<typename Str, typename Vec3, typename Vec4, typename TextureIndex = std::uint32_t>
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
		TextureIndex normal_texture_index;
		// �X�t�B�A�e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		TextureIndex sphere_texture_index;

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

		sphere_mode sphere_mode_value;


		// toon_flag=0�̎��Atoon_texture��Toon�e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		// toon_flag=1�̎��A���L�e�N�X�`���̃C���f�b�N�X�i0����9�܂ł̒l�j�i0�Ȃ�toon01.bmp,1�Ȃ�toon02.bmp�̂悤�ɑΉ��j
		std::uint8_t toon_flag;
		TextureIndex toon_texture;

		// �����A���R��
		Str memo;

		// �ގ��ɑΉ����钸�_�̐�
		// �K��3�̔{���ɂȂ�
		std::int32_t vertex_number;

	};

}