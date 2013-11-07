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

#ifndef _BLENDINT_FORM_HPP_
#define _BLENDINT_FORM_HPP_

#include <BlendInt/AbstractForm.hpp>
#include <BlendInt/GLBuffer.hpp>

#define WIDGET_AA_JITTER 8

/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	/**
	 * @brief A Normal form
	 */
	class Form: public AbstractForm
	{
		DISALLOW_COPY_AND_ASSIGN(Form);

	public:

		Form ();

		Form (AbstractForm* parent);

		virtual ~Form();

		void set_round_type (int type);

		int round_type () const {return m_round_type;}

		/**
		 * @brief set the round radius for the corner
		 * @param radius
		 */
		void set_radius (float radius);

		float radius () const {return m_radius;}

		void set_emboss (bool emboss);

		bool emboss () const {return m_emboss;}

		void set_border_width (float width = 1.0);

		float border_width () const {return m_border_width;}

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

		virtual void update (int type, const void* data);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		/**
		 * @brief draw the GL Buffer in render()
		 * @param index the index of the gl buffer to draw
		 * @param mode the primitive or primitives mode defined in gl.h, 
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_gl_buffer (size_t index, int mode = GL_POLYGON);

		/**
		 * @brief draw shaded GL buffer in render()
		 * @param index the index of the gl buffer to draw
		 * @param mode the primitive or primitives mode defined in gl.h,
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_shaded_gl_buffer (size_t index, int mode = GL_POLYGON);

		/**
		 * @brief draw the GL Buffer in render() with anti-alias
		 * @param index the index of the gl buffer to draw
		 * @param mode the primitive or primitives mode defined in gl.h,
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_gl_buffer_anti_alias (size_t index, int mode = GL_QUAD_STRIP);

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

		/**
		 * @brief generate shadow vertices
		 * @param[in] size
		 * @param[in] rad
		 * @param[in] step
		 * @param[out] vert
		 * @return
		 */
		int generate_shadow_vertices (const Size* size,
				float rad,
				float step,
				float vert[WIDGET_SIZE_MAX][2]);

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

		GLBuffer& glbuffer() {return m_glbuffer;}

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

		static const float jit[WIDGET_AA_JITTER][2];

	private:

		void update_shape (const Size* size);

		/**
		 * @brief the round type defined in enumeration RoundCornerType
		 */
		DRAWABLE_PROPERTY int m_round_type;

		/**
		 * @brief the radius of the round corner
		 *
		 * should be 0.0, 1.0, 2.0 etc
		 */
		DRAWABLE_PROPERTY float m_radius;

		/**
		 * @brief border width
		 */
		DRAWABLE_PROPERTY float m_border_width;

		/**
		 * @brief If draw emboss
		 */
		DRAWABLE_PROPERTY bool m_emboss;

		/**
		 * @brief the buffer for vertices
		 */
		GLBuffer m_glbuffer;

	};
}

#endif /* _BLENDINT_FORM_HPP_ */
