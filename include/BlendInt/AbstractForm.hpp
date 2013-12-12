/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_ABSTRACTFORM_HPP_
#define _BLENDINT_ABSTRACTFORM_HPP_

#include <BlendInt/Point.hpp>
#include <BlendInt/Size.hpp>
#include <BlendInt/Types.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	/**
	 * Structure used in calulating vertex buffer for inner and outline
	 *
	 * @note don't use nested class for SWIG later
	 */
	struct VerticesSum {
		VerticesSum ()
		: total(0), half(0)
		{ }

		int total;	/**< total number of vertices for widget */
		int half;	/**< halfway vertices number */
	};

	/**
	 * @brief Abstract form class
	 *
	 * An AbstractForm is the base class for all GUI elements in 2 dimension.
	 *
	 * The default size is 0 at (0, 0) and the subclass must provide a public
	 * method to resize the form, this method usually need to call Update for redraw
	 * the form.
	 *
	 * @sa Update()
	 */
	class AbstractForm
	{
	public:

#ifdef DEBUG
		friend class Interface;
#endif

		AbstractForm();

		AbstractForm(const AbstractForm& orig);

		virtual ~AbstractForm();

		AbstractForm& operator = (const AbstractForm& orig)
		{
			m_size = orig.size();
			return *this;
		}

		const Size& size () const
		{
			return m_size;
		}

	protected:

		/**
		 * @brief preset the size of the form
		 * @param width
		 * @param height
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (unsigned int width, unsigned int height)
		{
			m_size.set_width(width);
			m_size.set_height(height);
		}

		/**
		 * @brief preset the size of the form
		 * @param size
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (const Size& size)
		{
			m_size = size;
		}

		virtual void Update (int type, const void* data) = 0;

		virtual void Render () = 0;

		/**
		 * @brief generate vertices for drawing rectangle based on the given size and border width
		 * @param[in] size
		 * @param[in] border
		 * @param[out] inner_v
		 * @param[out] outer_v
		 */
		void generate_rect_vertices (const Size* size,
				float border,
				float inner_v[4][2],
				float outer_v[4][2]);

		/**
		 * @brief generate vertices for drawing round box based on the given size, border width, round type, radius
		 * @param[in] size
		 * @param[in] border
		 * @param[in] round_type
		 * @param[in] radius
		 * @param[out] inner_v
		 * @param[out] outer_v
		 * @return
		 */
		VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				float inner_v[WIDGET_SIZE_MAX][2],
				float outer_v[WIDGET_SIZE_MAX][2]);


		/**
		 * @brief calculate vertices for round box edges
		 * @param[in] size
		 * @param[in] border
		 * @param[in] round_type
		 * @param[in] radius
		 * @param[in] theme
		 * @param[in] shadedir shade direction
		 * @param[out] inner
		 * @param[out] outer
		 * @return
		 */
		VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				const WidgetTheme* theme,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2]);

		/**
		 * @brief generate vertices array for round box inner and edges
		 * @param[in] size the size to calculate position and shade uv
		 * @param[in] border
		 * @param[in] round_type
		 * @param[in] radius
		 * @param[in] color
		 * @param[in] shadetop the top shade, defined in theme
		 * @param[in] shadedown the bottom shade, defined in theme
		 * @param[in] shadedir true if shade with horizontal direction
		 * @param[out] inner inner vertices with position and color information
		 * @param[out] outer vertices for outline
		 * @return
		 */
		VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				const Color& color,
				short shadetop,
				short shadedown,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2]);


		static void verts_to_quad_strip (
				const float inner_v[WIDGET_SIZE_MAX][2],
				const float outer_v[WIDGET_SIZE_MAX][2],
				const int totvert,
				float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]);

		static void verts_to_quad_strip (
				const float inner_v[WIDGET_SIZE_MAX][6],
				const float outer_v[WIDGET_SIZE_MAX][2],
				const int totvert,
				float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]);

		static void verts_to_quad_strip_open (
				const float outer_v[WIDGET_SIZE_MAX][2],
				const int totvert,
				float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]);

		static void dispatch_render (AbstractForm* obj);

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

		static const float jit[WIDGET_AA_JITTER][2];

	private:

		Size m_size;

	};

}

#endif /* _BLENDINT_ABSTRACTFORM_HPP_ */
