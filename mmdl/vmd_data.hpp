#pragma once
#include<utility>

namespace mmdl
{
	struct vmd_header 
	{
		// ���O
		char name[20];

		//�@�t���[���f�[�^��
		std::uint32_t frame_data_num;
	};

	template<typename Vec3,typename Vec4>
	struct vmd_frame_data
	{
		// ���O
		char name[15];

		// �t���[���ԍ�
		std::uint32_t frame_num;

		// �ʒu
		Vec3 transform;

		// �N�I�[�^�j�I��
		Vec4 quaternion;

		// �⊮�p�����[�^
		char complement_parameter[64];
	};

}