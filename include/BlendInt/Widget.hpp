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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_FORM_HPP_
#define _BLENDINT_FORM_HPP_

#include <BlendInt/AbstractWidget.hpp>
#include <BlendInt/GLBuffer.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	/**
	 * @brief A Normal widget
	 *
	 * @ingroup widgets
	 */
	class Widget: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Widget);

	public:

		Widget ();

		Widget (AbstractWidget* parent);

		virtual ~Widget();

		static void SetDefaultBorderWidth (float border);

		static float DefaultBorderWidth ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void ContextMenuPressEvent (ContextMenuEvent* event);

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		/**
		 * @brief draw vertices without buffer
		 */
		void draw_outline (const float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2], int num);

		/**
		 * @brief draw vertices without buffer
		 */
		void draw_inner (const float inner_v[WIDGET_SIZE_MAX][2], int num);

		/**
		 * @brief draw the GL Buffer in Render()
		 * @param key the key to identify gl buffer to draw, @sa GLBuffer
		 * @param mode the primitive or primitives mode defined in gl.h, 
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in Render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_inner_buffer (GLBuffer* buffer, size_t index = 0, int mode = GL_POLYGON);

		/**
		 * @brief draw shaded GL buffer in Render()
		 * @param key the key to identify gl buffer to draw, @sa GLBuffer
		 * @param mode the primitive or primitives mode defined in gl.h,
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in Render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_shaded_inner_buffer (GLBuffer* buffer, size_t index = 0, int mode = GL_POLYGON);

		/**
		 * @brief draw the GL Buffer in Render() with anti-alias
		 * @param key the key to identify gl buffer to draw, @sa GLBuffer
		 * @param mode the primitive or primitives mode defined in gl.h,
		 * e.g. GL_QUAD_STRIP, or GL_POLYGON.
		 *
		 * draw the buffer of index in Render(), the parameter mode is
		 * the enumeration type of primitive or primitives defined in
		 * gl.h: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS,
		 * GL_QUAD_STRIP, and GL_POLYGON.
		 */
		void draw_outline_buffer (GLBuffer* buffer, size_t index = 0, int mode = GL_QUAD_STRIP);

		/**
		 * @brief generate GL buffer for form drawing
		 * @param[in] size the size to enclose vertices
		 * @param[in] emboss if draw emboss
		 * @param[in] round_type round type
		 * @param[in] radius round radius
		 * @param[out] buffer the buffer generated
		 *
		 * If emboss is true, 3 buffers are generated, if emboss is false, 2 buffers are generated:
		 *	- index 0: for inner drawing
		 *	- index 1: for outline drawing
		 *	- index 2: for emboss drawing
		 */
		void GenerateFormBuffer (const Size* size, bool emboss, int round_type,
								 float radius, GLBuffer* buffer);

		/**
		 * @brief generate GL buffer for rect form drawing
		 * @param size
		 * @param emboss
		 * @param buffer
		 */
		void GenerateRectFormBuffer (const Size* size, bool emboss, GLBuffer* buffer);

		/**
		 * @brief generate buffer with shaded color
		 * @param[in] size
		 * @param[in] border
		 * @param[in] round_type
		 * @param[in] radius
		 * @param[in] theme
		 * @param[in] shadedir
		 * @param[in] highlight
		 * @param[out] buffer
		 *
		 * This function calculate the output GLBuffers with shaded color
		 *
		 * If highlight > 0, 3 buffers will be generated, if not, 2 buffers generated:
		 *	- buffer index 0: used for inner
		 *	- buffer index 1: used for outline
		 *	- buffer index 2: used for inner highlight
		 */
		void GenerateShadedFormBuffer (const Size* size,
									   float border,
									   int round_type,
									   float radius,
									   const WidgetTheme* theme,
									   Orientation shadedir,
									   short highlight,
									   GLBuffer* buffer);

		/**
		 * @brief generate buffer with shaded color
		 * @param[in] size
		 * @param[in] border
		 * @param[in] round_type
		 * @param[in] radius
		 * @param[in] color
		 * @param[in] shadetop
		 * @param[in] shadedown
		 * @param[in] shadedir
		 * @param[in] highlight
		 * @param[out] buffer
		 *
		 * This function calculate the output GLBuffers with shaded color
		 *
		 * If highlight > 0, 3 buffers will be generated, if not, 2 buffers generated:
		 *	- buffer index 0: used for inner
		 *	- buffer index 1: used for outline
		 *	- buffer index 2: used for inner highlight
		 */
		void GenerateShadedFormBuffer (const Size* size,
									   float border,
									   int round_type,
									   float radius,
									   const Color& color,
									   short shadetop,
									   short shadedown,
									   Orientation shadedir,
									   short highlight,
									   GLBuffer* buffer);

	private:

		static float default_border_width;

	};
}

#endif /* _BLENDINT_FORM_HPP_ */
