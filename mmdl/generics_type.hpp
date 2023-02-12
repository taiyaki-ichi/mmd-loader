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

		static T construct(
			// ���f����
			char_type* model_name_str, std::size_t mode_name_str_size,
			// ���f���p��
			char_type* english_model_name_str, std::size_t english_mode_name_str_size,
			// �R�����g
			char_type* comment_str, std::size_t comment_str_size,
			// �R�����g�p
			char_type* english_comment_str, std::size_t english_comment_str_size
			);
	};

	template<typename T>
	struct pmx_vertex_traits
	{
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// BDEF1�`���̃{�[���̒ǉ�
		static void emplace_back_BDEF1(
			T& vertex,
			std::array<float,3> const& position,
			std::array<float, 3> const& normal,
			std::array<float, 2> const& uv,
			std::array<float, 4>* add_uv, std::size_t add_uv_size,
			// �E�F�C�g1.0�̒P��{�[��
			std::size_t bone_index_1,
			// �G�b�W�{��
			float edge_factor
		);

		// BDEF2�`���̃{�[���̒ǉ�
		static void emplace_back_BDEF2(
			T& vertex,
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
		);

		// BDEF4�`���̃{�[���̒ǉ�
		static void emplace_back_BDEF4(
			T& vertex,
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
		);

		// SDEF�`���̃{�[���̒ǉ�
		static void emplace_back_SDEF(
			T& vertex,
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
		);

	};


	template<typename T>
	struct pmx_surface_traits
	{
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		static void emplace_back(T& surface, std::size_t index);
	};


	template<typename T>
	struct pmx_texture_path_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		static void emplace_back(T& texture_path, char_type const* str, std::size_t size);
	};

	template<typename T>
	struct pmx_material_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		static void emplace_back(T& material,
			char_type const* name, std::size_t name_size,
			char_type const* english_name, std::size_t english_name_size,
			std::array<float, 4> const& diffuse,
			std::array<float, 3> const& specular,
			float specularity,
			std::array<float, 3> const& ambient,
			// �`��t���O 0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��, 0x10:�G�b�W�`��
			std::uint8_t bit_flag,
			std::array<float, 4> const& edge_color,
			float edge_size,
			std::size_t texture_index,
			std::size_t sphere_texture_index,
			// �X�t�B�A���[�h 0:���� 1:��Z(sph) 2:���Z(spa) 3:�T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
			std::uint8_t sphere_mode,
			// ���L�g�D�[���t���O 
			std::uint8_t toon_flag,
			// ���L�g�D�[���t���O��0: �e�N�X�`���̃C���f�b�N�X
			// ���L�g�D�[���t���O��1: 0..9 -> toon01.bmp...toon10.bmp �ɑΉ�
			std::size_t toon_index,
			char_type const* memo, std::size_t memo_size,
			// �}�e���A���ɑΉ����钸�_�̐�
			std::size_t vertex_num
		);
	};

	template<typename T>
	struct pmx_bone_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		static void emplace_back(T& bone,
			char_type* name, std::size_t name_size,
			char_type* english_name,std::size_t english_name_size,
			std::array<float,3> const& position,
			std::size_t parent_bone_index,
			std::size_t transformation_level,
			/*
			���{�[���t���O
				0x0001  : �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0:���W�I�t�Z�b�g�Ŏw�� 1:�{�[���Ŏw��

				0x0002  : ��]�\
				0x0004  : �ړ��\
				0x0008  : �\��
				0x0010  : �����

				0x0020  : IK

				0x0080  : ���[�J���t�^ | �t�^�Ώ� 0:���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1:�e�̃��[�J���ό`��
				0x0100  : ��]�t�^
				0x0200  : �ړ��t�^

				0x0400  : ���Œ�
				0x0800  : ���[�J����

				0x1000  : ������ό`
				0x2000  : �O���e�ό`
			*/
			std::uint16_t bone_flag,
			// �ڑ���̃r�b�g��0�̏ꍇ�Ɏg�p������W�I�t�Z�b�g
			std::array<float,3> const& access_point_offset,
			// �ڑ���̃r�b�g��1�̏ꍇ�Ɏg�p����ڑ���̃{�[���̃C���f�b�N�X
			std::size_t access_bone_index,
			// ��]�t�^�܂��͈ړ��t�^�̃r�b�g��1�̏ꍇ�Ɏg�p����t�^�e�̃{�[���̃C���f�b�N�X
			std::size_t assign_bone_index,
			// ��]�t�^�܂��͈ړ��t�^�̃r�b�g��1�̏ꍇ�Ɏg�p����t�^��
			std::size_t assign_param,
			// ���Œ肪1�̃r�b�g��1�̏ꍇ�Ɏg�p���鎲�̕����x�N�g��
			std::array<float,3> const& fixed_direction,
			// �O���e�ό`�̃r�b�g��1�̏ꍇ�Ɏg�p����Key�l
			std::size_t key_value,
			// �ȉ��AIK�̃r�b�g��1�̏ꍇ�Ɏg�p����
			// IK�^�[�Q�b�g�̃{�[��
			std::size_t ik_target_bone,
			// IK�̃��[�v�̉�
			std::size_t ik_loop_num,
			// IK��1�񂠂���̊p�x����
			float ik_angle_limit_par_process,
			// IK�����N�̐�
			std::size_t ik_link_size,
			// IK�����N
			// �{�[���̃C���f�b�N�X�A�p�x�����̗L���A�p�x�����̉����A�p�x�����̏���A�A�A�̏�
			std::tuple<std::size_t,std::uint8_t,std::array<float,3>,std::array<float,3>>* ik_link
			);
	};

	// �v���C�}���e���v���[�g
	template<typename, typename>
	struct count_construct_container_traits {};

	// �T�C�Y�w�肵�\�z�\�ŁA�Y�������Z�q�ŎQ�Ƃ�Ԃ��^�ɂ��Ă̓��ꉻ
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

		// �T�C�Y���w�肵�č\�z
		static T construct(SizeType n) {
			return T(n);
		}

		// �Y��������Q�Ƃ̎擾
		static value_type& get_reference(T& t, SizeType i) {
			return t[i];
		}
	};

	// �������\���^�ɂ��Ă̓��ꉻ
	template <typename CharT>
	struct count_construct_container_traits<std::basic_string<CharT>, std::size_t>
	{
		using value_type = std::basic_string<CharT>::value_type;
		using size_type = std::size_t;

		// �T�C�Y���w�肵�č\�z
		static std::basic_string<CharT> construct(std::size_t n) {
			return std::basic_string<CharT>(n, CharT{});
		}

		// �Y��������Q�Ƃ̎擾
		static value_type& get_reference(std::basic_string<CharT>& str, std::size_t i) {
			return str[i];
		}
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