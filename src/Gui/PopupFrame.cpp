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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <math.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/PopupFrame.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	PopupFrame::PopupFrame()
	: Frame()
	{
		//SetLayer(1);	// TODO: define layer in enumeration
	}

	PopupFrame::~PopupFrame()
	{

	}

	bool PopupFrame::Update (const UpdateRequest& request)
	{
		if (request.source() == Predefined) {

			switch (request.type()) {

				case FormSize: {
					return true;
				}

				default:
					return Frame::Update(request);
			}

		} else {
			return false;
		}

	}

	void PopupFrame::Draw (RedrawEvent* event)
	{
		Frame::Draw(event);
	}


}
