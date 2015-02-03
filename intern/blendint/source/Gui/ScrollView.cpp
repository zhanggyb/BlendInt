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
#endif  // __UNIX__

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <BlendInt/Gui/ScrollView.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	ScrollView::ScrollView()
	: AbstractScrollable(),
	  vao_(0),
	  m_orientation(Horizontal | Vertical),
	  moving_(false)
	{
		set_size(400, 300);

		std::vector<GLfloat> inner_verts;
		GenerateRoundedVertices(&inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		inner_.generate();
		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_.reset();
	}

	ScrollView::~ScrollView ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ScrollView::Setup (AbstractWidget* widget)
	{
		if(PushBackSubView(widget)) {
			RequestRedraw();
		}
	}

	void ScrollView::CentralizeViewport()
	{
		if(first_subview() == 0) return;

		AbstractView* p = first_subview();

		int w = size().width();
		int h = size().height();

		int x = position().x();
		x += (w - static_cast<int>(p->size().width())) / 2;
		int y = position().y();
		y += (h - static_cast<int>(p->size().height())) / 2;

		MoveSubViewTo(p, x, y);
	}

	int BlendInt::ScrollView::GetHPercentage () const
	{
		int percentage = 0;

		if(first_subview()) {
			AbstractView* p = first_subview();

			int w = size().width();

			if (p->size().width() <= w) {
				percentage = 100;
			} else {
				percentage = w * 100 / p->size().width();
			}

		}

		return percentage;
	}

	int BlendInt::ScrollView::GetVPercentage () const
	{
		int percentage = 0;

		if(first_subview()) {
			AbstractView* p = first_subview();

			int h = size().height();

			if (p->size().height() <= h) {
				percentage = 100;
			} else {
				percentage = h * 100 / p->size().height();
			}

		}

		return percentage;
	}

	void ScrollView::MoveViewport(int x, int y)
	{
		if(first_subview()) {

			if(x != 0 || y != 0) {
				AbstractView* p = first_subview();
				MoveSubViewTo(p, p->position().x() + x, p->position().y() + y);

				RequestRedraw();
			}
		}
	}

	void ScrollView::SetReletivePosition (int x, int y)
	{
		if(first_subview()) {
			AbstractView* p = first_subview();

			MoveSubViewTo(p, position().x() + x, position().y() + y);

			RequestRedraw();
		}
	}

	bool ScrollView::IsExpandX() const
	{
		if(first_subview()) {
			return first_subview()->IsExpandX();
		} else {
			return false;
		}
	}

	bool ScrollView::IsExpandY() const
	{
		if(first_subview()) {
			return first_subview()->IsExpandY();
		} else {
			return false;
		}
	}

	Size ScrollView::GetPreferredSize() const
	{
		Size prefer(400, 300);

		AbstractView* widget = first_subview();

		if(widget) {
			prefer = widget->GetPreferredSize();
		}

		return prefer;
	}

	void ScrollView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			std::vector<GLfloat> inner_verts;
			GenerateRoundedVertices(&inner_verts, 0);

			inner_.bind();
			inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			inner_.reset();

			// align the subwidget
			if (first_subview()) {

				int dy = request.size()->height() - size().height();

				first_subview()->MoveTo(first_subview()->position().x(),
				        first_subview()->position().y() + dy);
			}

			set_size(*request.size());
		}

		ReportSizeUpdate(request);
	}

	bool ScrollView::PreDraw(AbstractWindow* context)
	{
		if(!visiable()) return false;
		AbstractWindow* c = const_cast<AbstractWindow*>(context);

		glm::mat3 matrix = glm::translate(AbstractWindow::shaders->widget_model_matrix(),
				glm::vec2(position().x(), position().y()));

		AbstractWindow::shaders->PushWidgetModelMatrix();
		AbstractWindow::shaders->SetWidgetModelMatrix(matrix);

		AbstractWindow::shaders->widget_inner_program()->use();

		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		if(subs_count()) {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 0.908f, 0.208f, 0.208f, 0.25f);
		} else {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 0.947f, 0.447f, 0.447f, 0.25f);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		c->BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		c->EndPushStencil();

		glBindVertexArray(0);

		GLSLProgram::reset();

		return true;
	}

	ResponseType ScrollView::Draw (AbstractWindow* context)
	{
		if(subs_count()) {

			Point offset = GetOffset();

			glm::mat3 matrix = glm::translate(AbstractWindow::shaders->widget_model_matrix(),
					glm::vec2(offset.x(), offset.y()));

			AbstractWindow::shaders->PushWidgetModelMatrix();
			AbstractWindow::shaders->SetWidgetModelMatrix(matrix);

			return Ignore;

		} else {
			return Finish;
		}
	}

	void ScrollView::PostDraw(AbstractWindow* context)
	{
		if(subs_count())
			AbstractWindow::shaders->PopWidgetModelMatrix();

		// draw mask
		AbstractWindow::shaders->widget_inner_program()->use();
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		if(subs_count()) {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 0.908f, 0.208f, 0.208f, 0.25f);
		} else {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 0.947f, 0.447f, 0.447f, 0.25f);
		}

		glBindVertexArray(vao_);
		const_cast<AbstractWindow*>(context)->BeginPopStencil();	// pop inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		const_cast<AbstractWindow*>(context)->EndPopStencil();
		glBindVertexArray(0);
		GLSLProgram::reset();

		AbstractWindow::shaders->PopWidgetModelMatrix();
	}

	ResponseType ScrollView::PerformMousePress (AbstractWindow* context)
	{
		if (context->GetMouseButton() == MouseButtonMiddle) {
			moving_ = true;
			cursor_point_ = context->GetCursorPosition();
			last_offset_ = GetOffset();

			return Finish;
		}

		return subs_count() ? Ignore : Finish;
	}

	ResponseType ScrollView::PerformMouseRelease(AbstractWindow* context)
	{
		if(moving_) {
			moving_ = false;
			RequestRedraw();
		}

		return subs_count() ? Ignore : Finish;
	}

	ResponseType ScrollView::PerformMouseMove(AbstractWindow* context)
	{
		if(moving_) {

			int ox = context->GetCursorPosition().x() - cursor_point_.x();
			int oy = context->GetCursorPosition().y() - cursor_point_.y();

			set_offset(last_offset_.x() + ox, last_offset_.y() + oy);

			if((ox != 0) || (oy != 0)) {
				RequestRedraw();
			}

			return Finish;
		}

		return Ignore;
	}

}
