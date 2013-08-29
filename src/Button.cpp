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

#include <BIL/Button.hpp>
#include <iostream>

namespace BIL {

	Button::Button (Widget* parent)
		: AbstractButton(parent)
	{
	}

	Button::Button (const String& text, Widget* parent)
		: AbstractButton(parent)
	{
		set_text (text);
	}

	Button::~Button ()
	{

	}

	void Button::update ()
	{
		if (!size_.IsValid()) return;
		
		float rad;

		/* half rounded */
		// TODO: define widget_unit by user
		//rad = 0.2f * U.widget_unit;
		rad = 0.2f * 20;

		//round_box_edges(&wtb, roundboxalign, rect, rad);
		CalculateRoundBoxEdges (round_box_type_, Rect(pos_, size_), rad, &appearance_);
	}

	void Button::render ()
	{
		drawButton (&appearance_);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		text_.render();

		glPopMatrix();
		//glDisable(GL_BLEND);
	}

}
