/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */
#include <GL/glew.h>

#include <BIL/ScrollWidget.hpp>
#include <BIL/Theme.hpp>

#include <BIL/Interface.hpp>

#include <stdio.h>
#include <iostream>

namespace BIL {

	ScrollWidget::ScrollWidget(Drawable* parent)
	: Widget(parent), m_viewport(0)
	{

	}

	ScrollWidget::~ScrollWidget ()
	{

	}

	void ScrollWidget::set_viewport(Widget* widget)
	{
		// TODO: more code needed here
		add_child(widget);
	}

	void ScrollWidget::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable (GL_SCISSOR_TEST);
		glScissor (100, 100, 1000, 1000);

		glColor4f(0.2f, 0.05f, 0.2f, 0.2f);
		glBegin(GL_POLYGON);
			glVertex2d(50,50);
			glVertex2d(1200,50);
			glVertex2d(1200,1200);
			glVertex2d(50,1200);
		glEnd();

		glDisable(GL_SCISSOR_TEST);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}
