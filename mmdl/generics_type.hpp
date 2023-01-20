#pragma once
#include<concepts>
#include<type_traits>
#include<string>

namespace mmdl
{
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