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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Icons.hpp>

#include <BlendInt/Core/Image.hpp>

namespace BlendInt {

	using Stock::Shaders;
	using Stock::Icons;

	ToolButton::ToolButton ()
	: AbstractButton()
	{
		set_size(24, 24);
		set_round_type(RoundAll);

		InitializeToolButton();
	}

	ToolButton::~ToolButton ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	void ToolButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(*request.size(), round_type(),
					round_radius(), text());
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(), round_type(), round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_size(*request.size());
			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ToolButton::PerformRoundTypeUpdate(const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(size(), *request.round_type(), round_radius(),
					text());
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_round_type(*request.round_type());
			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void ToolButton::PerformRoundRadiusUpdate(const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(size(), round_type(), *request.round_radius(),
					text());
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(),
			        round_type(), *request.round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_round_radius(*request.round_radius());

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType ToolButton::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		glm::vec3 pos((GLfloat)position().x(), (GLfloat)position().y(), 0.f);

		glUniform3fv(Shaders::instance->location(Stock::TRIANGLE_POSITION), 1, glm::value_ptr(pos));
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		if (is_down()) {
			glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR),
			        Theme::instance->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 15);
			}

			glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR),
					Theme::instance->regular().inner.data());
		}

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 1);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 0.f, 0.f,
		        0.f, 1.f);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION),
			        (float) position().x(), (float) position().y() - 1.f, 0.f);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->reset();

		if(icon_) {
			pos.x += (size().width() - icon_->size().width()) / 2;
			pos.y += (size().height() - icon_->size().height()) / 2;

			if(hover()) {
				icon_->Draw(pos, 15);
			} else {
				icon_->Draw(pos, 0);
			}
		}
		return Accept;
	}

	void ToolButton::SetAction (const String& text)
	{
		RefPtr<Action> action(new Action(text));

		action_ = action;
	}

	void ToolButton::SetAction (const String& text, const String& shortcut)
	{
		RefPtr<Action> action(new Action(text, shortcut));

		action_ = action;
	}

	void ToolButton::SetAction (const RefPtr<AbstractIcon>& icon, const String& text)
	{
		RefPtr<Action> action(new Action(icon, text));

		action_ = action;

		icon_ = icon;	// for temporary use
	}

	void ToolButton::SetAction (const RefPtr<AbstractIcon>& icon, const String& text,
	        const String& shortcut)
	{
		RefPtr<Action> action(new Action(icon, text, shortcut));

		action_ = action;
	}

	void ToolButton::SetAction (const RefPtr<Action>& item)
	{
		action_ = item;
	}

	Size ToolButton::GetPreferredSize() const
	{
		return Size(24, 24);
	}

	void ToolButton::InitializeToolButton ()
	{
		VertexTool tool;
		tool.GenerateVertices(size(), DefaultBorderWidth(), RoundAll, 5.f);

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		// demo
		icon_ = Icons::instance->icon_16x16(Stock::IMAGE_ALPHA);
	}

}
