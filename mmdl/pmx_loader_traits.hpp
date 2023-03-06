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

	// ���_���[�t
	template<typename T>
	struct pmx_vertex_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_vertex_morph_buffer const&);
	};

	// uv���[�t
	template<typename T>
	struct pmx_uv_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_uv_morph_buffer const&);
	};

	// �{�[�����[�t
	template<typename T>
	struct pmx_bone_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_bone_morph_buffer const&);
	};

	// �}�e���A�����[�t
	template<typename T>
	struct pmx_material_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_material_buffer const&);
	};

	// �O���[�v���[�t
	template<typename T>
	struct pmx_group_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_group_morph_buffer const&);
	};

	// �ǉ�uv1���[�t
	template<typename T>
	struct pmx_addtional_uv_1_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_uv_morph_buffer const&);
	};

	// �ǉ�uv2���[�t
	template<typename T>
	struct pmx_addtional_uv_2_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_uv_morph_buffer const&);
	};

	// �ǉ�uv3���[�t
	template<typename T>
	struct pmx_addtional_uv_3_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_uv_morph_buffer const&);
	};

	// �ǉ�uv4���[�t
	template<typename T>
	struct pmx_addtional_uv_4_morph_traits
	{
		using value_type = void;
		using char_type = void;

		// �\�z
		template<std::size_t CharBufferSize>
		static T construct(
			std::int32_t name_size,
			std::array<char_type, CharBufferSize> const& name,
			std::int32_t english_name_size,
			std::array<char_type, CharBufferSize> const& english_name,
			std::uint8_t control_panel_option,/*����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��*/
			std::int32_t size);

		// �v�f�̒ǉ�
		static void emplace_back(T& vertex_morph, pmx_uv_morph_buffer const&);
	};

	// ���[�t�̃g���C�c
	template<typename T>
	struct pmx_morph_traits
	{
		using vertex_morph_traits = pmx_bone_morph_traits<T>;
		using uv_morph_traits = pmx_uv_morph_traits<T>;
		using bone_morph_traits = pmx_bone_morph_traits<T>;
		using material_morph_traits = pmx_material_morph_traits<T>;
		using group_morph_traits = pmx_group_morph_traits<T>;
		using addtional_uv_1_morph_traits = pmx_addtional_uv_1_morph_traits<T>;
		using addtional_uv_2_morph_traits = pmx_addtional_uv_2_morph_traits<T>;
		using addtional_uv_3_morph_traits = pmx_addtional_uv_3_morph_traits<T>;
		using addtional_uv_4_morph_traits = pmx_addtional_uv_4_morph_traits<T>;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// ���_���[�t��ǉ�
		static void emplace_back_vertex_morph(T&, vertex_morph_traits::value_type&&);

		// UV���[�t��ǉ�
		static void emplace_back_uv_morph(T&, uv_morph_traits::value_type&&);

		// �{�[�����[�t��ǉ�
		static void emplace_back_bone_morph(T&, bone_morph_traits::value_type&&);

		// �}�e���A�����[�t��ǉ�
		static void emplace_back_material_morph(T&, material_morph_traits::value_type&&);

		// �O���[�v���[�t��ǉ�
		static void emplace_back_group_morph(T&, group_morph_traits::value_type&&);

		// �ǉ�UV1���[�t��ǉ�
		static void emplace_back_adittional_uv_1_morph(T&, addtional_uv_1_morph_traits::value_type&&);

		// �ǉ�UV2���[�t��ǉ�
		static void emplace_back_adittional_uv_2_morph(T&, addtional_uv_2_morph_traits::value_type&&);

		// �ǉ�UV3���[�t��ǉ�
		static void emplace_back_adittional_uv_3_morph(T&, addtional_uv_3_morph_traits::value_type&&);

		// �ǉ�UV4���[�t��ǉ�
		static void emplace_back_adittional_uv_4_morph(T&, addtional_uv_4_morph_traits::value_type&&);

	};

}