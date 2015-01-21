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

#include <math.h>

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/TabButton.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	TabButton::TabButton ()
	: AbstractButton()
	{
		set_size(80, 14);
		set_checkable(true);

		InitializeTabButton();
	}

	TabButton::TabButton (const String& text)
	: AbstractButton()
	{
		set_size(80, 14);
		set_checkable(true);

		RefPtr<Text> t(new Text(text));
		set_text(t);

		InitializeTabButton();
	}

	BlendInt::TabButton::~TabButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void TabButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			std::vector<GLfloat> inner;
			std::vector<GLfloat> outer;
			GenerateTabButtonVertices(*request.size(), default_border_width(),
			        inner, outer);

			vbo_.bind(0);
			vbo_.set_data(sizeof(GLfloat) * inner.size(),
			        &inner[0]);

			vbo_.bind(1);
			vbo_.set_data(sizeof(GLfloat) * outer.size(),
			        &outer[0]);

			GLArrayBuffer::reset();

			set_size(*request.size());
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType TabButton::Draw (const Context* context)
	{
		Context::shaders->widget_triangle_program()->use();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_TRIANGLE_POSITION), 0.f, 0.f);
		glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);

		// draw inner, simple fill
		if (is_checked()) {
			glVertexAttrib4f(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COLOR), 0.447f, 0.447f, 0.447f, 1.0f);
			glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

			glBindVertexArray(vao_[0]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * 11);
		} else {
			glVertexAttrib4fv(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COLOR), Context::theme->tab().item.data());
			glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 1);

			glBindVertexArray(vao_[0]);
			glDrawArrays(GL_TRIANGLE_STRIP, 4, 2 * 11 - 4);
		}

		if (is_checked()) {
			glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 1);
			glVertexAttrib4fv(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COLOR), Context::theme->tab().outline.data());

			glBindVertexArray(vao_[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * 11 * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		if(text()) {
			// font().Print(0.f, 0.f, text(), text_length(), 0);
		}

		return Finish;
	}

	void TabButton::InitializeTabButton()
	{
		std::vector<GLfloat> inner;
		std::vector<GLfloat> outer;

		GenerateTabButtonVertices(size(), default_border_width(), inner, outer);

		glGenVertexArrays(2, vao_);

		glBindVertexArray(vao_[0]);

		vbo_.generate();
		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner.size(), &inner[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer.size(), &outer[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void TabButton::GenerateTabButtonVertices (const Size& size, float border,
					std::vector<GLfloat>& inner, std::vector<GLfloat>& outer)
	{
		int amp = size.height() / 2;
		int shift_x = 5;
		border = Context::theme->pixel() * border;

		if (inner.size() != 2 * 11 * 2)
			inner.resize(2 * 11 * 2);

		size_t count = 0;
		for (int i = 0; i <= 10; i++, count += 4) {
			inner[count + 0] = i;
			inner[count + 1] = sin_curve(i, amp, -shift_x, -border);
			inner[count + 2] = size.width() - i;
			inner[count + 3] = inner[count + 1];
		}

		if (outer.size() != 2 * 11 * 2 * 2)
			outer.resize(2 * 11 * 2 * 2);

		count = 0;
		for (int i = 0; i <= 10; i++, count += 4) {
			outer[count + 0] = i;
			outer[count + 1] = sin_curve(i, amp, -shift_x, 0.0);
			outer[count + 2] = i;
			outer[count + 3] = outer[count + 1] - border;
		}

		for (int i = 0; i <= 10; i++, count += 4) {
			outer[count + 0] = size.width() - outer[4 * (10 - i) + 0];
			outer[count + 1] = outer[4 * (10 - i) + 1];
			outer[count + 2] = size.width() - outer[4 * (10 - i) + 2];
			outer[count + 3] = outer[4 * (10 - i) + 3];
		}
	}

	inline double TabButton::sin_curve (double x, double amplitude,
					double shift_x, double shift_y)
	{
		return amplitude * sin((x + shift_x) / M_PI) + amplitude + shift_y;
	}
	
	Size TabButton::GetPreferredSize () const
	{
		return Size(80, 14);
	}

}
