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
		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f�𖖔��ɒǉ�
		static void emplace_back(T& vertex, pmx_vertex_buffer const&);
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
		template<std::size_t CharBufferSize>
		static void emplace_back(T& texture_path, std::array<char_type, CharBufferSize> const& str, std::size_t size);
	};

	template<typename T>
	struct pmx_material_traits
	{
		using char_type = void;

		// �T�C�Y���w�肵�č\�z
		static T construct(std::size_t size);

		// �v�f��ǉ�
		template<typename CharType, std::size_t CharBufferSize>
		static void emplace_back(T& material, pmx_material_buffer<CharType, CharBufferSize> const&);
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
			char_type* english_name, std::size_t english_name_size,
			std::array<float, 3> const& position,
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
			std::array<float, 3> const& access_point_offset,
			// �ڑ���̃r�b�g��1�̏ꍇ�Ɏg�p����ڑ���̃{�[���̃C���f�b�N�X
			std::size_t access_bone_index,
			// ��]�t�^�܂��͈ړ��t�^�̃r�b�g��1�̏ꍇ�Ɏg�p����t�^�e�̃{�[���̃C���f�b�N�X
			std::size_t assign_bone_index,
			// ��]�t�^�܂��͈ړ��t�^�̃r�b�g��1�̏ꍇ�Ɏg�p����t�^��
			std::size_t assign_param,
			// ���Œ肪1�̃r�b�g��1�̏ꍇ�Ɏg�p���鎲�̕����x�N�g��
			std::array<float, 3> const& fixed_direction,
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
			std::tuple<std::size_t, std::uint8_t, std::array<float, 3>, std::array<float, 3>>* ik_link
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