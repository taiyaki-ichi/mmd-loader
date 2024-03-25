#pragma once
#include<array>
#include<variant>

namespace mmdl::pmx
{
	struct header_buffer
	{
		// バージョン
		float version{};

		// エンコード
		//  0:UTF16 1:UTF8
		std::uint8_t encode{};

		// 追加UVの数
		// 0-4
		std::uint8_t additional_uv_num{};

		// 頂点のインデックスのサイズ
		// 1,2,4のいずれか
		std::uint8_t vertex_index_size{};

		// テクスチャのインデックスのサイズ
		std::uint8_t texture_index_size{};

		// テクスチャのインデックスのサイズ
		// 1,2,4のいずれか
		std::uint8_t material_index_size{};

		// ボーンのインデックスのサイズ
		// 1,2,4のいずれか
		std::uint8_t bone_index_size{};

		// モーフのインデックスの大きさ
		// 1,2,4のいずれか
		std::uint8_t morph_index_size{};

		// 剛体のインデックスの大きさ
		std::uint8_t rigid_body_index_size{};
	};


	template<typename CharType, std::size_t CharBufferSize>
	struct info_buffer
	{
		// モデルの名前
		std::int32_t model_name_size{};
		std::array<CharType, CharBufferSize> model_name{};

		// 英語のモデルの名前
		std::int32_t english_model_name_size{};
		std::array<CharType, CharBufferSize> english_model_name{};

		// コメント
		std::int32_t comment_size{};
		std::array<CharType, CharBufferSize> comment{};

		// 英語のコメント
		std::int32_t english_comment_size{};
		std::array<CharType, CharBufferSize> english_comment{};
	};


	struct vertex_buffer
	{
		// 位置
		std::array<float, 3> position{};

		// 法線
		std::array<float, 3> normal{};

		// UV
		std::array<float, 2> uv{};

		// 追加UV
		std::array<std::array<float, 4>, 4> additional_uv{};

		// ウェイト変形形式
		// 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF
		std::uint8_t weight_type{};

		// ボーンのインデックス
		std::array<std::size_t, 4> bone_index{};

		// ボーンのウェイト
		std::array<float, 4> bone_weight{};

		// SDEF-C値
		std::array<float, 3> sdef_c{};

		// SDEF-R0値
		std::array<float, 3> sdef_r0{};

		// SDEF-R1値
		std::array<float, 3> sdef_r1{};

		// エッジ倍率
		float edge_factor{};
	};


	template<typename CharType, std::size_t CharBufferSize>
	struct material_buffer
	{
		// マテリアルの名前
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// 英語のマテリアルの名前
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// ディフューズ
		std::array<float, 4> diffuse{};
		// スぺキュラ
		std::array<float, 3> specular{};
		// スぺキュラ係数
		float specularity{};
		// アンビエント
		std::array<float, 3> ambient{};

		/*
		描画フラグ
			0x01:両面描画
			0x02:地面影
			0x04:セルフシャドウマップへの描画
			0x08:セルフシャドウの描画
			0x10:エッジ描画
		*/
		std::uint8_t bit_flag{};

		// エッジの色
		std::array<float, 4> edge_color{};
		// エッジの大きさ
		float edge_size{};

		// 通常のテクスチャのインデックス
		std::size_t texture_index{};
		// スフィアテクスチャのインデックス
		std::size_t sphere_texture_index{};

		/*
		スフィアモード
			0:無効
			1:乗算(sph)
			2:加算(spa) 3:サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
		*/
		std::uint8_t sphere_mode{};

		// 共有トゥーンフラグ 
		std::uint8_t toon_flag{};
		// 共有トゥーンフラグが0: テクスチャのインデックス
		// 共有トゥーンフラグが1: 0..9 -> toon01.bmp...toon10.bmp に対応
		std::size_t toon_index{};

		// メモ
		std::int32_t memo_size{};
		std::array<CharType, CharBufferSize> memo{};

		// マテリアルの対応する頂点の数
		//  (必ず3の倍数になる)
		std::int32_t vertex_num{};
	};


