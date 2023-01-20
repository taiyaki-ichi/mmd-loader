#pragma once
#include"vmd_data.hpp"
#include"utility.hpp"
#include<istream>

namespace mmdl
{
	inline vmd_header load_vmd_header(std::istream& in)
	{
		vmd_header result;

		// マジックナンバー
		char magic[30];
		in.read(magic, sizeof(magic));

		// 名前
		in.read(result.name, sizeof(result.name));

		// フレームデータ数
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

			// ボーン名
			in.read(frame_data.name, sizeof(frame_data.name));

			// フレーム番号
			read_from_istream(in, &frame_data.frame_num);

			// 移動量
			read_vec3_from_istream(in, &frame_data.transform);

			// クオータニオン
			read_vec4_from_istream(in, &frame_data.quaternion);

			// 補完パラメータ
			in.read(frame_data.complement_parameter, sizeof(frame_data.complement_parameter));
		}

		return result;

	}
}