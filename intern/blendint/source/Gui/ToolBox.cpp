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
#include <glm/gtx/matrix_transform_2d.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	ToolBox::ToolBox (Orientation orientation)
	: Frame(),
	  focused_widget_(0),
	  hovered_widget_(0),
	  space_(1),
	  margin_(2, 2, 2, 2),
	  orientation_(orientation),
	  cursor_position_(0)
	{
		if(orientation_ == Horizontal) {
			set_size(400, 240);
		} else {
			set_size(240, 400);
		}

		InitializeToolBoxOnce();
	}

	ToolBox::ToolBox (int width, int height, Orientation orientation)
	: Frame(width, height),
	  focused_widget_(0),
	  hovered_widget_(0),
	  space_(1),
	  orientation_(orientation),
	  cursor_position_(0)
	{
		InitializeToolBoxOnce();
	}

	ToolBox::~ToolBox()
	{
		glDeleteVertexArrays(3, vao_);

		if(focused_widget_) {
			delegate_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	void ToolBox::AddWidget (AbstractWidget* widget)
	{
		if(orientation_ == Horizontal) {

			int x = GetLastPosition();
			int y = margin_.bottom();
			int h = size().height() - margin_.vsum();

			if(PushBackSubView(widget)) {

				Size prefer = widget->GetPreferredSize();
				MoveSubViewTo(widget, x, y);
				if(widget->IsExpandY()) {
					ResizeSubView(widget, prefer.width(), h);
				} else {
					if(widget->size().height() > h) {
						ResizeSubView(widget, prefer.width(), h);
					} else {
						ResizeSubView(widget, prefer.width(), widget->size().height());
						MoveSubViewTo(widget, x,
										y + (h - widget->size().height()) / 2);
					}
				}

				RequestRedraw();
			}

		} else {

			int x = margin_.left();
			int y = GetLastPosition();
			int w = size().width() - margin_.hsum();

			if(PushBackSubView(widget)) {
				Size prefer = widget->GetPreferredSize();
				y = y - prefer.height();
				MoveSubViewTo(widget, x, y);
				if(widget->IsExpandX()) {
					ResizeSubView(widget, w, prefer.height());
				} else {
					if(widget->size().width() > w) {
						ResizeSubView(widget, w, prefer.height());
					} else {
						ResizeSubView(widget, widget->size().width(), prefer.height());
					}
				}

				RequestRedraw();
			}

		}
	}

	bool ToolBox::IsExpandX () const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool ToolBox::IsExpandY () const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size ToolBox::GetPreferredSize () const
	{
		Size preferred_size;

		if(subs_count() == 0) {

			if(orientation_ == Horizontal) {
				preferred_size.reset(400, 240);
			} else {
				preferred_size.reset(240, 400);
			}

		} else {

			Size tmp;

			if(orientation_ == Horizontal) {

				preferred_size.set_width(-space_);

				for(AbstractView* p = first_subview(); p; p = p->next_view())
				{
					if(p->visiable()) {
						tmp = p->GetPreferredSize();

						preferred_size.add_width(tmp.width() + space_);
						preferred_size.set_height(std::max(preferred_size.height(), tmp.height()));
					}
				}

				preferred_size.add_width(margin_.hsum());
				preferred_size.add_height(margin_.vsum());

			} else {

				preferred_size.set_height(-space_);

				for(AbstractView* p = first_subview(); p; p = p->next_view())
				{
					if(p->visiable()) {
						tmp = p->GetPreferredSize();

						preferred_size.add_height(tmp.height() + space_);
						preferred_size.set_width(std::max(preferred_size.width(), tmp.width()));
					}
				}

				preferred_size.add_width(margin_.hsum());
				preferred_size.add_height(margin_.vsum());

			}

		}

		return preferred_size;
	}

	AbstractView* ToolBox::GetFocusedView() const
	{
		return focused_widget_;
	}

	bool ToolBox::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	void ToolBox::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			projection_matrix_  = glm::ortho(
				0.f,
				0.f + (float)request.size()->width(),
				0.f,
				0.f + (float)request.size()->height(),
				100.f, -100.f);

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;
			GenerateVertices(size(), 1.f * AbstractWindow::theme->pixel(), RoundNone, 0.f, &inner_verts, &outer_verts);

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			vbo_.bind(0);
			float* ptr = (float*)vbo_.map();
			*(ptr + 4) = (float)size().width();
			*(ptr + 9) = (float)size().height();
			*(ptr + 12) = (float)size().width();
			*(ptr + 13) = (float)size().height();
			vbo_.unmap();

			vbo_.reset();

			FillSubWidgets();

			RequestRedraw();

		} else if (request.target()->superview() == this) {
			FillSubWidgets();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool ToolBox::PreDraw (AbstractWindow* context)
	{
		if(!visiable()) return false;

		const_cast<AbstractWindow*>(context)->register_active_frame(this);

		if(refresh()) {
			RenderSubFramesToTexture(this, context, projection_matrix_, model_matrix_, &texture_buffer_);
		}

		//texture_buffer_.bind();
		//texture_buffer_.WriteToFile("file.png");
		//texture_buffer_.reset();

		return true;
	}

	Response ToolBox::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders->frame_inner_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_COLOR), 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		AbstractWindow::shaders->frame_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		glBindVertexArray(vao_[1]);

		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COLOR), 0.576f, 0.576f, 0.576f, 1.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 6);

		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COLOR), 0.4f, 0.4f, 0.4f, 1.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        AbstractWindow::shaders->frame_image_program()->use();
        
        texture_buffer_.bind();
        glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_POSITION), position().x(), position().y());
        glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
        glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_GAMMA), 0);
        
        glBindVertexArray(vao_[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Finish;
	}

	void ToolBox::PostDraw (AbstractWindow* context)
	{
	}

	void ToolBox::PerformFocusOn (AbstractWindow* context)
	{
		DBG_PRINT_MSG("%s", "focus in");
	}

	void ToolBox::PerformFocusOff (AbstractWindow* context)
	{
		DBG_PRINT_MSG("%s", "focus out");

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
			hovered_widget_ = 0;
		}
	}

	void ToolBox::PerformHoverIn (AbstractWindow* context)
	{
	}

	void ToolBox::PerformHoverOut (AbstractWindow* context)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_, context);
			hovered_widget_ = 0;
		}
	}

	Response ToolBox::PerformKeyPress (AbstractWindow* context)
	{
		const_cast<AbstractWindow*>(context)->register_active_frame(this);

		Response response = Ignore;

		if(focused_widget_) {
			delegate_key_press_event(focused_widget_, context);
		}

		return response;
	}

	Response ToolBox::PerformMousePress (AbstractWindow* context)
	{
		const_cast<AbstractWindow*>(context)->register_active_frame(this);

		if(cursor_position_ == InsideRectangle) {

			if(hovered_widget_) {

				AbstractView* widget = 0;	// widget may be focused

				widget = DispatchMousePressEvent(hovered_widget_, context);

				if(widget == 0) {
					DBG_PRINT_MSG("%s", "widget 0");
					set_pressed(true);
				} else {
					SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
				}

			} else {
				set_pressed(true);
				// SetFocusedWidget(0);
			}

		} else {
			set_pressed(false);
		}

		return Finish;
	}

	Response ToolBox::PerformMouseRelease (AbstractWindow* context)
	{
		cursor_position_ = InsideRectangle;
		set_pressed(false);

		if(focused_widget_) {
			const_cast<AbstractWindow*>(context)->register_active_frame(this);
			return delegate_mouse_release_event(focused_widget_, context);
		}

		return Ignore;
	}

	Response ToolBox::PerformMouseMove (AbstractWindow* context)
	{
		Response retval = Ignore;

		if(focused_widget_) {
			const_cast<AbstractWindow*>(context)->register_active_frame(this);
			retval = delegate_mouse_move_event(focused_widget_, context);
		}

		return retval;
	}

	Response ToolBox::DispatchHoverEvent (AbstractWindow* context)
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
							&ToolBox::OnHoverWidgetDestroyed);
				}

				hovered_widget_ = new_hovered_widget;
				if(hovered_widget_) {
					events()->connect(hovered_widget_->destroyed(), this,
							&ToolBox::OnHoverWidgetDestroyed);
				}

			}

			return Finish;

		} else {

			cursor_position_ = OutsideRectangle;

			if(hover()) {
				set_hover(false);
				PerformHoverOut(context);
			}

			return Ignore;
		}
	}

	void ToolBox::InitializeToolBoxOnce()
	{
		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;
		GenerateVertices(size(), 1.f * AbstractWindow::theme->pixel(), RoundNone, 0.f, &inner_verts, &outer_verts);

		buffer_.generate();
		glGenVertexArrays(3, vao_);

		glBindVertexArray(vao_[0]);
		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[2]);

		GLfloat vertices[] = {
				// coord											uv
				0.f, 0.f,											0.f, 0.f,
				(float)size().width(), 0.f,							1.f, 0.f,
				0.f, (float)size().height(),						0.f, 1.f,
				(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		vbo_.generate();
		vbo_.bind(0);
		vbo_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				AttributeCoord);
		glEnableVertexAttribArray (
				AttributeUV);
		glVertexAttribPointer (AttributeCoord,
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (AttributeUV, 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vbo_.reset();

		set_refresh(true);
	}

	void ToolBox::FillSubWidgets ()
	{
		if(orientation_ == Horizontal) {
			FillSubWidgetsHorizontally();
		} else {
			FillSubWidgetsVertically();
		}
	}

	void ToolBox::FillSubWidgetsHorizontally()
	{
		int x = margin_.left();
		int y = margin_.bottom();
		//int width = size().width() - margin_.hsum();
		int height = size().height() - margin_.vsum();

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			MoveSubViewTo(p, x, y);
			ResizeSubView(p, p->size().width(), height);
			/*
			if (p->IsExpandY()) {
				ResizeSubWidget(p, p->size().width(), height);
			} else {

				if (p->size().height() > height) {
					ResizeSubWidget(p, p->size().width(), height);
				} else {
					SetSubWidgetPosition(p, x,
							y + (height - p->size().height()) / 2);
				}

			}
			*/

			x += p->size().width() + space_;
		}
	}

	void ToolBox::FillSubWidgetsVertically()
	{
		int x = margin_.left();
		int y = size().height() - margin_.top();
		int width = size().width() - margin_.hsum();
		//int height = size().height() - margin_.vsum();

		y = y + space_;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			y = y - p->size().height() - space_;

			MoveSubViewTo(p, x, y);
			ResizeSubView(p, width, p->size().height());
			/*
			if(p->IsExpandX()) {
				ResizeSubWidget(p, width, p->size().height());
			} else {

				if(p->size().width() > width) {
					ResizeSubWidget(p, width, p->size().height());
				} else {
					SetSubWidgetPosition(p, x + (width - p->size().width()) / 2,
									y);
				}

			}
			*/
		}
	}

	int ToolBox::GetLastPosition () const
	{
		int retval = 0;

		if(orientation_ == Horizontal) {

			retval = margin_.left();
			if (last_subview()) {
				retval = last_subview()->position().x() + last_subview()->size().width() + space_;
			}

		} else {

			retval = size().height() - margin_.top();
			if(last_subview()) {
				retval = last_subview()->position().y() - space_;
			}

		}

		return retval;
	}

	void ToolBox::SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			delegate_focus_off(focused_widget_, context);
			focused_widget_->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			delegate_focus_on(focused_widget_, context);
			events()->connect(focused_widget_->destroyed(), this, &ToolBox::OnFocusedWidgetDestroyed);
		}
	}

	void ToolBox::OnFocusedWidgetDestroyed (AbstractWidget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void ToolBox::OnHoverWidgetDestroyed (AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

}
