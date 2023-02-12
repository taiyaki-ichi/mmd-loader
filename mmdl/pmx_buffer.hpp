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
}