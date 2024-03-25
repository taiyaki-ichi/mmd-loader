#pragma once
#include<istream>
#include<regex>
#include"loader_traits.hpp"
#include"buffer.hpp"

namespace mmdl
{
	template<typename T, typename traits = vpd_data_traits<T>, std::size_t CharBufferSize = 256>
	T load_vpd_data(std::istream& in)
	{
		T result;

		// 一時的なバッファとして利用
		std::string str1{};
		str1.resize(CharBufferSize);
		std::string str2{};
		str2.resize(CharBufferSize);

		vpd_buffer buffer{};

		std::regex commentout{ "(.*)//(.*)" };
		std::regex osm{ "\\w+\\.osm" };
		std::regex bone_num{ "(\\d+);(.*)" };
		std::regex bone_head{ "(\\w+)\\{(.*)" };

		// 
		std::smatch m;

		// 
		std::size_t i = 0;
		while (std::getline(in, str1))
		{
			if (std::regex_match(str1, m, commentout))
			{
				// 
				str1 = m[1].str();
			}

			if (std::regex_match(str1, m, osm))
			{

			}
			else if (std::regex_match(str1, m, bone_num))
			{
				result = traits::construct(static_cast<std::size_t>(std::stoi(m[1].str())));
			}
			else if (std::regex_match(str1, m, bone_head))
			{
				// 
				char dummy;

				// 
				buffer.name = m[2].str();

				std::getline(in, str2);

				if (std::regex_match(str2, m, commentout))
				{
					// 
					str2 = m[1].str();
				}

				// 
				std::stringstream ss1{ str2 };
				ss1 >> buffer.transform[0];
				ss1 >> dummy;
				ss1 >> buffer.transform[1];
				ss1 >> dummy;
				ss1 >> buffer.transform[2];
				ss1 >> dummy;

				in >> str2;
				if (std::regex_match(str2, m, commentout))
				{
					// 
					str2 = m[1].str();
				}

				// 
				std::stringstream ss2{ str2 };
				ss2 >> buffer.quaternion[0];
				ss2 >> dummy;
				ss2 >> buffer.quaternion[1];
				ss2 >> dummy;
				ss2 >> buffer.quaternion[2];
				ss2 >> dummy;
				ss2 >> buffer.quaternion[3];
				ss2 >> dummy;

				in >> str2;

				traits::emplace_back(result, buffer);

				i++;
			}
		}

		return result;
	}

}