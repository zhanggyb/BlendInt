/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/CircularPicker.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	CircularPicker::CircularPicker (unsigned int radius)
	: AbstractRoundForm()
	{
		if(radius == 0) radius = 1;

		set_size(radius * 2, radius * 2);
		set_round_type(RoundAll);
		set_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		buffer_.generate();
		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		buffer_.reset();

	}

	CircularPicker::~CircularPicker ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void CircularPicker::Resize(unsigned int radius)
	{
		if(radius == 0) radius = 1;

		if(radius == (unsigned int)size().width() / 2) return;

		set_radius(radius);
		set_size(radius * 2, radius * 2);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();
	}

	void CircularPicker::PerformSizeUpdate(const Size& size)
	{
		int radius = std::min(size.width(), size.height());
		if(radius == 0) radius = 1;

		set_radius(radius);
		set_size(radius * 2, radius * 2);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();
	}

	void CircularPicker::PerformRoundTypeUpdate(int type)
	{

	}

	void CircularPicker::PerformRoundRadiusUpdate(float radius)
	{
		unsigned int rad = static_cast<unsigned int>(radius + 0.5f);
		if(rad == 0) rad = 1;

		set_radius(rad);
		set_size(rad * 2, rad * 2);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();
	}

	void CircularPicker::Draw (float x, float y, short gamma) const
	{
		AbstractWindow::shaders->widget_simple_triangle_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_POSITION), x, y);
		glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_COLOR), 1.f, 1.f, 1.f, 1.f);
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA), gamma);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION), x, y);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1, AbstractWindow::theme->scroll().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		GLSLProgram::reset();
	}

}
