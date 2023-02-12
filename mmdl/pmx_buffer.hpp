#pragma once
#include<array>

namespace mmdl
{
	struct pmx_header_buffer
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
	struct pmx_info_buffer
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


	struct pmx_vertex_buffer
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
	struct pmx_material_buffer
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
	struct pmx_bone_buffer
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
}
