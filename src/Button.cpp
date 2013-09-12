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
#include <iostream>

#include <BIL/Button.hpp>
#include <BIL/ShaderManager.hpp>

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

	void Button::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		drawButton (&appearance_);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//text_.render();
		glUseProgram(ShaderManager::instance()->text_program().id());

		GLfloat black[4] = { 0, 0, 0, 1 };

		glUniform4fv(ShaderManager::instance()->text_uniform_color(), 1, black);

		glBindTexture(GL_TEXTURE_2D, FontCache::create(font_)->queryTexture(text_[0]));
		glUniform1i(ShaderManager::instance()->text_uniform_tex(), 0);

		glEnableVertexAttribArray(ShaderManager::instance()->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, ShaderManager::instance()->text_vbo());
		glVertexAttribPointer(ShaderManager::instance()->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6 * text_.length(), vertex_array_, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, valid_text_length_);

		glDisableVertexAttribArray(ShaderManager::instance()->text_attribute_coord());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
		glDisable(GL_BLEND);

		glPopMatrix();


	}

}
