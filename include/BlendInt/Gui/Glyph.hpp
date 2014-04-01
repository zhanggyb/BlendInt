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

/**
 * @brief Code for character glyph
 */

#ifndef _BLENDINT_GLYPH_HPP_
#define _BLENDINT_GLYPH_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

namespace BlendInt {

	struct GlyphVertex
	{
		GLfloat x;	// coord x
		GLfloat y;	// coord y
		GLfloat s;	// texture coord u
		GLfloat t;	// texture coord v
	};

	struct Glyph
	{
		// Glyph ();

		float bitmap_width;
		float bitmap_height;

		float bitmap_left;
		float bitmap_top;

		float advance_x;
		float advance_y;

		float texture_offset_x;
		float texture_offset_y;

		GlyphVertex vertexes[4];
	};

} /* namespace BlendInt */

#endif  /* _BIL_GLYPH_H_ */
