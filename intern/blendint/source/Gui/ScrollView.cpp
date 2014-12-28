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
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

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

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

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

	void ScrollView::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());

			if(first_subview()) {
				MoveSubWidgets(x, y);
			}
		}

		ReportPositionUpdate(request);
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

	bool ScrollView::PreDraw(Profile& profile)
	{
		if(!visiable()) return false;

		glm::mat3 matrix = glm::translate(Shaders::instance->widget_model_matrix(),
				glm::vec2(position().x(), position().y()));

		Shaders::instance->PushWidgetModelMatrix();
		Shaders::instance->SetWidgetModelMatrix(matrix);

		Shaders::instance->widget_inner_program()->use();

		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);

		if(subs_count()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.908f, 0.208f, 0.208f, 0.25f);
		} else {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.947f, 0.447f, 0.447f, 0.25f);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		profile.BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		profile.EndPushStencil();

		glBindVertexArray(0);

		GLSLProgram::reset();

		return true;
	}

	ResponseType ScrollView::Draw (Profile& profile)
	{
		if(subs_count()) {
			glm::mat3 matrix = glm::translate(Shaders::instance->widget_model_matrix(),
					glm::vec2(offset().x(), offset().y()));

			Shaders::instance->PushWidgetModelMatrix();
			Shaders::instance->SetWidgetModelMatrix(matrix);

			return Ignore;

		} else {
			return Finish;
		}
	}

	void ScrollView::PostDraw(Profile& profile)
	{
		Shaders::instance->PopWidgetModelMatrix();

		// draw mask
		Shaders::instance->widget_inner_program()->use();
		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);

		if(subs_count()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.908f, 0.208f, 0.208f, 0.25f);
		} else {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.947f, 0.447f, 0.447f, 0.25f);
		}

		glBindVertexArray(vao_);
		profile.BeginPopStencil();	// pop inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		profile.EndPopStencil();
		glBindVertexArray(0);
		GLSLProgram::reset();

		Shaders::instance->PopWidgetModelMatrix();
	}

	ResponseType ScrollView::MousePressEvent (const MouseEvent& event)
	{
		if (event.button() == MouseButtonMiddle) {
			moving_ = true;
			cursor_point_ = event.position();
			last_offset_ = offset();
		}

		return Finish;
	}

	ResponseType ScrollView::MouseReleaseEvent(const MouseEvent& event)
	{
		if(moving_) {
			moving_ = false;
			RequestRedraw();
		}

		return Finish;
	}

	ResponseType ScrollView::MouseMoveEvent(const MouseEvent& event)
	{
		if(moving_) {

			int ox = event.position().x() - cursor_point_.x();
			int oy = event.position().y() - cursor_point_.y();

			set_offset(last_offset_.x() + ox, last_offset_.y() + oy);

			if((ox != 0) || (oy != 0)) {
				RequestRedraw();
			}

			return Finish;
		}

		return Ignore;
	}

}
