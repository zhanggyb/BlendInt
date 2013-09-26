/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BILO/Frame.hpp>
#include <BILO/FontCache.hpp>

namespace BILO {

	Frame::Frame ()
	: Widget(), m_layout(0)
	{

	}

	Frame::Frame (Drawable* parent)
	: Widget (parent), m_layout(0)
	{

	}

	Frame::~Frame ()
	{

	}

	void Frame::set_layout(AbstractLayout* layout)
	{
		if(m_layout == layout) return;
	}

	void Frame::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_buffer.is_buffer(0)) {
			Theme* tm = Theme::instance();

			glColor4ub(tm->themeUI()->wcol_regular.inner.r(),
			        tm->themeUI()->wcol_regular.inner.g(),
			        tm->themeUI()->wcol_regular.inner.b(),
			        tm->themeUI()->wcol_regular.inner.a() * 0.5f);

			m_buffer.bind(GL_ARRAY_BUFFER);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glDrawArrays(GL_POLYGON, 0, 4);

			glColor4ub(tm->themeUI()->wcol_regular.outline.r(),
			        tm->themeUI()->wcol_regular.outline.g(),
			        tm->themeUI()->wcol_regular.outline.b(),
			        tm->themeUI()->wcol_regular.outline.a());

			glDrawArrays(GL_LINE_LOOP, 0, 4);

			glDisableClientState(GL_VERTEX_ARRAY);

			m_buffer.unbind(GL_ARRAY_BUFFER);
		}

		if(!m_layout) {
			FontCache::create()->print (10, size_.height()/2, "No layout is set");
		}

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}