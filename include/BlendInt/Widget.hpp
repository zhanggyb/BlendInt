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

#ifndef _BLENDINT_WIDGET_HPP_
#define _BLENDINT_WIDGET_HPP_

#include <BlendInt/Drawable.hpp>
#include <BlendInt/GLBuffer.hpp>

#define WIDGET_AA_JITTER 8

/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	enum WidgetPropertyType {
		WidgetPropertyPadding = BasicPropertyLast + 1,
		WidgetPropertyBorderWidth,
		WidgetPropertyEmboss,
		WidgetPropertyLast = WidgetPropertyEmboss
	};

	class Widget: public Drawable
	{
		DISALLOW_COPY_AND_ASSIGN(Widget);

	public:

		Widget ();

		Widget (Drawable* parent);

		virtual ~Widget ();

		const Padding& padding () const;

		void set_padding (const Padding& padding);

		void set_padding (int left, int right, int top, int bottom);

		void set_border_width (float width = 1.0)
		{
			if(width > 0.0)
				m_border_width = width;
		}

		float border_width () const {return m_border_width;}

		void set_emboss (bool emboss)
		{
			m_emboss = emboss;
		}

		bool emboss () const {return m_emboss;}

	protected:

		/**
		 * Structure used in calulating vertex buffer for inner and outline
		 */
		struct VerticesSum {
			VerticesSum ()
			: total(0), half(0)
			{ }

			int total;	/**< total number of vertices for widget */
			int half;	/**< halfway vertices number */
		};

		virtual bool update (int type, const void* property);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		/**
		 * @check if geometry contains the cursor, exclude padding
		 * @param cursor
		 * @return
		 */
		bool contain_no_padding (const Coord2d& cursor);

		/**
		 * @brief calculate vertices for round box edge with no shaded color
		 * @param[in] size the size to calculate edges
		 * @param[out] inner_v
		 * @param[out] outer_v
		 * @return how many vertices are used in the output array
		 */
		VerticesSum generate_vertices (const Size* size, float inner_v[WIDGET_SIZE_MAX][2], float outer_v[WIDGET_SIZE_MAX][2]);

		/**
		 * @brief calculate vertices for round box edges
		 * @param size
		 * @param theme
		 * @param shadedir shade direction
		 * @param inner
		 * @param outer
		 * @return
		 */
		VerticesSum generate_vertices (const Size* size,
				const WidgetTheme* theme,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2]);

		/**
		 * @brief generate vertices array for round box inner and edges
		 * @param[in] size the size to calculate position and shade uv
		 * @param[in] shadetop the top shade, defined in theme
		 * @param[in] shadedown the bottom shade, defined in theme
		 * @param[in] shadedir true if shade with horizontal direction
		 * @param[out] inner inner vertices with position and color information
		 * @param[out] outer vertices for outline
		 * @return
		 */
		VerticesSum generate_vertices (const Size* size,
				const Color& color,
				short shadetop,
				short shadedown,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2]);

		void verts_to_quad_strip (
				const float inner_v[WIDGET_SIZE_MAX][2],
				const float outer_v[WIDGET_SIZE_MAX][2],
				const int totvert,
				float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]);

		void verts_to_quad_strip (
				const float inner_v[WIDGET_SIZE_MAX][6],
				const float outer_v[WIDGET_SIZE_MAX][2],
				const int totvert,
				float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]);

		void verts_to_quad_strip_open (
				const float outer_v[WIDGET_SIZE_MAX][2],
				const int totvert,
				float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]);

		Padding m_padding;

		float m_border_width;

		bool m_emboss;

		/**
		 * @brief get the GLBuffer
		 * @return
		 */
		GLBuffer& buffer () {return m_buffer;}

		GLBuffer m_buffer;

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

		static const float jit[WIDGET_AA_JITTER][2];

	private:

		void update_shape (const Size* size);
	};

} /* namespace BlendInt */
#endif /* _BIL_WIDGET_H_ */
