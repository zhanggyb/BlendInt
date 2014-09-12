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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Gui/AbstractIcon.hpp>

namespace BlendInt {

	AbstractIcon::AbstractIcon (int width, int height)
	: AbstractForm()
	{
		set_size(width, height);
	}

	AbstractIcon::~AbstractIcon ()
	{
	}

	void AbstractIcon::Draw(float x, float y, short gamma) const
	{
		glm::vec3 pos(x, y, 0.f);
		Draw(pos, gamma);
	}

	void AbstractIcon::Draw(const glm::vec3& pos, short gamma) const
	{
	}

}
