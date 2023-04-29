#pragma once
#include<vector>
#include"pmx_buffer.hpp"

namespace mmdl
{
	//
	// header
	//

	template<typename T>
	struct pmx_header_traits
	{
		static T construct(pmx_header_buffer const&);
	};


	//
	// info
	//

	template<typename T>
	struct pmx_info_traits
	{
		using char_type = void;

		template<std::size_t CharBufferSize>
		static T construct(pmx_info_buffer<char_type, CharBufferSize> const&);
	};


	//
	// vertex
	//

	template<typename T>
	struct pmx_vertex_traits
	{
		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を末尾に追加
		static void emplace_back(T& model_vertex_data, pmx_vertex_buffer const&, std::uint8_t additional_uv_num);
	};


	//
	// surface
	//

	template<typename T>
	struct pmx_surface_traits
	{
		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		static void emplace_back(T& surface, std::size_t index);
	};


	//
	// texture_path
	//

	template<typename T>
	struct pmx_texture_path_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		template<std::size_t CharBufferSize>
		static void emplace_back(T& texture_path, std::size_t size, std::array<char_type, CharBufferSize> const& str);
	};


	//
	// material
	//

	template<typename T>
	struct pmx_material_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		template<std::size_t CharBufferSize>
		static void emplace_back(T& material, pmx_material_buffer<char_type, CharBufferSize> const&);
	};


	//
	// bone
	//

	template<typename T>
	struct pmx_bone_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		template<std::size_t CharBufferSize, std::size_t IKLinkBufferSize>
		static void emplace_back(T& bone, pmx_bone_buffer<char_type, CharBufferSize, IKLinkBufferSize> const&);
	};


	//
	// morph
	//

	// モーフのトレイツ
	template<typename T>
	struct pmx_morph_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// モーフを追加
		template<std::size_t CharBufferNum, std::size_t MorphDataNum>
		static void emplace_back(T&, pmx_morph_buffer<char_type, CharBufferNum, MorphDataNum> const&);
	};


	//
	// rigidbody
	//

	template<typename T>
	struct pmx_rigidbody_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 剛体を追加
		template<std::size_t CharBufferNum>
		static void emplace_back(T&, pmx_rigidbody_buffer<char_type, CharBufferNum> const&);
	};


	//
	// Joint
	//

	template<typename T>
	struct pmx_joint_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// ジョイントを追加
		template<std::size_t CharBufferNum>
		static void emplace_back(T&, pmx_joint_buffer<char_type, CharBufferNum> const&);
	};

}