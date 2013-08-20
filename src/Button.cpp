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

#include <BIL/Button.hpp>

namespace BIL {

	Button::Button (Widget* parent)
		: AbstractButton(parent)
	{

	}

	Button::Button (const wstring& text, Widget* parent)
		: AbstractButton(parent)
	{
		SetText (text);
	}

	Button::~Button ()
	{

	}

	void Button::Render (void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 pos_.z());

		glColor4f(background_.rgba.r,
				  background_.rgba.g,
				  background_.rgba.b,
				  background_.rgba.a);

		DrawBox(GL_POLYGON,
				0.0f,
				0.0f,
				size_.x(),
				size_.y(),
				corner_radius_);

		text_.Render();

		glPopMatrix();
		glDisable(GL_BLEND);
	}
}
