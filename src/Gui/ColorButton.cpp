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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ColorButton.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ColorButton::ColorButton ()
		: AbstractButton()
	{
		set_round_type(RoundAll);
		int h = font().GetHeight();
		set_size(4 * h + round_radius() * 2 +default_padding.hsum(),
						h + default_padding.vsum());

		color_.set_red(0.3f);
		color_.set_blue(0.8f);
		color_.set_green(0.2f);

		InitializeColorButton();
	}

	ColorButton::~ColorButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	Size ColorButton::GetPreferredSize () const
	{
		int h = font().GetHeight();

		Size prefer(4 * h + round_radius() * 2 +default_padding.hsum(),
					h + default_padding.vsum());

		return prefer;
	}

	void ColorButton::SetColor(const Color& color)
	{
		color_ = color;
		Refresh();
	}

	void ColorButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			UpdateTextPosition(*request.size(), round_type(), round_radius(),
			        text());
			VertexTool tool;
			tool.GenerateVerticesExt(*request.size(), DefaultBorderWidth(), round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void ColorButton::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if (request.target() == this) {
			UpdateTextPosition(size(), *request.round_type(), round_radius(),
			        text());
			VertexTool tool;
			tool.GenerateVerticesExt(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			set_round_type(*request.round_type());
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void ColorButton::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if (request.target() == this) {
			UpdateTextPosition(size(), round_type(), *request.round_radius(),
			        text());
			VertexTool tool;
			tool.GenerateVerticesExt(size(), DefaultBorderWidth(), round_type(),
			        *request.round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType ColorButton::Draw (Profile& profile)
	{
		int outline_vertices = GetOutlineVertices(round_type());

		RefPtr<GLSLProgram> program =
				Shaders::instance->widget_program();
		program->Use();

		glUniform3f(Shaders::instance->location(Stock::WIDGET_POSITION),
				(float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 0);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1, color_.data());

		if(hover()) {
			glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 15);
		} else {
			glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 0);
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertices + 2);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS),
				1);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1, Theme::instance->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, outline_vertices * 2 + 2);

		if (emboss()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 1.0f, 1.0f, 1.0f, 0.16f);

			glUniform3f(Shaders::instance->location(Stock::WIDGET_POSITION),
					(float) position().x(), (float) position().y() - 1.f, 0.f);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->reset();

		if(text().size()) {
			font().Print(position(), text(), text_length(), 0);
		}

		return Accept;
	}

	void ColorButton::InitializeColorButton ()
	{
		VertexTool tool;
		tool.GenerateVerticesExt (size(), DefaultBorderWidth(), round_type(), round_radius());

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(
				Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(
				Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

}
