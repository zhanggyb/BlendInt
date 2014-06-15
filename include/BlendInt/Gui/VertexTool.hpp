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

#ifndef _BLENDINT_GUI_VERTEXTOOL_HPP_
#define _BLENDINT_GUI_VERTEXTOOL_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <vector>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	class VertexTool
	{
	public:

		VertexTool ();

		~VertexTool ();

		void Initialize ();

		/**
		 * @brief Generate round vertices for inner and outline of a form
		 * @param[in] size The size of the form
		 * @param[in] border The width of the outline
		 * @param[in] round_type Round type, MUST be one or bit operation of the values in RoundType
		 * @param[in] radius The round radius
		 * @param[out] inner The inner vertices in std::vector
		 * @param[out] outer The outline vertices in std::vector
		 */
		void Setup (const Size& size, int border, int round_type, int radius);

		void Setup (const Size& size,
						int border,
						int round_type,
						int radius,
						const Color& color,
						Orientation shadedir,
						short shadetop,
						short shadedown
						);

		RefPtr<GLArrayBuffer> GenerateInnerBuffer (GLenum usage = GL_STATIC_DRAW);

		void SetInnerBufferData (GLArrayBuffer* buffer, GLenum usage = GL_STATIC_DRAW);

		void UpdateInnerBuffer (GLArrayBuffer* buffer, GLenum usage = GL_STATIC_DRAW);

		RefPtr<GLArrayBuffer> GenerateOuterBuffer (GLenum usage = GL_STATIC_DRAW);

		void SetOuterBufferData (GLArrayBuffer* buffer, GLenum usage = GL_STATIC_DRAW);

		void UpdateOuterBuffer (GLArrayBuffer* buffer, GLenum usage = GL_STATIC_DRAW);

		RefPtr<GLArrayBuffer> GenerateEmbossBuffer (GLenum usage = GL_STATIC_DRAW);

		void SetEmbossBufferData (GLArrayBuffer* buffer, GLenum usage = GL_STATIC_DRAW);

		void UpdateEmbossBuffer (GLArrayBuffer* buffer, GLenum usage = GL_STATIC_DRAW);

		const GLfloat* inner_data () const
		{
			return m_inner.data();
		}

		const GLfloat* outer_data () const
		{
			return m_outer.data();
		}

		unsigned int total () const
		{
			return m_total;
		}

		unsigned int half () const
		{
			return m_half;
		}

	private:

		void GenerateTriangleStripVertices (
						const std::vector<GLfloat>& inner,
						const std::vector<GLfloat>& outer,
						unsigned int num,
						std::vector<GLfloat>& strip);

		void GenerateOpenTriangleStripVertices (
						const std::vector<GLfloat>& outer,
						unsigned int num,
						std::vector<GLfloat>& strip);


		unsigned int m_total;	/**< total number of vertices for widget */

		unsigned int m_half;	/**< halfway vertices number */

		std::vector<GLfloat> m_inner;
		std::vector<GLfloat> m_outer;

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

	};

}

#endif /* _BLENDINT_GUI_VERTEXTOOL_HPP_ */
