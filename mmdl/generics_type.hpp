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
			// 追加UV数 0-4
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

	template<typename T>
	struct pmx_vertex_traits
	{
		// サイズを指定して構築
		static T construct(std::size_t size);

		// BDEF1形式のボーンの追加
		static void emplace_back_BDEF1(
			T& vertex,
			std::array<float,3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ウェイト1.0の単一ボーン
			std::size_t bone_index_1,
			// エッジ倍率
			float edge_factor
		);

		// BDEF2形式のボーンの追加
		static void emplace_back_BDEF2(
			T& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ボーン1のインデックス
			std::size_t bone_index_1,
			// ボーン2のインデックス
			std::size_t bone_index_2,
			// ボーン1のウェイト(ボーン2のウェイトは 1.0 - ボーン1のウェイト )
			float bone_weight_1,
			// エッジ倍率
			float edge_factor
		);

		// BDEF4形式のボーンの追加
		static void emplace_back_BDEF4(
			T& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ボーン1のインデックス
			std::size_t bone_index_1,
			// ボーン2のインデックス
			std::size_t bone_index_2,
			// ボーン3のインデックス
			std::size_t bone_index_3,
			// ボーン4のインデックス
			std::size_t bone_index_4,
			// ボーン1のウェイト
			float bone_weight_1,
			// ボーン2のウェイト
			float bone_weight_2,
			// ボーン3のウェイト
			float bone_weight_3,
			// ボーン4のウェイト（ウェイトの合計について1の保証はない）
			float bone_weight_4,
			// エッジ倍率
			float edge_factor
		);

		// SDEF形式のボーンの追加
		static void emplace_back_SDEF(
			T& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// ボーン1のインデックス
			std::size_t bone_index_1,
			// ボーン2のインデックス
			std::size_t bone_index_2,
			// ボーン1のウェイト(ボーン2のウェイトは 1.0 - ボーン1のウェイト )
			float bone_weight_1,
			// SDEF-C値(x,y,z)
			std::array<float, 3> const& sdef_c,
			// SDEF-R0値(x,y,z)
			std::array<float, 3> const& sdef_r0,
			// SDEF-R1値(x,y,z) ※修正値を要計算
			std::array<float, 3> const& sdef_r1,
			// エッジ倍率
			float edge_factor
		);

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
		static void emplace_back(T& texture_path, char_type const* str, std::size_t size);
	};

	template<typename T>
	struct pmx_material_traits
	{
		using char_type = void;

		// サイズを指定して構築
		static T construct(std::size_t size);

		// 要素を追加
		static void emplace_back(T& material,
			char_type const* name, std::size_t name_size,
			char_type const* english_name, std::size_t english_name_size,
			std::array<float, 4> const& diffuse,
			std::array<float, 3> const& specular,
			float specularity,
			std::array<float, 3> const& ambient,
			// 描画フラグ 0x01:両面描画, 0x02:地面影, 0x04:セルフシャドウマップへの描画, 0x08:セルフシャドウの描画, 0x10:エッジ描画
			std::uint8_t bit_flag,
			std::array<float, 4> const& edge_color,
			float edge_size,
			std::size_t texture_index,
			std::size_t sphere_texture_index,
			// スフィアモード 0:無効 1:乗算(sph) 2:加算(spa) 3:サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
			std::uint8_t sphere_mode,
			// 共有トゥーンフラグ 
			std::uint8_t toon_flag,
			// 共有トゥーンフラグが0: テクスチャのインデックス
			// 共有トゥーンフラグが1: 0..9 -> toon01.bmp...toon10.bmp に対応
			std::size_t toon_index,
			char_type const* memo, std::size_t memo_size,
			// マテリアルに対応する頂点の数
			std::size_t vertex_num
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