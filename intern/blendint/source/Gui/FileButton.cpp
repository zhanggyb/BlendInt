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

#include <BlendInt/Gui/FileButton.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>
#include <BlendInt/Gui/Dialog.hpp>

namespace BlendInt {

	FileButton::FileButton ()
	: AbstractButton(String("...")),
	  dialog_(0)
	{
		set_round_type(RoundAll);

		int h = this->text()->font().height();
		int w = h;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);

		InitializeFileButtonOnce();

		events()->connect(clicked(), this, &FileButton::OnClicked);
	}

	FileButton::~FileButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	Size FileButton::GetPreferredSize() const
	{
		int h = this->text()->font().height();
		int w = h;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		return Size(w, h);
	}

	void FileButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (AbstractWindow::theme->regular().shaded) {
				GenerateRoundedVertices(Vertical,
						AbstractWindow::theme->regular().shadetop,
						AbstractWindow::theme->regular().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			vbo_.bind(0);
			vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			vbo_.bind(1);
			vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			vbo_.reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void FileButton::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->regular().shadetop,
					AbstractWindow::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	void FileButton::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->regular().shadetop,
					AbstractWindow::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	ResponseType FileButton::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders->widget_inner_program()->use();

		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		if (is_down()) {
			glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
			        AbstractWindow::theme->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 15);
			}

			glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
					AbstractWindow::theme->regular().inner.data());
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION), 0.f, 0.f);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1,
		        AbstractWindow::theme->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
			        0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		DrawIconText();

		return Finish;
	}

	void FileButton::InitializeFileButtonOnce ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->regular().shadetop,
					AbstractWindow::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(2, vao_);
		vbo_.generate ();

		glBindVertexArray(vao_[0]);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		vbo_.reset();

	}

	void FileButton::OnClicked (AbstractButton* sender)
	{
		AbstractWindow* context = AbstractWindow::GetContext(this);
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
