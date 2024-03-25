#pragma once
#include<array>

namespace mmdl::vmd
{
	struct header_buffer
	{
		// ���O
		// �T�C�Y�͌Œ�
		std::array<char, 20> name{};

		// �t���[���f�[�^��
		std::uint32_t frame_data_num{};
	};

	struct frame_buffer
	{
		// ���O
		// �T�C�Y�͌Œ�
		std::array<char, 15> name{};

		// �t���[���ԍ�
		std::uint32_t frame_num{};

		// �ʒu
		std::array<float, 3> transform{};

		// �N�I�[�^�j�I��
		std::array<float, 4> quaternion{};

		// �⊮�p�����[�^
		std::array<char, 64> complement_parameter{};
	};

	struct morph_buffer
	{
		// ���O
		// �T�C�Y�͌Œ�
		std::array<char, 15> name{};

		// �t���[���ԍ�
		std::uint32_t frame_num{};

		// �\��X���C�_�[�̒l
		float weight{};
	};
}