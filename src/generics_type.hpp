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

		static void resize(T& t, U u)
		{
			t.resize(u);
		}

		static value_type& get(T& t, U u)
		{
			return t[u];
		}

	};

	// サイズ指定可能なコンテナ
	template<typename T, typename U>
	concept resizable_container = requires(T & t, U u)
	{
		resizable_container_traits<T, U>::resize(t, u);
		resizable_container_traits<T, U>::get(t, u);
	};



}