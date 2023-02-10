#pragma once
#include<utility>
#include<array>
#include<optional>
#include<bitset>
#include"generics_type.hpp"
#include<vector>

namespace mmdl
{
	// �G���R�[�h�̃^�C�v
	// �Ή����鐮����1�����̑傫��
	enum class encode_type
	{
		utf16 = 2,
		utf8 = 1,
	};

	// pmx�w�b�_
	// Data��pmx2.0�ł̓T�C�Y��8�Œ�
	template<typename HeaderDataType = std::int64_t>
	struct pmx_header
	{
		// �o�[�W����
		float version;

		// �G���R�[�h�̕���
		encode_type encode;

		// ���_�̒ǉ�UV�̐�
		// 1,2,4�̂����ꂩ
		HeaderDataType add_uv_number;

		// ���_�̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderDataType vertex_index_size;

		// �e�N�X�`���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderDataType texture_index_size;

		// �}�e���A���̃C���f�b�N�X�̑傫��
		// 1.2.4�̂����ꂩ
		HeaderDataType material_index_size;

		// �{�[���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderDataType bone_index_size;

		// ���[���̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderDataType morph_index_size;

		// ���W�b�g�{�f�B�̃C���f�b�N�X�̑傫��
		// 1,2,4�̂����ꂩ
		HeaderDataType rigid_body_index_size;

		// �u1,2,4�̂����ꂩ�v�ɂ���
		// 1: 1byte��unsigned
		// 2: 2byte��unsigned
		// 4: 4byte��signed
		// �ƂȂ�̂Œ���

	};

	template<typename HeaderDataType>
	struct pmx_header_traits<pmx_header<HeaderDataType>>
	{
		static pmx_header<HeaderDataType> construct(float version, std::uint8_t encode, std::size_t add_uv_num, std::size_t vertex_index_size, std::size_t texture_index_size,
			std::size_t material_index_size, std::size_t bone_index_size, std::size_t morph_index_size, std::size_t rigid_body_index_size)
		{
			auto result = pmx_header<HeaderDataType>{
				version,
				encode == 0 ? encode_type::utf16 : encode_type::utf8,
				static_cast<HeaderDataType>(add_uv_num),
				static_cast<HeaderDataType>(vertex_index_size),
				static_cast<HeaderDataType>(texture_index_size),
				static_cast<HeaderDataType>(material_index_size),
				static_cast<HeaderDataType>(bone_index_size),
				static_cast<HeaderDataType>(morph_index_size),
				static_cast<HeaderDataType>(rigid_body_index_size)
			};

			return result;
		}
	};


	// ���f���̖��O�Ƃ��̑��̏��
	template<typename Str>
	struct pmx_info
	{
		// ���f���̖��O
		Str model_name;

		// ���f���̉p�ꖼ
		Str english_mode_name;

		// �R�����g
		Str comment;

		// �p��̃R�����g
		Str english_comment;

	};

	template<typename Str>
	struct pmx_info_traits<pmx_info<Str>>
	{
		using char_type = Str::value_type;

		static pmx_info<Str> construct(
			char_type* model_name_str, std::size_t mode_name_str_size,
			char_type* english_model_name_str, std::size_t english_mode_name_str_size,
			char_type* comment_str, std::size_t comment_str_size,
			char_type* english_comment_str, std::size_t english_comment_str_size
		)
		{
			return {
				Str(model_name_str,mode_name_str_size),
				Str(english_model_name_str,english_mode_name_str_size),
				Str(comment_str,comment_str_size),
				Str(english_comment_str,english_comment_str_size)
			};
		}
	};

	// 1���_���
	// �{�[���̃C���f�b�N�X�̑傫���ɂ��āApmx_header.bone_index_size��1,2,4�̒l���Ƃ�
	// ���̍ő�̒l�ł���4�̑傫���ɑΉ�����std::int32_t���f�t�H���g�Ƃ��Ďg�p����
	template<typename Vec2, typename Vec3, typename Vec4, typename BoneIndex = std::int32_t>
	struct pmx_vertex
	{
		// ���_���W
		Vec3 position;

		// �@��
		Vec3 normal;

		// uv
		Vec2 uv;

		// �ǉ���uv
		// pmx_header.add_uv_number�̐������g�p����
		std::array<Vec4, 4> additional_uv{};

