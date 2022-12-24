#include"pmx_loader.hpp"

#include<fstream>

int main()
{
	{
		const wchar_t* file_path = L"../../3dmodel/ƒpƒCƒ‚ƒ“/”h–Ö.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/‚Æ‚«‚Ì‚»‚çŒöŽ®mmd_ver2.1/‚Æ‚«‚Ì‚»‚ç.pmx";
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