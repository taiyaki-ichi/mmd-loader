#pragma once
#include<concepts>
#include<type_traits>

namespace mmdl
{

	// �W�����C�u�����Ȃǂ̃T�C�Y�w��\�ȃR���e�i
	template<typename T, typename U>
	concept standard_resizable_container = requires(T t, U u)
	{
		typename T::value_type;
		t.resize(u);
		{t[u]} ->  std::convertible_to<std::add_lvalue_reference_t<typename T::value_type>>;
	};

	// �T�C�Y�w��\�ȃR���e�i�̃g���C�c
	template<typename T, typename U>
	requires standard_resizable_container<T, U>
		struct resizable_container_traits
	{
		using value_type = typename T::value_type;
		using size_type = U;

		static void resize(T& t, size_type u)
		{
			t.resize(u);
		}

		static value_type& get(T& t, size_type u)
		{
			return t[u];
		}

	};

	// �T�C�Y�w��\�ȃR���e�i
	template<typename T, typename U>
	concept resizable_container = requires(T & t, U u)
	{
		typename resizable_container_traits<T, U>::value_type;
		resizable_container_traits<T, U>::resize(t, u);
		resizable_container_traits<T, U>::get(t, u);
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