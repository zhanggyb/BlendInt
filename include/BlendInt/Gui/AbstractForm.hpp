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

#ifndef _BLENDINT_GUI_ABSTRACTFORM_HPP_
#define _BLENDINT_GUI_ABSTRACTFORM_HPP_

#include <vector>

#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Types.hpp>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	enum UpdateID
	{
		Predefined,
		Custom
	};

	class UpdateRequest
	{
	public:

		UpdateRequest (UpdateID id, int type, const void* data) :
				m_id(id), m_type(type), m_data(data)
		{

		}

		~UpdateRequest ()
		{

		}

		UpdateID id () const
		{
			return m_id;
		}

		int type () const
		{
			return m_type;
		}

		const void* data () const
		{
			return m_data;
		}

	private:

		UpdateID m_id;
		int m_type;
		const void* m_data;
	};

	/**
	 * Structure used in calulating vertex buffer for inner and outline
	 *
	 * @note don't use nested class for SWIG later
	 */
	struct VerticesSum {
		VerticesSum ()
		: total(0), half(0)
		{ }

		size_t total;	/**< total number of vertices for widget */
		size_t half;	/**< halfway vertices number */
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
	class AbstractForm: public Object
	{
	public:

#ifdef DEBUG
		friend class Interface;
#endif

		AbstractForm();

		virtual ~AbstractForm();

		explicit AbstractForm(const AbstractForm& orig);

		AbstractForm& operator = (const AbstractForm& orig)
		{
			m_size = orig.size();
			return *this;
		}

		const Size& size () const
		{
			return m_size;
		}

		static void SetDefaultBorderWidth (float border);

		static float DefaultBorderWidth ();

		static void GenerateFlatRectVertices (
						const Size& size,
						float border,
						std::vector<GLfloat>* vertices);

		static void GenerateRectVertices (const Size& size,
						float border,
						std::vector<GLfloat>* inner,
						std::vector<GLfloat>* outer);

		/**
		 * @brief Generate round vertices for inner and outline of a form
		 * @param[in] size The size of the form
		 * @param[in] border The width of the outline
		 * @param[in] round_type Round type, MUST be one or bit operation of the values in RoundType
		 * @param[in] radius The round radius
		 * @param[out] inner The inner vertices in std::vector
		 * @param[out] outer The outline vertices in std::vector
		 */
		static VerticesSum GenerateRoundVertices (
						const Size& size,
						float border,
						int round_type,
						float radius,
						std::vector<GLfloat>* inner,
						std::vector<GLfloat>* outer);

		static VerticesSum GenerateRoundVertices (
						const Size& size,
						float border,
						int round_type,
						float radius,
						const Color& color,
						short shadetop,
						short shadedown,
						Orientation shadedir,
						std::vector<GLfloat>* inner,
						std::vector<GLfloat>* outer);

		static void GenerateTriangleStripVertices (
						const std::vector<GLfloat>& inner,
						const std::vector<GLfloat>& outer,
						const size_t totvert,
						std::vector<GLfloat>* strip);

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

		virtual bool Update (const UpdateRequest& request) = 0;

		/**
		 * @brief generate vertices for drawing rectangle based on the given size and border width
		 * @param[in] size
		 * @param[in] border
		 * @param[out] inner_v
		 * @param[out] outer_v
		 */
		static void generate_rect_vertices (const Size* size,
				float border,
				float inner_v[4][2],
				float outer_v[4][2]);

		static VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				float inner_v[WIDGET_SIZE_MAX][2]);

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
		static VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				float inner_v[WIDGET_SIZE_MAX][2],
				float outer_v[WIDGET_SIZE_MAX][2]);

		static VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				const WidgetTheme* theme,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6]);

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
		static VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				const WidgetTheme* theme,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2]);

		static VerticesSum generate_round_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				const Color& color,
				short shadetop,
				short shadedown,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6]);

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
		static VerticesSum generate_round_vertices (const Size* size,
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

		/**
		 * @brief draw vertices without buffer
		 */
		static void DrawOutlineArray (const float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2], int num);

		/**
		 * @brief draw vertices without buffer
		 */
		static void DrawInnerArray (const float inner_v[WIDGET_SIZE_MAX][2], int num);

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
		static void DrawInnerBuffer (GLArrayBuffer* buffer, int mode = GL_POLYGON);

		static void DrawInnerBuffer (const RefPtr<GLArrayBuffer>& buffer, int mode = GL_POLYGON);

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
		static void DrawShadedInnerBuffer (GLArrayBuffer* buffer, int mode = GL_POLYGON);

		static void DrawShadedInnerBuffer (const RefPtr<GLArrayBuffer>& buffer, int mode = GL_POLYGON);

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
		static void DrawOutlineBuffer (GLArrayBuffer* buffer, int mode = GL_QUAD_STRIP);

		static void DrawOutlineBuffer (const RefPtr<GLArrayBuffer>& buffer, int mode = GL_QUAD_STRIP);

		/**
		 * @brief Generate vertex buffer object for drawing a form
		 * @param[in] size The size of the form
		 * @param[in] round_type The round type
		 * @param[in] radius The radius of the round corner
		 * @param[out] inner_buffer Buffer object for drawing the inner
		 * @param[out] outer_buffer Buffer object for drawing the outer
		 * @param[out] emboss_buffer Buffer object for drawing the emboss
		 */
		static void GenerateFormBuffer (const Size* size,
				int round_type,
				float radius,
				GLArrayBuffer* inner_buffer,
				GLArrayBuffer* outer_buffer,
				GLArrayBuffer* emboss_buffer);

		static void GenerateShadedFormBuffers (const Size* size,
									   int round_type,
									   float radius,
									   const WidgetTheme* theme,
									   Orientation shadedir,
									   short highlight,
									   GLArrayBuffer* inner_buffer_p,
									   GLArrayBuffer* outer_buffer_p,
									   GLArrayBuffer* highlight_buffer_p);

		static void GenerateShadedFormBuffer (const Size* size,
									   float border,
									   int round_type,
									   float radius,
									   const Color& color,
									   short shadetop,
									   short shadedown,
									   Orientation shadedir,
									   GLArrayBuffer* buffer);

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
		static void GenerateShadedFormBuffers (const Size* size,
									   int round_type,
									   float radius,
									   const Color& color,
									   short shadetop,
									   short shadedown,
									   Orientation shadedir,
									   short highlight,
									   GLArrayBuffer* inner_buffer,
									   GLArrayBuffer* outer_buffer,
									   GLArrayBuffer* highlight_buffer);

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

		static const float jit[WIDGET_AA_JITTER][2];

	private:

		static float default_border_width;

		Size m_size;

	};

}

#endif /* _BLENDINT_ABSTRACTFORM_HPP_ */
