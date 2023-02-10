#pragma once
#include<concepts>
#include<type_traits>
#include<string>

namespace mmdl
{
	template<typename T>
	struct pmx_header_traits
	{
		static T construct(
			// バージョン
			float version,
			// エンコード方式
			// 0:UTF16, 1:UTF8
			std::uint8_t encode,
			// 追加UV数
			std::size_t add_uv_num,
			// 頂点Indexサイズ
			std::size_t vertex_index_size,
			// テクスチャIndexサイズ
			std::size_t texture_index_size,
			// 材質Indexサイズ
			std::size_t material_index_size,
			// ボーンIndexサイズ
			std::size_t bone_index_size,
			// モーフIndexサイズ
			std::size_t morph_index_size,
			// 剛体Indexサイズ
			std::size_t rigid_body_index_size
			);
	};

	template<typename T>
	struct pmx_info_traits
	{
		using char_type = void;

		static T construct(
			// モデル名
			char_type* model_name_str, std::size_t mode_name_str_size,
			// モデル英名
			char_type* english_model_name_str, std::size_t english_mode_name_str_size,
			// コメント
			char_type* comment_str, std::size_t comment_str_size,
			// コメント英
			char_type* english_comment_str, std::size_t english_comment_str_size
			);
	};




	// プライマリテンプレート
	template<typename, typename>
	struct count_construct_container_traits {};

	// サイズ指定し構築可能で、添え字演算子で参照を返す型についての特殊化
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

		// サイズを指定して構築
		static T construct(SizeType n) {
			return T(n);
		}

		// 添え字から参照の取得
		static value_type& get_reference(T& t, SizeType i) {
			return t[i];
		}
	};

	// 文字列を表す型についての特殊化
	template <typename CharT>
	struct count_construct_container_traits<std::basic_string<CharT>, std::size_t>
	{
		using value_type = std::basic_string<CharT>::value_type;
		using size_type = std::size_t;

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