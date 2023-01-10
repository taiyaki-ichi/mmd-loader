#pragma once

namespace mmdl
{
	template<typename Str,typename Vec3,typename Vec4>
	struct vpd_data
	{
		Str name;
		Vec3 transform;
		Vec4 quaternion;
	};
}