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

#include <BlendInt/MenuItem.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/FontCache.hpp>

namespace BlendInt {

	MenuItem::MenuItem ()
	: FormBase(), m_highlight(false)
	{
		Resize(200, 20);
	}

	MenuItem::MenuItem (const String& text)
	: FormBase(), m_highlight(false)
	{
		Resize(200, 20);
		m_text = text;
	}


	MenuItem::~MenuItem()
	{

	}

	void MenuItem::Update(int type, const void* data)
	{
		// Do nothing here
	}

	void MenuItem::Render ()
	{
//		FontCache::create(Font())->print(m_text);

		FontCache* fc = FontCache::create(Font());

		int y = (size().height() - fc->get_height()) / 2 + std::abs(fc->get_descender());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(m_highlight) {
			glColor4ub(themes()->menu_item.inner_sel.r(),
					themes()->menu_item.inner_sel.g(),
					themes()->menu_item.inner_sel.b(),
					themes()->menu_item.inner_sel.a());
		} else {
			glColor4ub(themes()->menu_item.inner.r(),
					themes()->menu_item.inner.g(),
					themes()->menu_item.inner.b(),
					themes()->menu_item.inner.a());
		}

		glRectf(0.0, 0.0, size().width(), size().height());

		glColor4ub(themes()->menu_item.text.r(),
				themes()->menu_item.text.g(),
				themes()->menu_item.text.b(),
				themes()->menu_item.text.a());

		FontCache::create(Font())->print(4, y, m_text);	// move offset x 4

		glDisable(GL_BLEND);
	}

}
