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
#include <BlendInt/Gui/Dialog.hpp>

#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FileButton::FileButton ()
	: AbstractButton(),
	  dialog_(0)
	{
		set_round_type(RoundAll);

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
			tool.GenerateVertices(*request.size(), default_border_width(),
			        round_type(), round_radius());
			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			set_size(*request.size());
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void FileButton::PerformRoundTypeUpdate (int round_type)
	{
		UpdateTextPosition(size(), round_type, round_radius(),
				text());
		VertexTool tool;
		tool.GenerateVertices(size(), default_border_width(), round_type,
				round_radius());
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		set_round_type(round_type);
		RequestRedraw();
	}

	void FileButton::PerformRoundRadiusUpdate (float radius)
	{
		UpdateTextPosition(size(), round_type(), radius, text());
		VertexTool tool;
		tool.GenerateVertices(size(), default_border_width(),
			        round_type(), radius);
		inner_->bind();
		inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

		set_round_radius(radius);
		RequestRedraw();
	}

	ResponseType FileButton::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->widget_triangle_program();
		program->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION),
		        0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		if (is_down()) {
			glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR),
			        Theme::instance->regular().inner_sel.data());
		} else {
			if (hover()) {
				Color color = Theme::instance->regular().inner + 15;
				glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR),
				        color.data());
			} else {
				glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR),
				        Theme::instance->regular().inner.data());
			}
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 1);
		glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR),
		        Theme::instance->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glVertexAttrib4f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);

			glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION),
					0.f, 0.f - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->reset();

		if (text().size()) {
			font().Print(0.f, 0.f, text(), text_length(), 0);
		}

		return Accept;
	}

	void FileButton::InitializeFileButtonOnce ()
	{
		set_round_type(RoundAll);
		String text("...");
		set_text(text);

		int left = kDefaultPadding.left() * Theme::instance->pixel();
		int right = kDefaultPadding.right() * Theme::instance->pixel();
		int top = kDefaultPadding.top() * Theme::instance->pixel();
		int bottom = kDefaultPadding.bottom() * Theme::instance->pixel();
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
		tool.GenerateVertices (size(), default_border_width(), round_type(), round_radius());

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void FileButton::OnClicked (AbstractButton* sender)
	{
		Context* context = Context::GetContext(this);
		assert(dialog_ == 0);

		if(context) {

			dialog_ = Manage(new FileSelector);
			DBG_SET_NAME(dialog_, "File Selector on File Button");

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

			dialog_->Resize(w, h);
			dialog_->MoveTo(x, y);

			context->AddFrame(dialog_);

			events()->connect(dialog_->destroyed(), this, &FileButton::OnDialogDestroyed);

			//events()->connect(dialog_->opened(), this, &FileButton::OnOpened);
			//events()->connect(dialog_->canceled(), this, &FileButton::OnCanceled);

		}
	}

	void FileButton::OnOpened ()
	{
		// TODO:
		//file_selected_.fire();
	}

	void FileButton::OnCanceled ()
	{
		// TODO:
	}

	void FileButton::OnDialogDestroyed(AbstractFrame* dialog)
	{
		assert(dialog == dialog_);

		dialog_->destroyed().disconnectOne(this, &FileButton::OnDialogDestroyed);
		dialog_ = 0;
	}
}
