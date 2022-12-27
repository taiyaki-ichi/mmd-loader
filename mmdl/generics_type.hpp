#pragma once
#include<concepts>
#include<type_traits>

namespace mmdl
{

	// 標準ライブラリなどのサイズ指定可能なコンテナ
	template<typename T, typename U>
	concept standard_resizable_container = requires(T t, U u)
	{
		typename T::value_type;
		t.resize(u);
		{t[u]} ->  std::convertible_to<std::add_lvalue_reference_t<typename T::value_type>>;
	};

	// サイズ指定可能なコンテナのトレイツ
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

	// サイズ指定可能なコンテナ
	template<typename T, typename U>
	concept resizable_container = requires(T & t, U u)
	{
		typename resizable_container_traits<T, U>::value_type;
		resizable_container_traits<T, U>::resize(t, u);
		resizable_container_traits<T, U>::get(t, u);
	};


	// 2次元ベクトルが構築可能であることを表すトレイツ
	template<typename T>
	requires std::constructible_from<T, float, float>&& std::move_constructible<T>
		struct constructible_vec2_traits
	{
		static T construct(float x, float y)
		{
			return T(x, y);
		}
	};

	// 構築可能な2次元ベクトルのコンセプト
	template<typename T>
	concept constructible_vec2 = requires
	{
		constructible_vec2_traits<T>::construct(std::declval<float>(), std::declval<float>());
	};


	// 3次元ベクトルが構築可能であることを表すトレイツ
	template<typename T>
	requires std::constructible_from<T, float, float, float>&& std::move_constructible<T>
		struct constructible_vec3_traits
	{
		static T construct(float x, float y, float z)
		{
			return T(x, y, z);
		}
	};

	// 構築可能な3次元ベクトルのコンセプト
	template<typename T>
	concept constructible_vec3 = requires
	{
		constructible_vec3_traits<T>::construct(std::declval<float>(), std::declval<float>(), std::declval<float>());
	};


	// 4次元ベクトルが構築可能であることを表すトレイツ
	template<typename T>
	requires std::constructible_from<T, float, float, float, float>&& std::move_constructible<T>
		struct constructible_vec4_traits
	{
		static T construct(float x, float y, float z, float w)
		{
			return T(x, y, z, w);
		}
	};

	// 構築可能な4次元ベクトルのコンセプト
	template<typename T>
	concept constructible_vec4 = requires
	{
		constructible_vec4_traits<T>::construct(std::declval<float>(), std::declval<float>(), std::declval<float>(), std::declval<float>());
	};

}