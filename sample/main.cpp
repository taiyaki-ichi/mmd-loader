#include"pmx_loader.hpp"

#include<fstream>
#include<string>

int main()
{
	{
		const wchar_t* file_path = L"../../3dmodel/パイモン/派蒙.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::wstring>(file);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/パイモン/派蒙.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::string>(file);
	}
	
	{
		const wchar_t* file_path = L"../../3dmodel/ときのそら公式mmd_ver2.1/ときのそら.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::wstring>(file);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/ときのそら公式mmd_ver2.1/ときのそら.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::string>(file);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/kizunaai/kizunaai.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::wstring>(file);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/kizunaai/kizunaai.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::string>(file);
	}

	return 0;
}