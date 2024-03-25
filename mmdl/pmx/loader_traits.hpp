#pragma once
#include<vector>
#include"buffer.hpp"

namespace mmdl::pmx
{
	//
	// header
	//

	template<typename T>
	struct header_traits
	{
		static T construct(header_buffer const&);
	};


	//
	// info
	//

	template<typename T>
	struct info_traits
	{
		using char_type = void;

		template<std::size_t CharBufferSize>
		static T construct(info_buffer<char_type, CharBufferSize> const&);
	};


	//
	// vertex
	//

	template<typename T>
	struct vertex_traits
	{
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f�𖖔��ɒǉ�
		static void emplace_back(T& model_vertex_data, vertex_buffer const&, std::uint8_t additional_uv_num);
	};


	//
	// surface
	//

	template<typename T>
	struct surface_traits
	{
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		static void emplace_back(T& surface, std::size_t index);
	};


	//
	// texture_path
	//

	template<typename T>
	struct texture_path_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		template<std::size_t CharBufferSize>
		static void emplace_back(T& texture_path, std::size_t size, std::array<char_type, CharBufferSize> const& str);
	};


	//
	// material
	//

	template<typename T>
	struct material_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		template<std::size_t CharBufferSize>
		static void emplace_back(T& material, material_buffer<char_type, CharBufferSize> const&);
	};


	//
	// bone
	//

	template<typename T>
	struct bone_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		template<std::size_t CharBufferSize, std::size_t IKLinkBufferSize>
		static void emplace_back(T& bone, bone_buffer<char_type, CharBufferSize, IKLinkBufferSize> const&);
	};


	//
	// morph
	//

	// ���[�t�̃g���C�c
	template<typename T>
	struct morph_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// ���[�t��ǉ�
		template<std::size_t CharBufferNum, std::size_t MorphDataNum>
		static void emplace_back(T&, morph_buffer<char_type, CharBufferNum, MorphDataNum> const&);
	};


	//
	// rigidbody
	//

	template<typename T>
	struct rigidbody_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// ���̂�ǉ�
		template<std::size_t CharBufferNum>
		static void emplace_back(T&, rigidbody_buffer<char_type, CharBufferNum> const&);
	};


	//
	// Joint
	//

	template<typename T>
	struct joint_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �W���C���g��ǉ�
		template<std::size_t CharBufferNum>
		static void emplace_back(T&, joint_buffer<char_type, CharBufferNum> const&);
	};

}