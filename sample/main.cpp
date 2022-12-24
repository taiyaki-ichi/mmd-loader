#include"pmx_loader.hpp"

#include<fstream>

int main()
{
	{
		const wchar_t* file_path = L"../../3dmodel/�p�C����/�h��.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/�Ƃ��̂������mmd_ver2.1/�Ƃ��̂���.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/kizunaai/kizunaai.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
	}

	return 0;
}