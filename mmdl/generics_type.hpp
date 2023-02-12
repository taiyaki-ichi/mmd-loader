#pragma once
#include<concepts>
#include<type_traits>
#include<string>
#include"pmx_buffer.hpp"

namespace mmdl
{
	template<typename T>
	struct pmx_header_traits
	{
		static T construct(pmx_header_buffer const&);
	};

	template<typename T>
	struct pmx_info_traits
	{
		using char_type = void;

		template<std::size_t CharBufferSize>
		static T construct(pmx_info_buffer<char_type, CharBufferSize> const&);
	};

	template<typename T>
	struct pmx_vertex_traits
	{
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f�𖖔��ɒǉ�
		static void emplace_back(T& vertex, pmx_vertex_buffer const&);
	};


	template<typename T>
	struct pmx_surface_traits
	{
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		static void emplace_back(T& surface, std::size_t index);
	};


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

	template<typename T>
	struct pmx_bone_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		template<std::size_t CharBufferSize, std::size_t IKLinkBufferSize >
		static void emplace_back(T& bone, pmx_bone_buffer<char_type, CharBufferSize, IKLinkBufferSize> const&);
	};



	// �v���C�}���e���v���[�g
	template<typename, typename>
	struct count_construct_container_traits {};

	// �T�C�Y�w�肵�\�z�\�ŁA�Y�������Z�q�ŎQ�Ƃ�Ԃ��^�ɂ��Ă̓��ꉻ
	template<typename T, typename SizeType>
		requires (std::constructible_from<T, SizeType>&&
			requires(T& t, SizeType i) {
		typename T::value_type;
		{t[i]} ->  std::convertible_to<std::add_lvalue_reference_t<typename T::value_type>>;
	})
		struct count_construct_container_traits<T, SizeType>
	{
		using value_type = T::value_type;
		using size_type = SizeType;

		// �T�C�Y���w�肵�č\�z
		static T construct(SizeType n) {
			return T(n);
		}

		// �Y��������Q�Ƃ̎擾
		static value_type& get_reference(T& t, SizeType i) {
			return t[i];
		}
	};

	// �������\���^�ɂ��Ă̓��ꉻ
	template <typename CharT>
	struct count_construct_container_traits<std::basic_string<CharT>, std::size_t>
	{
		using value_type = std::basic_string<CharT>::value_type;
		using size_type = std::size_t;

		// �T�C�Y���w�肵�č\�z
		static std::basic_string<CharT> construct(std::size_t n) {
			return std::basic_string<CharT>(n, CharT{});
		}

		// �Y��������Q�Ƃ̎擾
		static value_type& get_reference(std::basic_string<CharT>& str, std::size_t i) {
			return str[i];
		}
	};

	// 2�����x�N�g�����\�z�\�ł��邱�Ƃ�\���g���C�c
	template<typename T>
		requires std::constructible_from<T, float, float>&& std::move_constructible<T>
	struct constructible_vec2_traits
	{
		static T construct(float x, float y)
		{
			return T(x, y);
		}
	};

	// �\�z�\��2�����x�N�g���̃R���Z�v�g
	template<typename T>
	concept constructible_vec2 = requires
	{
		constructible_vec2_traits<T>::construct(std::declval<float>(), std::declval<float>());
	};

	// 3�����x�N�g�����\�z�\�ł��邱�Ƃ�\���g���C�c
	template<typename T>
		requires std::constructible_from<T, float, float, float>&& std::move_constructible<T>
	struct constructible_vec3_traits
	{
		static T construct(float x, float y, float z)
		{
			return T(x, y, z);
		}
	};

	// �\�z�\��3�����x�N�g���̃R���Z�v�g
	template<typename T>
	concept constructible_vec3 = requires
	{
		constructible_vec3_traits<T>::construct(std::declval<float>(), std::declval<float>(), std::declval<float>());
	};


	// 4�����x�N�g�����\�z�\�ł��邱�Ƃ�\���g���C�c
	template<typename T>
		requires std::constructible_from<T, float, float, float, float>&& std::move_constructible<T>
	struct constructible_vec4_traits
	{
		static T construct(float x, float y, float z, float w)
		{
			return T(x, y, z, w);
		}
	};

	// �\�z�\��4�����x�N�g���̃R���Z�v�g
	template<typename T>
	concept constructible_vec4 = requires
	{
		constructible_vec4_traits<T>::construct(std::declval<float>(), std::declval<float>(), std::declval<float>(), std::declval<float>());
	};

}