		// �e�����󂯂�{�[���̐���weight���Ή����Ă���
		// BDEF1�Ȃ�weight[0]=1�A���̂ق���0
		// BDEF2�Ȃ�weight[0]!=0,weight[1]!=0�A���̂ق���0�i���Ȃ݂�weight[0]+weight[1]=1�j
		// BDEF4�Ȃ�weight�͑S��0�łȂ�
		std::array<BoneIndex, 4> bone{ 0,0,0,0 };
		std::array<float, 4> weight{ 0.f,0.f,0.f,0.f };

		// SDEF�p�̕␳�s��
		// SDEF��BDEF2�ƕ␳�s��ɂ���Čv�Z�����
		// sdef[0]�FSDEF-C�l
		// sdef[1]�FSDEF-R0�l
		// sdef[2]�FSDEF-R1�l
		std::optional<std::array<Vec3, 3>> sdef = std::nullopt;

		// �G�b�W�{��
		// �ގ��̃G�b�W�T�C�Y�ɑ΂��Ă̔{���l
		float edge_magnification;

	};

	template<typename Vec2, typename Vec3, typename Vec4, typename BoneIndex>
	struct pmx_vertex_traits<std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>>
	{
		// �T�C�Y���w�肵�č\�z
		static std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> construct(std::size_t size) {

			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>> result{};
			result.reserve(size);
			return result;
		}

