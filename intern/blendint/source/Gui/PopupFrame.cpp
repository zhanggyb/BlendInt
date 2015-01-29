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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/PopupFrame.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	PopupFrame::PopupFrame()
	: AbstractFloatingFrame(),
	  focused_widget_(0),
	  hovered_widget_(0),
	  layout_(0),
	  cursor_position_(0)
	{
		set_size(400, 300);
		set_round_type(RoundAll);
		set_round_radius(5.f);
		set_refresh(true);

		projection_matrix_ = glm::ortho(
				0.f, (float)size().width(),
				0.f, (float)size().height(),
				100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;
		if (AbstractWindow::theme->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->menu_back().shadetop,
					AbstractWindow::theme->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(3, vao_);
		glBindVertexArray(vao_[0]);

		buffer_.generate();
		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::FRAME_INNER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::FRAME_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COORD), 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[2]);

		GLfloat vertices[] = {
				// coord											uv
				0.f, 0.f,											0.f, 0.f,
				(float)size().width(), 0.f,							1.f, 0.f,
				0.f, (float)size().height(),						0.f, 1.f,
				(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		buffer_.bind(2);
		buffer_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				AbstractWindow::shaders->location (Shaders::FRAME_IMAGE_COORD));
		glEnableVertexAttribArray (
				AbstractWindow::shaders->location (Shaders::FRAME_IMAGE_UV));
		glVertexAttribPointer (AbstractWindow::shaders->location (Shaders::FRAME_IMAGE_COORD),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (AbstractWindow::shaders->location (Shaders::FRAME_IMAGE_UV), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		buffer_.reset();

		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
	}

	PopupFrame::~PopupFrame()
	{
		glDeleteVertexArrays(3, vao_);

		if(focused_widget_) {
			delegate_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &PopupFrame::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &PopupFrame::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	void PopupFrame::SetLayout(AbstractLayout* layout)
	{
		if(!layout) return;

		if(layout == layout_) {
			DBG_PRINT_MSG("layout %s is already in this frame, skip this function", layout->name().c_str());
			return;
		}

		if(layout_) {
			layout_->destroyed().disconnectOne(this, &PopupFrame::OnLayoutDestroyed);
		}

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			layout->AddWidget(dynamic_cast<AbstractWidget*>(p));
		}

		if(PushBackSubView(layout)) {
			layout_ = layout;
			events()->connect(layout_->destroyed(), this, &PopupFrame::OnLayoutDestroyed);
			MoveSubViewTo(layout_, 0, 0);
			ResizeSubView(layout_, size());
		} else {
			DBG_PRINT_MSG("Warning: %s", "Fail to set layout");
		}

		RequestRedraw();
	}

	void PopupFrame::AddWidget(AbstractWidget* widget)
	{
		if(layout_) {
			layout_->AddWidget(widget);
		} else {
			PushBackSubView(widget);
		}

		RequestRedraw();
	}

	void PopupFrame::InsertWidget(int index, AbstractWidget* widget)
	{
		if(layout_) {
			layout_->InsertWidget(index, widget);
		} else {
			InsertSubView(index, widget);
		}

		RequestRedraw();
	}

	AbstractView* PopupFrame::GetFocusedView() const
	{
		return focused_widget_;
	}

	void PopupFrame::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			projection_matrix_  = glm::ortho(
				0.f,
				0.f + size().width(),
				0.f,
				0.f + size().height(),
				100.f, -100.f);

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (AbstractWindow::theme->menu_back().shaded) {
				GenerateRoundedVertices(Vertical,
						AbstractWindow::theme->menu_back().shadetop,
						AbstractWindow::theme->menu_back().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			buffer_.bind(2);
			float* ptr = (float*)buffer_.map();
			*(ptr + 4) = (float)size().width();
			*(ptr + 9) = (float)size().height();
			*(ptr + 12) = (float)size().width();
			*(ptr + 13) = (float)size().height();
			buffer_.unmap();

			buffer_.reset();

			if(layout_) {
				ResizeSubView(layout_, size());
			}

			shadow_->Resize(size());

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void PopupFrame::PerformRoundTypeUpdate(int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->menu_back().shadetop,
					AbstractWindow::theme->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		shadow_->SetRoundType(round_type);
	}

	void PopupFrame::PerformRoundRadiusUpdate(float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->menu_back().shadetop,
					AbstractWindow::theme->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		shadow_->SetRadius(radius);
	}

	bool PopupFrame::PreDraw(AbstractWindow* context)
	{
		if(!visiable()) return false;

		const_cast<AbstractWindow*>(context)->register_active_frame(this);

		if(refresh()) {
			//DBG_PRINT_MSG("%s", "refresh once");
			RenderSubFramesToTexture(this, context, projection_matrix_, model_matrix_, &texture_buffer_);
		}

		return true;
	}

	ResponseType PopupFrame::Draw(AbstractWindow* context)
	{
		shadow_->Draw(position().x(), position().y());

		AbstractWindow::shaders->frame_inner_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::FRAME_INNER_COLOR), 1, AbstractWindow::theme->menu_back().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		AbstractWindow::shaders->frame_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		glUniform4fv(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COLOR), 1,
		        AbstractWindow::theme->menu_back().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        AbstractWindow::shaders->frame_image_program()->use();

        texture_buffer_.bind();
        glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_POSITION), position().x(), position().y());
        glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
        glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_GAMMA), 0);

        glBindVertexArray(vao_[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        texture_buffer_.reset();
		GLSLProgram::reset();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Finish;
	}

	void PopupFrame::PostDraw(AbstractWindow* context)
	{
	}

	void PopupFrame::PerformFocusOn (AbstractWindow* context)
	{
		DBG_PRINT_MSG("%s", "focus on");
	}

	void PopupFrame::PerformFocusOff (AbstractWindow* context)
	{
		DBG_PRINT_MSG("%s", "focus out");

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &PopupFrame::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
			hovered_widget_ = 0;
		}
	}

	void PopupFrame::PerformHoverIn(AbstractWindow* context)
	{
	}

	void PopupFrame::PerformHoverOut(AbstractWindow* context)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &PopupFrame::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_, context);
			hovered_widget_ = 0;
		}

		//DBG_PRINT_MSG("%s", "hover out");
	}

	ResponseType PopupFrame::PerformKeyPress(AbstractWindow* context)
	{
		ResponseType response = Ignore;

		if(context->GetKeyInput() == Key_Escape) {
			RequestRedraw();
			delete this;
			return Finish;
		}

		if(focused_widget_) {
			const_cast<AbstractWindow*>(context)->register_active_frame(this);
			response = DispatchKeyEvent(focused_widget_, context);
		}

		return response;
	}

	ResponseType PopupFrame::PerformContextMenuPress(AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType PopupFrame::PerformContextMenuRelease(AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType PopupFrame::PerformMousePress(AbstractWindow* context)
	{
		const_cast<AbstractWindow*>(context)->register_active_frame(this);

		if(cursor_position_ == InsideRectangle) {

			last_position_ = position();
			cursor_point_ = context->GetCursorPosition();

			if(hovered_widget_) {

				AbstractView* widget = 0;	// widget may be focused

				widget = DispatchMousePressEvent(hovered_widget_, context);

				if(widget == 0) {
					DBG_PRINT_MSG("%s", "widget 0");
					set_pressed(true);
				} else if (widget == layout_) {
					DBG_PRINT_MSG("%s", "hovered is layout");
					set_pressed(true);
				} else {
					SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
				}

			} else {
				set_pressed(true);
			}

		} else if (cursor_position_ == OutsideRectangle) {
			set_pressed(false);
		}

		return Finish;
	}

	ResponseType PopupFrame::PerformMouseRelease(AbstractWindow* context)
	{
		cursor_position_ = InsideRectangle;
		set_pressed(false);

		if(focused_widget_) {
			const_cast<AbstractWindow*>(context)->register_active_frame(this);
			return delegate_mouse_release_event(focused_widget_, context);
		}

		return Ignore;
	}

	ResponseType PopupFrame::PerformMouseMove(AbstractWindow* context)
	{
		ResponseType retval = Ignore;

		if(pressed_ext()) {

			int ox = context->GetCursorPosition().x() - cursor_point_.x();
			int oy = context->GetCursorPosition().y() - cursor_point_.y();

			set_position(last_position_.x() + ox, last_position_.y() + oy);

			if(superview()) {
				superview()->RequestRedraw();
			}
			retval = Finish;

		} else {

			if(focused_widget_) {

				const_cast<AbstractWindow*>(context)->register_active_frame(this);
				retval = delegate_mouse_move_event(focused_widget_, context);

			}

		}

		return retval;
	}

	ResponseType PopupFrame::DispatchHoverEvent(AbstractWindow* context)
	{
		if(pressed_ext()) return Finish;

		if(Contain(context->GetCursorPosition())) {

			cursor_position_ = InsideRectangle;

			if(!hover()) {
				set_hover(true);
				PerformHoverIn(context);
			}

			AbstractWidget* new_hovered_widget = DispatchHoverEventsInWidgets(hovered_widget_, context);

			if(new_hovered_widget != hovered_widget_) {

				if(hovered_widget_) {
					hovered_widget_->destroyed().disconnectOne(this,
							&PopupFrame::OnHoverWidgetDestroyed);
				}

				hovered_widget_ = new_hovered_widget;
				if(hovered_widget_) {
					events()->connect(hovered_widget_->destroyed(), this,
							&PopupFrame::OnHoverWidgetDestroyed);
				}

			}

		} else {
			cursor_position_ = OutsideRectangle;
			if(hover()) {
				set_hover(false);
				PerformHoverOut(context);
			}
		}

		return Finish;
	}

	void PopupFrame::SetFocusedWidget(AbstractWidget* widget, AbstractWindow* context)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			delegate_focus_off(focused_widget_, context);
			focused_widget_->destroyed().disconnectOne(this, &PopupFrame::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			delegate_focus_on(focused_widget_, context);
			events()->connect(focused_widget_->destroyed(), this, &PopupFrame::OnFocusedWidgetDestroyed);
		}
	}

	void PopupFrame::OnFocusedWidgetDestroyed(AbstractWidget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &PopupFrame::OnFocusedWidgetDestroyed);

		focused_widget_ = nullptr;
	}

	void PopupFrame::OnHoverWidgetDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &PopupFrame::OnHoverWidgetDestroyed);

		hovered_widget_ = nullptr;
	}

	void PopupFrame::OnLayoutDestroyed(AbstractWidget* layout)
	{
		assert(layout == layout_);

		DBG_PRINT_MSG("layout %s is destroyed", layout->name().c_str());
		layout->destroyed().disconnectOne(this, &PopupFrame::OnLayoutDestroyed);
		layout_ = 0;
	}

}
