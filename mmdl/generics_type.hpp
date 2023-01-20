#pragma once
#include<concepts>
#include<type_traits>
#include<string>

namespace mmdl
{
	// プライマリテンプレート
	template<typename>
	struct count_construct_container_traits {};

	// サイズ指定し構築可能で、添え字演算子で参照を返す型についての特殊化
	template<typename T>
		requires (std::constructible_from<T, std::size_t>&&
		requires(T& t, std::size_t i) {
		typename T::value_type;
		{t[i]} ->  std::convertible_to<std::add_lvalue_reference_t<typename T::value_type>>;
	})
	struct count_construct_container_traits<T>
	{
		using value_type = T::value_type;

		// サイズを指定して構築
		static T construct(std::size_t n) {
			return T(n);
		}

		// 添え字から参照の取得
		static value_type& get_reference(T& t, std::size_t i) {
			return t[i];
		}
	};

	// 文字列を表す型についての特殊化
	template <typename CharT>
	struct count_construct_container_traits<std::basic_string<CharT>>
	{
		using value_type = std::basic_string<CharT>::value_type;

		// サイズを指定して構築
		static std::basic_string<CharT> construct(std::size_t n) {
			return std::basic_string<CharT>(n, CharT{});
		}

		// 添え字から参照の取得
		static value_type& get_reference(std::basic_string<CharT>& str, std::size_t i) {
			return str[i];
		}
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