		// BDEF1�`���̃{�[���̒ǉ�
		static void emplace_back_BDEF1(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// �E�F�C�g1.0�̒P��{�[��
			std::size_t bone_index_1,
			// �G�b�W�{��
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.weight[0] = 1.f;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

		// BDEF2�`���̃{�[���̒ǉ�
		static void emplace_back_BDEF2(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// �{�[��1�̃C���f�b�N�X
			std::size_t bone_index_1,
			// �{�[��2�̃C���f�b�N�X
			std::size_t bone_index_2,
			// �{�[��1�̃E�F�C�g(�{�[��2�̃E�F�C�g�� 1.0 - �{�[��1�̃E�F�C�g )
			float bone_weight_1,
			// �G�b�W�{��
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.bone[1] = bone_index_2;
			v.weight[0] = bone_weight_1;
			v.weight[1] = 1.f - bone_weight_1;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

		// BDEF4�`���̃{�[���̒ǉ�
		static void emplace_back_BDEF4(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// �{�[��1�̃C���f�b�N�X
			std::size_t bone_index_1,
			// �{�[��2�̃C���f�b�N�X
			std::size_t bone_index_2,
			// �{�[��3�̃C���f�b�N�X
			std::size_t bone_index_3,
			// �{�[��4�̃C���f�b�N�X
			std::size_t bone_index_4,
			// �{�[��1�̃E�F�C�g
			float bone_weight_1,
			// �{�[��2�̃E�F�C�g
			float bone_weight_2,
			// �{�[��3�̃E�F�C�g
			float bone_weight_3,
			// �{�[��4�̃E�F�C�g�i�E�F�C�g�̍��v�ɂ���1�̕ۏ؂͂Ȃ��j
			float bone_weight_4,
			// �G�b�W�{��
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.bone[1] = bone_index_2;
			v.bone[2] = bone_index_3;
			v.bone[3] = bone_index_4;
			auto weight_sum = bone_weight_1 + bone_index_2 + bone_index_3 + bone_index_4;
			v.weight[0] = bone_weight_1 / weight_sum;
			v.weight[1] = bone_weight_2 / weight_sum;
			v.weight[2] = bone_weight_3 / weight_sum;
			v.weight[3] = bone_weight_4 / weight_sum;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

		// SDEF�`���̃{�[���̒ǉ�
		static void emplace_back_SDEF(
			std::vector<pmx_vertex<Vec2, Vec3, Vec4, BoneIndex>>& vertex,
			std::array<float, 3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// �{�[��1�̃C���f�b�N�X
			std::size_t bone_index_1,
			// �{�[��2�̃C���f�b�N�X
			std::size_t bone_index_2,
			// �{�[��1�̃E�F�C�g(�{�[��2�̃E�F�C�g�� 1.0 - �{�[��1�̃E�F�C�g )
			float bone_weight_1,
			// SDEF-C�l(x,y,z)
			std::array<float, 3> const& sdef_c,
			// SDEF-R0�l(x,y,z)
			std::array<float, 3> const& sdef_r0,
			// SDEF-R1�l(x,y,z) ���C���l��v�v�Z
			std::array<float, 3> const& sdef_r1,
			// �G�b�W�{��
			float edge_factor
		)
		{
			pmx_vertex<Vec2, Vec3, Vec4, BoneIndex> v{};

			v.position = { position[0],position[1] ,position[2] };
			v.normal = { normal[0],normal[1] ,normal[2] };
			v.uv = { uv[0],uv[1] };

			for (std::size_t i = 0; i < add_uv_size; i++)
			{
				v.additional_uv[i] = { add_uv[i][0],add_uv[i][1] ,add_uv[i][2] ,add_uv[i][3] };
			}

			v.bone[0] = bone_index_1;
			v.bone[1] = bone_index_2;
			v.weight[0] = bone_weight_1;
			v.weight[1] = 1.f - bone_weight_1;

			std::array<Vec3, 3> sdef{ {
				{sdef_c[0],sdef_c[1],sdef_c[2]},
				{sdef_r0[0],sdef_r0[1],sdef_r0[2]},
				{sdef_r1[0],sdef_r1[1],sdef_r1[2]}
			} };
			v.sdef = sdef;

			v.edge_magnification = edge_factor;

			vertex.emplace_back(std::move(v));
		}

	};



	// �ʂ��\�����钸�_�̏��
	// 3�ň�̎O�p�`�̖ʂ�\��
	// pmx_header.vertex_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����int32_t���f�t�H���g�Ɏw��
	// WARNING: ����Ȃ�����
	template<typename VertexIndex = std::int32_t>
	using pmx_surface = VertexIndex;

	template<typename VertexIndex>
	struct pmx_surface_traits<std::vector<pmx_surface<VertexIndex>>>
	{
		// �T�C�Y���w�肵�č\�z
		static std::vector<pmx_surface<VertexIndex>> construct(std::size_t size)
		{
			std::vector<pmx_surface<VertexIndex>> result{};
			result.reserve(size);

			return result;
		}

		// �v�f��ǉ�
		static void emplace_back(std::vector<pmx_surface<VertexIndex>>& surface, std::size_t index)
		{
			surface.emplace_back(index);
		}
	};

	// pmx_material�Ŏg�p
	enum class sphere_mode
	{
		// �����Ȃ�
		none,

		// ��Z
		sph,

		// ���Z
		spa,

		// �T�u�e�N�X�`��
		// �ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��
		subtexture,
	};

	// pmx_material�Ŏg�p
	enum class toon_type
	{
		// ���L
		shared,

		// ��
		unshared,
	};

	// �}�e���A��
	// pmx_header.texture_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����int32_t���f�t�H���g�Ɏw��
	template<typename Str, typename Vec3, typename Vec4, typename TextureIndex = std::int32_t>
	struct pmx_material
	{
		// �}�e���A���̖��O
		Str name;

		// �}�e���A���̉p��̖��O
		Str english_name;

		// �f�B�t���[�Y
		Vec4 diffuse;
		// �X�؃L����
		Vec3 specular;
		// �X�؃L�����W��
		float specularity;
		// �A���r�G���g
		Vec3 ambient;

		enum class draw_flag
		{
			// �Жʂ̕`����s�����ǂ���
			single_sided,

			// �n�ʂɉe�𗎂Ƃ����ǂ���
			grond_shadow,

			// �Z���t�V���h�E�}�b�v�֕`�悷�邩�ǂ���
			// WARNING: ���O������ɂ���
			draw_to_self_shadow_map,

			// �Z���t�V���h�E��`�悷�邩�ǂ���
			// WARNING: ���O������ɂ���
			draw_self_shadow,

			// �G�b�W��`�悷�邩�ǂ���
			draw_edge,
		};

		// �`��̃I�v�V����
		// WARNING: �}�W�b�N�i���o�[
		std::bitset<5> draw_flag_bits;

		// �G�b�W�̐F
		Vec4 edge_color;
		// �G�b�W�̑傫��
		float edge_size;

		// �ʏ�̃e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		TextureIndex texture_index;
		// �X�t�B�A�e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		TextureIndex sphere_texture_index;

		sphere_mode sphere_mode_value;


		// toon_type::unshared�̎��Atoon_texture��Toon�e�N�X�`���̃e�N�X�`���e�[�u���̎Q�ƃC���f�b�N�X
		// toon_type::shared�̎��A���L�e�N�X�`���̃C���f�b�N�X�i0����9�܂ł̒l�j�i0�Ȃ�toon01.bmp,1�Ȃ�toon02.bmp�̂悤�ɑΉ��j
		toon_type toon_type_value;
		TextureIndex toon_texture;

		// �����A���R��
		Str memo;

		// �ގ��ɑΉ����钸�_�̐�
		// �K��3�̔{���ɂȂ�
		std::int32_t vertex_number;

	};

	enum class bone_flag
	{
		// �ڑ���@�����@
		// false: ���W�I�t�Z�b�g�Ŏw��
		// true: �{�[���Ŏw��
		access_point = 0,

		// ��]�\���ǂ���
		rotatable = 1,

		// �ړ��\���ǂ���
		movable = 2,

		// �\��
		// WARNING; �\���\���ǂ����Ƃ����Ӗ���?
		display = 3,

		// ����\���ǂ���
		operable = 4,

		// IK�g�����ǂ���
		ik = 5,

		// ���[�J���t�^
		// false: ���[�U�[�ό`�l�^IK�����N�^���d�t�^
		// true: �e�̃��[�J���ό`��
		local_grant = 7,

		// ��]�t�^
		rotation_grant = 8,

		// �ړ��t�^
		move_grant = 9,

		// ���Œ肩�ǂ���
		fix_axis = 10,

		// ���[�J�������ǂ���
		local_axis = 11,

		// ������ό`���ǂ���
		post_physical_deformation = 12,

		// �O���e�ό`���ǂ���
		external_parent_deformation = 13,

	};

	template<typename Vec3, typename BoneIndex>
	struct ik_link {
		// �����N���Ă���{�[��
		BoneIndex bone;

		// �p�x�������s���ꍇ�̍ŏ��A�ő�̊p�x�̐���
		std::optional<std::pair<Vec3, Vec3>> min_max_angle_limit;
	};

	// �{�[��
	// pmx_header.bone_index_size��1,2,4�̒l���Ƃ�̂ŁA�ő�l�ɑΉ�����int32_t���f�t�H���g�Ɏw��
	template<typename Str, typename Vec3, template<typename> typename Container, typename BoneIndex = std::int32_t>
	struct pmx_bone
	{
		// �{�[���̖��O
		Str name;

		// �{�[���̉p��̖��O
		Str english_name;

		// �ʒu
		Vec3 position;
		// �e�{�[���̃C���f�b�N�X
		BoneIndex parent_index;
		// �ό`�K�w
		std::int32_t trannsformation_level;

		// WARNING: �}�W�b�N�i���o�[
		std::bitset<16> bone_flag_bits;

		// bone_flag_bits[access_point]=false�̂Ƃ��g�p
		// �{�[���̈ʒu����̑��ΓI�Ȉʒu��\��
		Vec3 access_point_offset;
		// bone_flag_bits[access_point]=ture�̂Ƃ��g�p
		// �ڑ���̃{�[���̃C���f�b�N�X��\��
		BoneIndex access_point_index;

		// bone_flag_bits[rotation_grant]=true�܂��� bone_flag_bits[move_grant]=true�̂Ƃ��g�p
		// �Ώۂ̃{�[����\��
		BoneIndex grant_index;
		// �t�^��
		float grant_rate;

		// bone_flag_bits[fix_axis]=true�̎��Ɏg�p
		// �Œ肳�ꂽ���̕����x�N�g��
		Vec3 fix_axis_direction;

		// bone_flag_bits[local_axis]=true�̎��Ɏg�p
		// x���̕����x�N�g��
		Vec3 local_axis_x;
		// z���̕����x�N�g��
		Vec3 local_axis_z;

		// bone_flag_bits[external_parent_deformation]=true�̎��Ɏg�p
		// Key�̒l
		std::int32_t external_parent_deformation_key;

		// bone_flag_bits[ik]=true�̎��Ɏg�p
		//IK�̃^�[�Q�b�g�̃{�[��
		BoneIndex ik_target_bone;
		// IK�̃��[�v��
		std::int32_t ik_roop_number;
		// IK�̃��[�v���s���ۂ�1�񓖂���̐����p�x�i���W�A���j
		float ik_rook_angle;

		Container<ik_link<Vec3, BoneIndex>> ik_link;


		// TODO: �u bone_flag_bits[-]=true�̎��Ɏg�p�v���ď����Ă���Ƃ���Optional�ŃC�C����
	};

}