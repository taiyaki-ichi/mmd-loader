#pragma once
#include"utility.hpp"
#include<istream>
#include"vmd_loader_traits.hpp"

namespace mmdl
{
	template<typename T, typename traits = vmd_header_traits<T>>
	T load_vmd_header(std::istream& in)
	{
		vmd_header_buffer buffer{};

		// �}�W�b�N�i���o�[
		char magic[30];
		in.read(magic, sizeof(magic));

		// ���O
		read_from_istream(in, &buffer.name);

		// �t���[���f�[�^��
		// TODO: load_vmd_frame_data�œǂݍ��ނ悤�ɂ���
		read_from_istream(in, &buffer.frame_data_num);

		return traits::construct(buffer);
	}

	template<typename T, typename traits = vmd_frame_data_traits<T>>
	T load_vmd_frame_data(std::istream& in, std::size_t size)
	{
		auto result = traits::construct(size);

		vmd_frame_data_buffer buffer{};

		for (std::size_t i = 0; i < size; i++)
		{

			// �{�[����
			read_from_istream(in, &buffer.name);

			// �t���[���ԍ�
			read_from_istream(in, &buffer.frame_num);

			// �ړ���
			read_from_istream(in, &buffer.transform);

			// �N�I�[�^�j�I��
			read_from_istream(in, &buffer.quaternion);

			// �⊮�p�����[�^
			read_from_istream(in, &buffer.complement_parameter);

			// �v�f��ǉ�
			traits::emplace_back(result, buffer);
		}

		return result;
	}

	template<typename T, typename traits = vmd_morph_data_traits<T>>
	T load_vmd_morph_data(std::istream& in)
	{
		std::uint32_t size{};
		read_from_istream(in, &size);

		auto result = traits::construct(size);

		vmd_morph_data_buffer buffer{};

		for (std::size_t i = 0; i < size; i++)
		{
			// ���[�t��
			read_from_istream(in, &buffer.name);

			// �t���[���ԍ�
			read_from_istream(in, &buffer.frame_num);

			// �d��
			read_from_istream(in, &buffer.weight);

			traits::emplace_back(result, buffer);
		}

		return result;
	}
}