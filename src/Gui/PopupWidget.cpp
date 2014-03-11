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
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <math.h>

#include <BlendInt/Gui/PopupWidget.hpp>

#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	PopupWidget::PopupWidget()
	: Widget()
	{
		SetLayer(1);	// TODO: define layer in enumeration
	}

	PopupWidget::~PopupWidget()
	{

	}

	bool PopupWidget::Update(int type, const void* data)
	{
		Widget::Update(type, data);
		switch(type) {

			case FormSize: {

				Size shadow_size = *(static_cast<const Size*>(data));

				m_shadow.Resize(shadow_size);

				return true;
			}

			default:
				return Widget::Update(type, data);
		}

	}

	void PopupWidget::Draw (RedrawEvent* event)
	{
		Widget::Draw(event);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		m_shadow.Draw (event->pv_matrix());

		glPopMatrix();
	}


}
