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

#include <BlendInt/Gui/ColorButton.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	ColorButton::ColorButton ()
		: AbstractButton()
	{
		set_round_type(RoundAll);
		Font font;	// default font
		set_size(font.height() + kPadding.hsum(),
		        font.height() + kPadding.vsum());

		color0_.set_red(0.3f);
		color0_.set_blue(0.8f);
		color0_.set_green(0.2f);

		color1_.set_red(0.3f);
		color1_.set_blue(0.8f);
		color1_.set_green(0.2f);
		color1_.set_alpha(0.5f);

		InitializeColorButton();
	}

	ColorButton::~ColorButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void ColorButton::SetColor(const Color& color)
	{
		color0_ = color;
		color0_.set_alpha(1.f);
		color1_ = color;
		RequestRedraw();
	}

	void ColorButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			GenerateRoundedVertices(&inner_verts, &outer_verts);
			buffer_.bind(0);
			buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			buffer_.reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ColorButton::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);
		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		RequestRedraw();
	}

	void ColorButton::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);
		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		RequestRedraw();
	}

	ResponseType ColorButton::Draw (const Context* context)
	{
		Point pos = context->active_frame()->GetAbsolutePosition(this);

		int outline_vertices = GetOutlineVertices(round_type());

		Context::shaders->widget_split_inner_program()->use();

		glUniform1f(Context::shaders->location(Shaders::WIDGET_SPLIT_INNER_PARTING), pos.x() + size().width() / 2.f);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_SPLIT_INNER_COLOR0), 1, color0_.data());
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_SPLIT_INNER_COLOR1), 1, color1_.data());

		if(hover()) {
			glUniform1i(Context::shaders->location(Shaders::WIDGET_SPLIT_INNER_GAMMA), 15);
		} else {
			glUniform1i(Context::shaders->location(Shaders::WIDGET_SPLIT_INNER_GAMMA), 0);
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertices + 2);

		Context::shaders->widget_outer_program()->use();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_OUTER_POSITION),
				0.f, 0.f);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1, Context::theme->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, outline_vertices * 2 + 2);

		if (emboss()) {
			glUniform4f(Context::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f, 1.0f, 1.0f, 0.16f);

			glUniform2f(Context::shaders->location(Shaders::WIDGET_OUTER_POSITION),
					0.f, - 1.f);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		if(text()) {
			// font().Print(0.f, 0.f, text(), text_length(), 0);
		}

		return Finish;
	}

	void ColorButton::InitializeColorButton ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		buffer_.generate();

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(
				Context::shaders->location(Shaders::WIDGET_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_INNER_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		glEnableVertexAttribArray(
				Context::shaders->location(Shaders::WIDGET_OUTER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_OUTER_COORD),
				2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

}
