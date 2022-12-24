#pragma once
#include<utility>

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

}