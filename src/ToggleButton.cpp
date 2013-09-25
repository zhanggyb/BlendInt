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

#include <BILO/ToggleButton.hpp>
#include <BILO/ShaderManager.hpp>

namespace BILO {

	ToggleButton::ToggleButton(const String& text)
	: AbstractButton()
	{
		setCheckable(true);
		set_text (text);
	}

	ToggleButton::ToggleButton(Drawable* parent)
	: AbstractButton(parent)
	{
		setCheckable(true);
	}

	ToggleButton::ToggleButton(const String& text, Drawable* parent)
	: AbstractButton(parent)
	{
		setCheckable(true);
		set_text (text);
	}

	ToggleButton::~ToggleButton()
	{

	}

	void ToggleButton::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		FontCache::create(m_font)->print(m_text_outline.left() + padding_.left(), padding_.bottom() + std::abs(m_text_outline.bottom()), m_text);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}
