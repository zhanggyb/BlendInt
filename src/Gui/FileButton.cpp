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
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

#include <BlendInt/Gui/FileButton.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FileButton::FileButton ()
	: AbstractButton(),
	  panel_(0)
	{
		set_round_type(RoundAll);
		set_drop_shadow(true);

		InitializeFileButtonOnce();

		events()->connect(clicked(), this, &FileButton::OnClicked);
	}

	FileButton::~FileButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void FileButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(*request.size(), round_type(),
			        round_radius(), text());
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(),
			        round_type(), round_radius());
			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			set_size(*request.size());
			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void FileButton::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
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

			set_round_type(*request.round_type());
			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void FileButton::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(size(), round_type(), *request.round_radius(),
			        text());
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(),
			        round_type(), *request.round_radius());
			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType FileButton::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(),
		        (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		if (is_down()) {
			glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
			        Theme::instance->regular().inner_sel.data());
		} else {
			if (hover()) {
				Color color = Theme::instance->regular().inner + 15;
				glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
				        color.data());
			} else {
				glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
				        Theme::instance->regular().inner.data());
			}
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);
		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
		        Theme::instance->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 1.0f,
			        1.0f, 1.0f, 0.16f);

			glUniform3f(Shaders::instance->triangle_uniform_position(),
			        (float) position().x(), (float) position().y() - 1.f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->reset();

		if (text().size()) {
			font().Print(position(), text(), text_length(), 0);
		}

		return Accept;
	}

	void FileButton::InitializeFileButtonOnce ()
	{
		set_round_type(RoundAll);
		set_drop_shadow(true);
		String text("...");
		set_text(text);

		int left = default_padding.left() * Theme::instance->pixel();
		int right = default_padding.right() * Theme::instance->pixel();
		int top = default_padding.top() * Theme::instance->pixel();
		int bottom = default_padding.bottom() * Theme::instance->pixel();
		int h = font().GetHeight();

		set_text_length(text.length());
		Rect text_outline = font().GetTextOutline(text);

		int width = text_outline.width()
		        + round_radius() * 2 * Theme::instance->pixel() + left + right;
		int height = h + top + bottom;

		set_size(width, height);

		set_pen((width - text_outline.width()) / 2,
		        (height - font().GetHeight()) / 2
		                + std::abs(font().GetDescender()));

		VertexTool tool;
		tool.GenerateVertices (size(), DefaultBorderWidth(), round_type(), round_radius());

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void FileButton::OnClicked ()
	{
		Context* context = Context::GetContext(this);

		if(context) {

			assert(panel_ == 0);

			panel_ = Manage(new StaticPanel);

			FileSelector* file_selector = Manage(new FileSelector);
			DBG_SET_NAME(file_selector, "File Selector on File Button");

			int w = 800;
			int h = 600;

			if(w > context->size().width()) {
				w = context->size().width();
			}

			if(h > context->size().height()) {
				h = context->size().height();
			}

			int x = (context->size().width() - w) / 2;
			int y = (context->size().height() - h) / 2;

			panel_->Resize(w, h);
			panel_->SetPosition(x, y);
			panel_->SetContent(file_selector);
			Section* section = context->PushBack(panel_);
			section->set_mode(Section::Modal);
			context->SetFocusedWidget(file_selector);

			events()->connect(file_selector->opened(), this, &FileButton::OnOpened);
			events()->connect(file_selector->canceled(), this, &FileButton::OnCanceled);

		}
	}

	void FileButton::OnOpened ()
	{
		FileSelector* fs = dynamic_cast<FileSelector*>(panel_->content());
		fs->opened().disconnectOne(this, &FileButton::OnOpened);
		file_ = fs->file_selected();
		delete panel_;
		panel_ = 0;

		file_selected_.fire();
	}

	void FileButton::OnCanceled ()
	{
		FileSelector* fs = dynamic_cast<FileSelector*>(panel_->content());
		fs->canceled().disconnectOne(this, &FileButton::OnCanceled);
		delete panel_;
		panel_ = 0;
	}

}
