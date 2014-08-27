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

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	class VertexTool
	{
	public:

		VertexTool ();

		~VertexTool ();

		void GenerateVertices (const Size& size, float border, int round_type, float radius);

		void GenerateVertices (const Size& size,
				float border,
				int round_type,
				float radius,
				const Color& color,
				Orientation shadedir,
				short shadetop,
				short shadedown);

		inline int inner_size () const
		{
			return sizeof(GLfloat) * inner_.size();
		}

		inline int outer_size () const
		{
			return sizeof(GLfloat) * outer_.size();
		}

		const GLfloat* inner_data () const
		{
			return &inner_[0];
		}

		const GLfloat* outer_data () const
		{
			return &outer_[0];
		}

		const std::vector<GLfloat>& inner () const
		{
			return inner_;
		}

		const std::vector<GLfloat>& outer () const
		{
			return outer_;
		}

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

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

		std::vector<GLfloat> inner_;

		std::vector<GLfloat> outer_;
	};

}

#endif /* _BLENDINT_GUI_VERTEXTOOL_HPP_ */
