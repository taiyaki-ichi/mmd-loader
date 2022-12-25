#include"pmx_loader.hpp"

#include<fstream>
#include<string>
#include<vector>


struct vec2
{
	float x, y;
};

struct vec3
{
	float x, y, z;
};

struct vec4
{
	float x, y, z, w;
};


int main()
{
	
	{
		const wchar_t* file_path = L"../../3dmodel/ƒpƒCƒ‚ƒ“/”h–Ö.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::wstring>(file, header.encode);
		auto vertex = mmdl::load_vertex<std::vector, vec2, vec3, vec4>(file, header.add_uv_number, header.bone_index_size);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/ƒpƒCƒ‚ƒ“/”h–Ö.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::string>(file, header.encode);
		auto vertex = mmdl::load_vertex<std::vector, vec2, vec3, vec4>(file, header.add_uv_number, header.bone_index_size);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/‚Æ‚«‚Ì‚»‚çŒöŽ®mmd_ver2.1/‚Æ‚«‚Ì‚»‚ç.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::wstring>(file, header.encode);
		auto vertex = mmdl::load_vertex<std::vector, vec2, vec3, vec4>(file, header.add_uv_number, header.bone_index_size);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/‚Æ‚«‚Ì‚»‚çŒöŽ®mmd_ver2.1/‚Æ‚«‚Ì‚»‚ç.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::string>(file, header.encode);
		auto vertex = mmdl::load_vertex<std::vector, vec2, vec3, vec4>(file, header.add_uv_number, header.bone_index_size);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/kizunaai/kizunaai.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::wstring>(file, header.encode);
		auto vertex = mmdl::load_vertex<std::vector, vec2, vec3, vec4>(file, header.add_uv_number, header.bone_index_size);
	}

	{
		const wchar_t* file_path = L"../../3dmodel/kizunaai/kizunaai.pmx";
		std::ifstream file{ file_path ,std::ios::binary };
		auto header = mmdl::load_header<std::uint16_t>(file);
		auto info = mmdl::load_info<std::string>(file, header.encode);
		auto vertex = mmdl::load_vertex<std::vector, vec2, vec3, vec4>(file, header.add_uv_number, header.bone_index_size);
	}
	
	return 0;
}