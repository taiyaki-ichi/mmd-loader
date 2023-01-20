#pragma once
#include<utility>

namespace mmdl
{
	struct vmd_header 
	{
		// 名前
		char name[20];

		//　フレームデータ数
		std::uint32_t frame_data_num;
	};

	template<typename Vec3,typename Vec4>
	struct vmd_frame_data
	{
		// 名前
		char name[15];

		// フレーム番号
		std::uint32_t frame_num;

		// 位置
		Vec3 transform;

		// クオータニオン
		Vec4 quaternion;

		// 補完パラメータ
		char complement_parameter[64];
	};

}