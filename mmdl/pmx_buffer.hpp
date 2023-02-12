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
}