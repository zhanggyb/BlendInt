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
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/Types.hpp>
#include <BlendInt/Gui/Icon.hpp>

namespace BlendInt {

	Icon::Icon ()
	: AbstractResizableForm()
	{
	}

	Icon::Icon (const Icon& orig)
	{
		set_size(orig.size());
	}

	Icon::~Icon ()
	{
	}

	Icon& Icon::operator = (const Icon& orig)
	{
		set_size(orig.size());
		return *this;
	}

	bool Icon::Update (int type, const void* data)
	{
		// Do nothing
		return true;
	}

	void Icon::Scale (float ratio)
	{
		if(ratio < 0.0) return;

		Size new_size;
		new_size.set_width(static_cast<unsigned int>(size().width() * ratio));
		new_size.set_height(static_cast<unsigned int>(size().height() * ratio));

		Update(FormSize, &new_size);
		set_size(new_size);
	}

	void Icon::Draw ()
	{
#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0.0, 0.0);
		glVertex2f(size().width(), 0.0);
		glVertex2f(size().width(), size().height());
		glVertex2f(0.0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif

	}

}
