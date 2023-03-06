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
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f�𖖔��ɒǉ�
		static void emplace_back(T& model_vertex_data, pmx_vertex_buffer const&, std::uint8_t additional_uv_num);
	};


	//
	// surface
	//

	template<typename T>
	struct pmx_surface_traits
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
	struct pmx_texture_path_traits
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
	struct pmx_material_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
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

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		template<std::size_t CharBufferSize, std::size_t IKLinkBufferSize>
		static void emplace_back(T& bone, pmx_bone_buffer<char_type, CharBufferSize, IKLinkBufferSize> const&);
	};

	//
	// morph
	//

	// �e���[�t�̃g���C�c
	template<typename T, typename Buffer>
	struct pmx_morph_element_traits
	{
		using value_type = void;

		// �\�z
		template<typename CharType, std::size_t CharBufferSize>
		static T construct(
			std::array<CharType, CharBufferSize> const& name,
			std::array<CharType, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, Buffer const&);
	};

	// ���ꂼ��̃��[�t�̃C���X�^���X

	// ���_���[�t
	template<typename T>
	using pmx_vertex_morph_traits = pmx_morph_element_traits<T, pmx_vertex_morph_buffer>;

	// uv���[�t
	template<typename T>
	using pmx_uv_morph_traits = pmx_morph_element_traits<T, pmx_uv_morph_buffer>;

	// �{�[�����[�t
	template<typename T>
	using pmx_bone_morph_traits = pmx_morph_element_traits<T, pmx_bone_morph_buffer>;

	// �}�e���A�����[�t
	template<typename T>
	using pmx_material_morph_traits = pmx_morph_element_traits<T, pmx_material_morph_buffer>;

	// ���[�t�̃g���C�c
	template<typename T,
		typename VertexMorphTraits = pmx_vertex_morph_traits<T>,
		typename UVMorphTraits = pmx_uv_morph_traits<T>,
		typename BoneMorphTraits = pmx_bone_morph_traits<T>,
		typename MaterialMorphTraits = pmx_material_morph_traits<T>
	>
	struct pmx_morph_traits
	{
		using vertex_morph_traits = VertexMorphTraits;
		using uv_morph_traits = UVMorphTraits;
		using bone_morph_traits = BoneMorphTraits;
		using material_morph_traits = MaterialMorphTraits;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// ���_���[�t��ǉ�
		static void emplace_back(T&, vertex_morph_traits::value_type&&);

		// UV���[�t��ǉ�
		static void emplace_back(T&, uv_morph_traits::value_type&&);

		// �{�[�����[�t��ǉ�
		static void emplace_back(T&, bone_morph_traits::value_type&&);

		// �}�e���A�����[�t��ǉ�
		static void emplace_back(T&, material_morph_traits::value_type&&);

	};

}