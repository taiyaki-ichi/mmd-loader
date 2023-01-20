#pragma once
#include"vmd_data.hpp"
#include"utility.hpp"
#include<istream>

namespace mmdl
{
	inline vmd_header load_vmd_header(std::istream& in)
	{
		vmd_header result;

		// �}�W�b�N�i���o�[
		char magic[30];
		in.read(magic, sizeof(magic));

		// ���O
		in.read(result.name, sizeof(result.name));

		// �t���[���f�[�^��
		read_from_istream(in, &result.frame_data_num);

		return result;
	}

	template<template<typename>typename Container,typename Vec3,typename Vec4>
	inline Container<vmd_frame_data<Vec3, Vec4>> load_vmd_frame_data(std::istream& in,auto size)
	{
		Container<vmd_frame_data<Vec3, Vec4>> result(size);

		for (std::size_t i = 0; i < static_cast<std::size_t>(size); i++)
		{
			auto& frame_data = result[i];

			// �{�[����
			in.read(frame_data.name, sizeof(frame_data.name));

			// �t���[���ԍ�
			read_from_istream(in, &frame_data.frame_num);

			// �ړ���
			read_vec3_from_istream(in, &frame_data.transform);

			// �N�I�[�^�j�I��
			read_vec4_from_istream(in, &frame_data.quaternion);

			// �⊮�p�����[�^
			in.read(frame_data.complement_parameter, sizeof(frame_data.complement_parameter));
		}

		return result;

	}
}