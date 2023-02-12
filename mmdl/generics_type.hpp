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

		template<typename CharType, std::size_t CharBufferSize>
		static T construct(pmx_info_buffer<CharType, CharBufferSize> const&);
	};

	template<typename T>
	struct pmx_vertex_traits
	{
		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を末尾に追加
		static void emplace_back(T& vertex, pmx_vertex_buffer const&);
	};


	template<typename T>
	struct pmx_surface_traits
	{
		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		static void emplace_back(T& surface, std::size_t index);
	};


	template<typename T>
	struct pmx_texture_path_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		template<std::size_t CharBufferSize>
		static void emplace_back(T& texture_path, std::array<char_type, CharBufferSize> const& str, std::size_t size);
	};

	template<typename T>
	struct pmx_material_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		template<typename CharType, std::size_t CharBufferSize>
		static void emplace_back(T& material, pmx_material_buffer<CharType, CharBufferSize> const&);
	};

	template<typename T>
	struct pmx_bone_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		static void emplace_back(T& bone,
			char_type* name, std::size_t name_size,
			char_type* english_name, std::size_t english_name_size,
			std::array<float, 3> const& position,
			std::size_t parent_bone_index,
			std::size_t transformation_level,
			/*
			○ボーンフラグ
				0x0001  : 接続先(PMD子ボーン指定)表示方法 -> 0:座標オフセットで指定 1:ボーンで指定

				0x0002  : 回転可能
				0x0004  : 移動可能
				0x0008  : 表示
				0x0010  : 操作可

				0x0020  : IK

				0x0080  : ローカル付与 | 付与対象 0:ユーザー変形値／IKリンク／多重付与 1:親のローカル変形量
				0x0100  : 回転付与
				0x0200  : 移動付与

				0x0400  : 軸固定
				0x0800  : ローカル軸

				0x1000  : 物理後変形
				0x2000  : 外部親変形
			*/
			std::uint16_t bone_flag,
			// 接続先のビットが0の場合に使用する座標オフセット
			std::array<float, 3> const& access_point_offset,
			// 接続先のビットが1の場合に使用する接続先のボーンのインデックス
			std::size_t access_bone_index,
			// 回転付与または移動付与のビットが1の場合に使用する付与親のボーンのインデックス
			std::size_t assign_bone_index,
			// 回転付与または移動付与のビットが1の場合に使用する付与率
			std::size_t assign_param,
			// 軸固定が1のビットが1の場合に使用する軸の方向ベクトル
			std::array<float, 3> const& fixed_direction,
			// 外部親変形のビットが1の場合に使用するKey値
			std::size_t key_value,
			// 以下、IKのビットが1の場合に使用する
			// IKターゲットのボーン
			std::size_t ik_target_bone,
			// IKのループの回数
			std::size_t ik_loop_num,
			// IKの1回あたりの角度制限
			float ik_angle_limit_par_process,
			// IKリンクの数
			std::size_t ik_link_size,
			// IKリンク
			// ボーンのインデックス、角度制限の有無、角度制限の下限、角度制限の上限、、、の順
			std::tuple<std::size_t, std::uint8_t, std::array<float, 3>, std::array<float, 3>>* ik_link
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