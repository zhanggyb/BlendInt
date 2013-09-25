/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <iostream>

#include <BILO/Button.hpp>
#include <BILO/ShaderManager.hpp>

namespace BILO {

	Button::Button ()
		: AbstractButton()
	{
	}

	Button::Button (const String& text)
		: AbstractButton()
	{
		set_text (text);
	}

	Button::Button (Drawable* parent)
		: AbstractButton(parent)
	{
	}

	Button::Button (const String& text, Drawable* parent)
		: AbstractButton(parent)
	{
		set_text (text);
	}

	Button::~Button ()
	{

	}

	void Button::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		FontCache::create(m_font)->print(m_text_outline.left() + padding_.left(), padding_.bottom() + std::abs(m_text_outline.bottom()), m_text);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}
