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

#include <GL/glew.h>

#include <BlendInt/ScrollWidget.hpp>
#include <BlendInt/Theme.hpp>

#include <BlendInt/Interface.hpp>

#include <stdio.h>
#include <iostream>

namespace BlendInt {

	ScrollWidget::ScrollWidget()
	: Frame(), m_viewport(0), m_hslider(0), m_vslider(0)
	{

	}

	ScrollWidget::ScrollWidget(AbstractWidget* parent)
	: Frame(parent), m_viewport(0), m_hslider(0), m_vslider(0)
	{

	}

	ScrollWidget::~ScrollWidget ()
	{

	}

	void ScrollWidget::set_viewport(Frame* widget)
	{
		// TODO: more code needed here
		// add_child(widget);
	}

	void ScrollWidget::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
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
