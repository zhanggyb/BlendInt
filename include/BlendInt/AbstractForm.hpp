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
	 * An AbstractForm is a 2 dimension object with the position/size.
	 *
	 * The default size is 0 at (0, 0), so it draws nothing
	 */
	class AbstractForm
	{
	public:

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

		/**
		 * @brief reset the size of the form
		 * @param width
		 * @param height
		 * @return true if the new size is set, false if it's the same as the current size
		 *
		 * Reset the form's size, return true if the new size is set, and the update() was
		 * called once (this usually some operations of OpenGL API, e.g. update the GL Buffer
		 */
		bool resize (unsigned int width, unsigned int height);

		/**
		 * @brief reset the size of the form
		 * @param size
		 * @return true if the new size is set, false if it's the same as the current size
		 *
		 * Reset the form's size, return true if the new size is set, and the update() was
		 * called once (this usually some operations of OpenGL API, e.g. update the GL Buffer
		 */
		bool resize (const Size& size);

	protected:

		/**
		 * @brief preset the size of the form
		 * @param width
		 * @param height
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through update() for performance.
		 */
		inline void preset_size (unsigned int width, unsigned int height)
		{
			m_size.set_width(width);
			m_size.set_height(height);
		}

		/**
		 * @brief preset the size of the form
		 * @param size
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through update() for performance.
		 */
		inline void preset_size (const Size& size)
		{
			m_size = size;
		}

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

		static const float jit[WIDGET_AA_JITTER][2];

		virtual void update (int type, const void* data) = 0;

		virtual void render () = 0;

		void generate_rect_vertices (const Size* size,
				float border,
				float inner_v[4][2],
				float outer_v[4][2]);

		VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				float inner_v[WIDGET_SIZE_MAX][2],
				float outer_v[WIDGET_SIZE_MAX][2]);


		/**
		 * @brief calculate vertices for round box edges
		 * @param size
		 * @param border
		 * @param theme
		 * @param shadedir shade direction
		 * @param inner
		 * @param outer
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

	private:

		Size m_size;

	};

}

#endif /* _BLENDINT_ABSTRACTFORM_HPP_ */