	template<typename CharType, std::size_t CharBufferNum, std::size_t IKLinkBufferNum>
	struct bone_buffer
	{
		// 名前
		std::int32_t name_size{};
		std::array<CharType, CharBufferNum> name{};

		// 英語の名前
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferNum> english_name{};

		// 位置
		std::array<float, 3> position{};

		// 親ボーンのインデックス
		std::size_t parent_bone_index{};

		// 変形階層
		std::int32_t transformation_level{};

		/*
		ボーンフラグ
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
		std::uint16_t bone_flag{};

		// 接続先のビットが0の場合に使用する座標オフセット
		std::array<float, 3> access_point_offset{};
		// 接続先のビットが1の場合に使用する接続先のボーンのインデックス
		std::size_t access_bone_index{};
		// 回転付与または移動付与のビットが1の場合に使用する付与親のボーンのインデックス
		std::size_t assign_bone_index{};
		// 回転付与または移動付与のビットが1の場合に使用する付与率
		float assign_rate{};
		// 軸固定のビットが1の場合に使用する軸の方向ベクトル
		std::array<float, 3> fixed_axis_direction{};
		// ローカル軸のビットが1の場合に使用するX軸の方向ベクトル
		std::array<float, 3> local_x_axis_direction{};
		// ローカル軸のビットが1の場合に使用するZ軸の方向ベクトル
		std::array<float, 3> local_z_axis_direction{};
		// 外部親変形のビットが1の場合に使用するKey値
		std::int32_t key_value{};

		// 以下、IKのビットが1の場合に使用する
		// IKターゲットのボーン
		std::size_t ik_target_bone{};
		// IKのループ回数
		std::int32_t ik_loop_num{};
		// IKの1回当たりの角度制限
		float ik_angle_limit_par_process{};
		// IKリンクの数
		std::int32_t ik_link_size{};
		// ikリンクの型
		// ボーンのインデックス、角度制限の有無、角度制限の下限、角度制限の上限、、、の順
		using ik_link_type = std::tuple<std::size_t, std::uint8_t, std::array<float, 3>, std::array<float, 3>>;
		// ikリンク
		std::array<ik_link_type, IKLinkBufferNum> ik_link{};
	};


	// 頂点モーフ
	// 頂点のオフセット
	struct vertex_morph_buffer
	{
		// 対象の頂点のインデックス
		std::size_t index{};

		// 座標のオフセット値
		std::array<float, 3> offset{};
	};

	// uvモーフ
	// uvオフセット値
	// 通常UVはz,wが不要項目になるがモーフとしてのデータ値は記録しておく
	struct uv_morph_buffer
	{
		// 対象の頂点のインデックス
		std::size_t index{};

		// uvのオフセット値
		std::array<float, 4> offset{};
	};

	// ボーンモーフ
	struct bone_morph_buffer
	{
		// 対象のボーンのインデックス
		std::size_t index{};

		// 移動量
		std::array<float, 3> transform{};

		// 回転量（クォータニオン）
		std::array<float, 4> quaternion{};
	};

	// 材質モーフ
	struct material_morph_buffer
	{
		// 対象のマテリアルのインデックス
		std::size_t index{};

		// オフセット形式
		//  0:乗算, 1:加算
		std::uint8_t offset_type{};

		// ディフューズ
		std::array<float, 4> diffuse{};
		// スぺキュラ
		std::array<float, 3> specular{};
		// スぺキュラ係数
		float specularity{};
		// アンビエント
		std::array<float, 3> ambient{};

		// エッジの色
		std::array<float, 4> edge_color{};
		// エッジの大きさ
		float edge_size{};

		// テスクチャ係数
		std::array<float, 4> texture_factor{};

		// スフィアテクスチャ係数
		std::array<float, 4> sphere_texture_factor{};

		// トゥーンテクスチャ係数
		std::array<float, 4> toon_texture_factor{};

	};

	// グループモーフ
	struct group_morph_buffer
	{
		// 対象のモーフのインデックス
		std::size_t index{};

		// モーフ率
		// グループモーフのモーフ値 * モーフ率 = 対象モーフのモーフ値
		float morph_factor{};
	};

	// モーフのバッファ
	template<typename CharType, std::size_t CharBufferSize,std::size_t MorphBufferNum>
	struct morph_buffer
	{
		// 名前
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// 英語の名前
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// 操作パネル (PMD:カテゴリ) 1:眉(左下) 2:目(左上) 3:口(右上) 4:その他(右下)  | 0:システム予約
		std::uint8_t control_panel_option{};

		// モーフ種類 - 0:グループ, 1 : 頂点, 2 : ボーン, 3 : UV, 4 : 追加UV1, 5 : 追加UV2, 6 : 追加UV3, 7 : 追加UV4, 8 : 材質
		std::uint8_t morph_type{};

		// 以降のモーフのデータの数
		std::int32_t morph_data_num{};

		// モーフの種類ごとのデータ
		using morph_type_variant = std::variant<vertex_morph_buffer, uv_morph_buffer, bone_morph_buffer,
			material_morph_buffer, group_morph_buffer>;
		std::array<morph_type_variant, MorphBufferNum> morph_data{};

		static constexpr std::size_t VERTEX_MORPH_INDEX = 0;
		static constexpr std::size_t UV_MORPH_INDEX = 1;
		static constexpr std::size_t BONE_MORPH_INDEX = 2;
		static constexpr std::size_t MATERIAL_MORPH_INDEX = 3;
		static constexpr std::size_t GROUP_MORPH_INDEX = 4;
	};

	// 剛体のバッファ
	template<typename CharType, std::size_t CharBufferSize>
	struct rigidbody_buffer
	{
		// 名前
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// 英語の名前
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// 関連ボーンのインデックス
		// 関連ナシの場合は-1
		std::size_t bone_index{};

		// グループ
		std::uint8_t group{};
		// 非衝突グループフラグ
		std::uint16_t non_collision_group{};

		// 形状
		// 0:球 1:箱 2:カプセル
		std::uint8_t shape{};
		// サイズ
		// (x,y,z)？？？？
		std::array<float, 3> size{};

		// 位置
		std::array<float, 3> position{};
		// 回転（ラジアン角）
		std::array<float, 3> rotation{};

		// 質量
		float mass{};
		// 移動減衰
		float liner_damping{};
		// 回転減衰
		float angular_damping{};
		// 反発力
		float restitution{};
		// 摩擦力
		float friction{};

		// 剛体の物理演算
		// 0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
		std::uint8_t rigidbody_type{};
	};

	// ジョイントのバッファ
	template<typename CharType, std::size_t CharBufferSize>
	struct joint_buffer
	{
		// 名前
		std::int32_t name_size{};
		std::array<CharType, CharBufferSize> name{};

		// 英語の名前
		std::int32_t english_name_size{};
		std::array<CharType, CharBufferSize> english_name{};

		// ジョイントの種類
		// 0:スプリング6DOF | PMX2.0では 0 のみ(拡張用)
		std::uint8_t type{};

		// 以下、type==0 の場合に使用
		// ただし、PMX2.0ではtype==0であるので使うことになる

		// 関連剛体Aのインデックス（感染なしの場合は-1）
		std::int32_t rigidbody_a{};

		// 関連剛体Bのインデックス（感染なしの場合は-1）
		std::int32_t rigidbody_b{};

		// 位置
		std::array<float, 3> position{};
		// 回転（ラジアン角）
		std::array<float, 3> rotation{};

		// 移動制限：下限
		std::array<float, 3> move_lower_limit{};
		// 移動制限：上限
		std::array<float, 3> move_upper_limit{};
		// 回転制限（ラジアン角）：下限
		std::array<float, 3> rotation_lower_limit{};
		// 回転制限（ラジアン角）：上限
		std::array<float, 3> rotation_upper_limit{};

		// ばね定数：移動
		std::array<float, 3> move_spring_constant{};
		// ばね定数：回転
		std::array<float, 3> rotation_spring_constant{};

	};

}

