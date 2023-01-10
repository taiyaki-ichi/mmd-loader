#pragma once
#include"vpd_data.hpp"
#include"generics_type.hpp"
#include<istream>
#include<regex>

namespace mmdl
{
	template<template<typename>typename Container, typename Str, typename Vec3, typename Vec4, typename ContainerSizeType = std::size_t, typename StrSizeType = std::size_t>
	Container<vpd_data<Str, Vec3, Vec4>> load_vpd_data(std::istream& in)
	{
		using container_type = Container<vpd_data<Str, Vec3, Vec4>>;
		using container_traits = resizable_container_traits<container_type, ContainerSizeType>;

		Container<vpd_data<Str, Vec3, Vec4>> result;

		Str str1;
		resizable_container_traits<Str, StrSizeType>::resize(str1, static_cast<StrSizeType>(256));

		Str str2;
		resizable_container_traits<Str, StrSizeType>::resize(str2, static_cast<StrSizeType>(256));

		// in >> str1;
		// if (str != "Vocaloid Pose Data file")
			// throw std::runtime_error{};

		std::regex commentout{ "(.*)//(.*)" };
		std::regex osm{ "\\w+\\.osm" };
		std::regex bone_num{ "(\\d+);(.*)" };
		std::regex bone_head{ "(\\w+)\\{(.*)" };

		// TODO: traits‘Î‰ž
		std::smatch m;

		// 
		std::size_t i = 0;
		while (std::getline(in, str1))
		{
			if (std::regex_match(str1, m, commentout))
			{
				// TODO: traits‘Î‰ž
				str1 = m[1].str();
			}

			if (std::regex_match(str1, m, osm))
			{

			}
			else if (std::regex_match(str1, m, bone_num))
			{
				container_traits::resize(result, static_cast<std::size_t>(std::stoi(m[1].str())));
			}
			else if (std::regex_match(str1, m, bone_head))
			{
				auto& vpd = container_traits::get(result, static_cast<container_traits::size_type>(i));

				// TODO: traits‘Î‰ž
				char dummy;

				// TODO: traits‘Î‰ž
				auto name = m[2].str();
				vpd.name = name;

				std::getline(in, str2);

				if (std::regex_match(str2, m, commentout))
				{
					// TODO: traits‘Î‰ž
					str2 = m[1].str();
				}

				// 
				std::stringstream ss1{ str2 };
				float  tmp1[3];
				ss1 >> tmp1[0];
				ss1 >> dummy;
				ss1 >> tmp1[1];
				ss1 >> dummy;
				ss1 >> tmp1[2];
				ss1 >> dummy;
				vpd.transform = { tmp1[0],tmp1[1] ,tmp1[2] };

				in >> str2;
				if (std::regex_match(str2, m, commentout))
				{
					// TODO: traits‘Î‰ž
					str2 = m[1].str();
				}

				// 
				std::stringstream ss2{ str2 };
				float tmp2[4];
				ss2 >> tmp2[0];
				ss2 >> dummy;
				ss2 >> tmp2[1];
				ss2 >> dummy;
				ss2 >> tmp2[2];
				ss2 >> dummy;
				ss2 >> tmp2[3];
				ss2 >> dummy;
				vpd.quaternion = { tmp2[0],tmp2[1] ,tmp2[2] ,tmp2[3] };

				// }
				in >> str2;

				i++;
			}
		}

		return result;
	}

}