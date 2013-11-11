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
#include <math.h>

#include <BlendInt/PopupWidget.hpp>

#include <BlendInt/Theme.hpp>

namespace BlendInt {

	PopupWidget::PopupWidget()
	: Frame()
	{
		reset_z(1);	// TODO: define layer in enumeration
		set_emboss(false);

		m_icon.demo_init();

		m_icon.set_position(50, 50);
	}

	PopupWidget::~PopupWidget()
	{

	}

	void PopupWidget::update(int type, const void* data)
	{
		Frame::update(type, data);

		switch(type) {

			case FormSize: {

				Size shadow_size = *(static_cast<const Size*>(data));

				m_shadow.resize(shadow_size);

				m_icon.resize(16, 16);
				break;
			}

			default:
				break;
		}

	}

	void PopupWidget::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		dispatch_render(&m_shadow);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());
		draw_gl_buffer(WidgetBufferKeyInner);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_gl_buffer_anti_alias(WidgetBufferKeyOuter);

		if(emboss()) {
			glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
			draw_gl_buffer_anti_alias(WidgetBufferKeyEmboss);
		}

		glDisable(GL_BLEND);

		dispatch_render(&m_icon);

		glPopMatrix();
	}


}
