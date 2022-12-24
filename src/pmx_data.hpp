#pragma once
#include<utility>
#include<array>
#include<optional>

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
}