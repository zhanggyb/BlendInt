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

#include <BIL/ToggleButton.hpp>
#include <BIL/ShaderManager.hpp>

namespace BIL {

	ToggleButton::ToggleButton(Widget* parent)
	: AbstractButton(parent)
	{
		setCheckable(true);
	}

	ToggleButton::ToggleButton(const String& text, Widget* parent)
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
		drawButton (&appearance_);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		//text_.render();
		glUseProgram(ShaderManager::instance()->text_program().id());

		GLfloat black[4] = { 0, 0, 0, 1 };
		//GLfloat red[4] = { 1, 0, 0, 1 };
		//GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

		/* Set color to black */
		glUniform4fv(ShaderManager::instance()->text_uniform_color(), 1, black);

		/* Use the texture containing the atlas */
		glBindTexture(GL_TEXTURE_2D, FontCache::create(font_)->queryTexture(text_[0]));
		glUniform1i(ShaderManager::instance()->text_uniform_tex(), 0);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(ShaderManager::instance()->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, ShaderManager::instance()->text_vbo());
		glVertexAttribPointer(ShaderManager::instance()->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		/* Draw all the character on the screen in one go */
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6 * text_.length(), vertex_array_, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, valid_text_length_);

		glDisableVertexAttribArray(ShaderManager::instance()->text_attribute_coord());

		glUseProgram(0);

		glPopMatrix();

		glDisable(GL_BLEND);
	}

}
