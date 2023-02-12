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
		std::array<CharType,CharBufferSize> name{};

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
}