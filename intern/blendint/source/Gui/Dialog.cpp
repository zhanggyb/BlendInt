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

#include <BlendInt/Core/Rect.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Cursor.hpp>

#include <BlendInt/Gui/Dialog.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Gui/FreeLayout.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Dialog::Dialog(const String& title, bool modal)
	: AbstractFloatingFrame(),
	  focused_widget_(0),
	  hovered_widget_(0),
	  decoration_(0),
	  layout_(0),
	  cursor_position_(InsideRectangle),
	  dialog_flags_(0)
	{
		set_size(400, 300);
		set_round_type(RoundAll);
		set_round_radius(5.f);
		set_refresh(true);
		set_modal(modal);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		applied_.reset(new Cpp::Event<Dialog*>);
		canceled_.reset(new Cpp::Event<Dialog*>);

		InitializeDialogOnce();

		decoration_ = Manage(new Decoration(title));
		decoration_->Resize(size().width(), decoration_->GetPreferredSize().height());
		decoration_->MoveTo(0, size().height() - decoration_->size().height());
		PushBackSubView(decoration_);
		events()->connect(decoration_->close_button_clicked(), this, &Dialog::OnCloseButtonClicked);

		// create default layout
		layout_ = Manage(new FreeLayout);
		PushBackSubView(layout_);
		layout_->Resize(size().width(), size().height() - decoration_->size().height());
		events()->connect(layout_->destroyed(), this, &Dialog::OnLayoutDestroyed);

		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
	}

	Dialog::~Dialog()
	{
		glDeleteVertexArrays(3, vao_);

		if(focused_widget_) {
			delegate_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Dialog::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &Dialog::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	void Dialog::SetLayout(AbstractLayout* layout)
	{
		if((layout == 0) || (layout == layout_)) return;

		if(layout_) {
			layout_->destroyed().disconnectOne(this, &Dialog::OnLayoutDestroyed);

			for(AbstractView* p = layout_->first_subview(); p; p = p->next_view()) {
				layout->AddWidget(dynamic_cast<AbstractWidget*>(p));
			}

			if(layout_->managed()) {
				delete layout_;
			} else {
				DBG_PRINT_MSG("Warning: %s", "Layout is not set managed, remove this will not delete it");
				RemoveSubView(layout_);
			}
		}

		// index 1 is for layout
		if(InsertSubView(1, layout)) {
			layout_ = layout;
			events()->connect(layout_->destroyed(), this, &Dialog::OnLayoutDestroyed);
			MoveSubViewTo(layout_, 0, 0);
			ResizeSubView(layout_, size().width(), size().height() - decoration_->size().height());
		} else {
			DBG_PRINT_MSG("Warning: %s", "Fail to set layout");
		}

		RequestRedraw();
	}

	void Dialog::AddWidget(AbstractWidget* widget)
	{
		assert(layout_ != nullptr);

		if(layout_->AddWidget(widget)) {
			RequestRedraw();
		}
	}

	void Dialog::InsertWidget(int index, AbstractWidget* widget)
	{
		assert(layout_ != nullptr);

		if(layout_->InsertWidget(index, widget)) {
			RequestRedraw();
		}
	}

	void Dialog::SetModal(bool modal)
	{
		set_modal(modal);
	}

	AbstractView* Dialog::GetFocusedView () const
	{
		return focused_widget_;
	}

	bool Dialog::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		return true;
	}

	bool Dialog::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		return true;
	}

	void Dialog::PerformSizeUpdate(const SizeUpdateRequest& request)
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

			if (Theme::instance->dialog().shaded) {
				GenerateRoundedVertices(Vertical,
						Theme::instance->dialog().shadetop,
						Theme::instance->dialog().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			buffer_.bind(0);
			buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			buffer_.bind(2);
			float* ptr = (float*)buffer_.map();
			*(ptr + 4) = (float)size().width();
			*(ptr + 9) = (float)size().height();
			*(ptr + 12) = (float)size().width();
			*(ptr + 13) = (float)size().height();
			buffer_.unmap();

			buffer_.reset();

			shadow_->Resize(size());

			UpdateLayout();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool Dialog::PreDraw(Profile& profile)
	{
		if(!visiable()) return false;

		assign_profile_frame(profile, this);

		if(refresh()) {
			RenderToBuffer(profile);
		}

		return true;
	}

	ResponseType Dialog::Draw(Profile& profile)
	{
		shadow_->Draw(position().x(), position().y());

		Shaders::instance->frame_inner_program()->use();

		glUniform2f(Shaders::instance->location(Stock::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(Shaders::instance->location(Stock::FRAME_INNER_GAMMA), 0);
		glUniform4fv(Shaders::instance->location(Stock::FRAME_INNER_COLOR), 1, Theme::instance->dialog().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        Shaders::instance->frame_image_program()->use();

        texture_buffer_.bind();
        glUniform2f(Shaders::instance->location(Stock::FRAME_IMAGE_POSITION), position().x(), position().y());
        glUniform1i(Shaders::instance->location(Stock::FRAME_IMAGE_TEXTURE), 0);
        glUniform1i(Shaders::instance->location(Stock::FRAME_IMAGE_GAMMA), 0);

        glBindVertexArray(vao_[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        texture_buffer_.reset();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Shaders::instance->frame_outer_program()->use();

		glUniform2f(Shaders::instance->location(Stock::FRAME_OUTER_POSITION), position().x(), position().y());
		//glUniform4fv(Shaders::instance->location(Stock::FRAME_OUTER_COLOR), 1,
		//        Theme::instance->dialog().outline.data());
		glUniform4f(Shaders::instance->location(Stock::FRAME_OUTER_COLOR), 0.f, 0.f, 0.f, 1.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		GLSLProgram::reset();

		return Finish;
	}

	void Dialog::PostDraw(Profile& profile)
	{
	}

	void Dialog::FocusEvent(bool focus)
	{
	}

	void Dialog::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void Dialog::MouseHoverOutEvent(const MouseEvent& event)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &Dialog::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_, event);
			hovered_widget_ = 0;
		}
	}

	ResponseType Dialog::KeyPressEvent(const KeyEvent& event)
	{
		ResponseType response = Ignore;

		if(event.key() == Key_Escape) {
			RequestRedraw();
			delete this;
			return Finish;
		}

		if(focused_widget_) {
			assign_event_frame(event, this);
			response = DispatchKeyEvent(focused_widget_, event);
		}

		return response;
	}

	ResponseType Dialog::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Dialog::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Dialog::MousePressEvent(const MouseEvent& event)
	{
		assign_event_frame(event, this);

		if(cursor_position_ == InsideRectangle) {

			last_position_ = position();
			cursor_point_ = event.position();

			if(hovered_widget_) {

				AbstractView* widget = 0;	// widget may be focused

				widget = DispatchMousePressEvent(hovered_widget_, event);

				if(widget == 0) {
					DBG_PRINT_MSG("%s", "widget 0");
					set_mouse_button_pressed(true);
				} else if (widget == layout_) {
					DBG_PRINT_MSG("%s", "hovered is layout");
					set_mouse_button_pressed(true);
				} else {
					SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget));
				}

			} else {
				set_mouse_button_pressed(true);
			}

			if(!modal()) {
				MoveToLast();
			}

			return Finish;

		} else if (cursor_position_ != OutsideRectangle) {

			set_mouse_button_pressed(true);

			last_position_ = position();
			last_size_ = size();
			cursor_point_ = event.position();

			return Finish;
		}

		if(modal()) {
			return Finish;
		}

		return Ignore;
	}

	ResponseType Dialog::MouseReleaseEvent(const MouseEvent& event)
	{
		cursor_position_ = InsideRectangle;
		set_mouse_button_pressed(false);

		if(focused_widget_) {
			assign_event_frame(event, this);
			return delegate_mouse_release_event(focused_widget_, event);
		}

		return Ignore;
	}

	ResponseType Dialog::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		if(mouse_button_pressed()) {

			int ox = event.position().x() - cursor_point_.x();
			int oy = event.position().y() - cursor_point_.y();

			switch(cursor_position_) {

				case InsideRectangle: {
					set_position(last_position_.x() + ox, last_position_.y() + oy);
					break;
				}

				case OnLeftBorder: {
					set_position(last_position_.x() + ox, last_position_.y());
					Resize(last_size_.width() - ox, last_size_.height());
					break;
				}

				case OnRightBorder: {
					Resize(last_size_.width() + ox, last_size_.height());
					break;
				}

				case OnTopBorder: {
					Resize(last_size_.width(), last_size_.height() + oy);
					break;
				}

				case OnBottomBorder: {
					set_position(last_position_.x(), last_position_.y() + oy);
					Resize(last_size_.width(), last_size_.height() - oy);
					break;
				}

				case OnTopLeftCorner: {
					set_position(last_position_.x() + ox, last_position_.y());
					Resize(last_size_.width() - ox, last_size_.height() + oy);
					break;
				}

				case OnTopRightCorner: {
					Resize(last_size_.width() + ox, last_size_.height() + oy);
					break;
				}

				case OnBottomLeftCorner: {
					set_position(last_position_.x() + ox, last_position_.y() + oy);
					Resize(last_size_.width() - ox, last_size_.height() - oy);
					break;
				}

				case OnBottomRightCorner: {
					set_position(last_position_.x(), last_position_.y() + oy);
					Resize(last_size_.width() + ox, last_size_.height() - oy);
					break;
				}

				default: {
					return Finish;
					break;
				}

			}

			if(superview()) {
				superview()->RequestRedraw();
			}
			retval = Finish;

		} else {

			if(focused_widget_) {

				assign_event_frame(event, this);
				retval = delegate_mouse_move_event(focused_widget_, event);

			}
		}

		return retval;
	}

	ResponseType Dialog::DispatchHoverEvent(const MouseEvent& event)
	{
		if(mouse_button_pressed()) return Finish;

		ResponseType retval = Finish;
		int border = 4;

		Rect valid_rect(position().x() - border, position().y() - border,
			size().width() + 2 * border, size().height() + 2 * border);

		if(valid_rect.contains(event.position())) {

			if(Contain(event.position())) {

				cursor_position_ = InsideRectangle;

				AbstractWidget* new_hovered_widget = DispatchHoverEventsInSubWidgets(hovered_widget_, event);

				if(new_hovered_widget != hovered_widget_) {

					if(hovered_widget_) {
						hovered_widget_->destroyed().disconnectOne(this,
								&Dialog::OnHoverWidgetDestroyed);
					}

					hovered_widget_ = new_hovered_widget;
					if(hovered_widget_) {
						events()->connect(hovered_widget_->destroyed(), this,
								&Dialog::OnHoverWidgetDestroyed);
					}

				}

				// set cursor shape
				if(cursor_on_border()) {
					set_cursor_on_border(false);
					Cursor::instance->PopCursor();
				}

			} else {

				set_cursor_on_border(true);
				cursor_position_ = InsideRectangle;

				if(event.position().x() <= position().x()) {
					cursor_position_ |= OnLeftBorder;
				} else if (event.position().x() >= (position().x() + size().width())) {
					cursor_position_ |= OnRightBorder;
				}

				if (event.position().y() >= (position().y() + size().height())) {
					cursor_position_ |= OnTopBorder;
				} else if (event.position().y () <= position().y()) {
					cursor_position_ |= OnBottomBorder;
				}

				// set cursor shape
				switch(cursor_position_) {

					case OnLeftBorder:
					case OnRightBorder: {
						Cursor::instance->PushCursor();
						Cursor::instance->SetCursor(SplitHCursor);

						break;
					}

					case OnTopBorder:
					case OnBottomBorder: {
						Cursor::instance->PushCursor();
						Cursor::instance->SetCursor(SplitVCursor);
						break;
					}

					case OnTopLeftCorner:
					case OnBottomRightCorner: {
						Cursor::instance->PushCursor();
						Cursor::instance->SetCursor(SizeFDiagCursor);
						break;
					}

					case OnTopRightCorner:
					case OnBottomLeftCorner: {
						Cursor::instance->PushCursor();
						Cursor::instance->SetCursor(SizeBDiagCursor);
						break;
					}

					default:
						break;
				}

			}

		} else {
			cursor_position_ = OutsideRectangle;

			// set cursor shape
			if(cursor_on_border()) {
				set_cursor_on_border(false);
				Cursor::instance->PopCursor();
			}

			retval = Ignore;
		}

		// a modal dialog always return Finish
		if(modal()) {
			return Finish;
		} else {
			return retval;
		}
	}

	void Dialog::UpdateLayout()
	{
		decoration_->Resize(size().width(), decoration_->size().height());
		decoration_->MoveTo(0, size().height() - decoration_->size().height());

		layout_->MoveTo(0, 0);
		layout_->Resize(size().width(), size().height() - decoration_->size().height());
	}

	void Dialog::SetFocusedWidget(AbstractWidget* widget)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			delegate_focus_event(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Dialog::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			delegate_focus_event(focused_widget_, true);
			events()->connect(focused_widget_->destroyed(), this, &Dialog::OnFocusedWidgetDestroyed);
		}
	}

	void Dialog::OnFocusedWidgetDestroyed(AbstractWidget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Dialog::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void Dialog::OnHoverWidgetDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Dialog::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

	void Dialog::OnLayoutDestroyed(AbstractWidget* layout)
	{
		assert(layout == layout_);

		DBG_PRINT_MSG("layout %s is destroyed", layout->name().c_str());
		layout->destroyed().disconnectOne(this, &Dialog::OnLayoutDestroyed);
		layout_ = 0;
	}

	void Dialog::InitializeDialogOnce()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->dialog().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->dialog().shadetop,
					Theme::instance->dialog().shadedown,
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

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_OUTER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_OUTER_COORD),
				2, GL_FLOAT, GL_FALSE, 0, 0);

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
				Shaders::instance->location (Stock::FRAME_IMAGE_COORD));
		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::FRAME_IMAGE_UV));
		glVertexAttribPointer (Shaders::instance->location (Stock::FRAME_IMAGE_COORD),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (Shaders::instance->location (Stock::FRAME_IMAGE_UV), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		buffer_.reset();
	}

	void Dialog::OnCloseButtonClicked(AbstractButton* button)
	{
		assert(button == decoration_->close_button());

		delete this;
	}

	void Dialog::RenderToBuffer(Profile& profile)
	{
        // Create and set texture to render to.
        GLTexture2D* tex = &texture_buffer_;
        if(!tex->id())
            tex->generate();

        tex->bind();
        tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
        tex->SetMinFilter(GL_NEAREST);
        tex->SetMagFilter(GL_NEAREST);
        tex->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
        GLFramebuffer* fb = new GLFramebuffer;
        fb->generate();
        fb->bind();

        // Set "renderedTexture" as our colour attachement #0
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, tex->id(), 0);
        //fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

        // Critical: Create a Depth_STENCIL renderbuffer for this off-screen rendering
        GLuint rb;
        glGenRenderbuffers(1, &rb);

        glBindRenderbuffer(GL_RENDERBUFFER, rb);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
                              size().width(), size().height());
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, rb);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, rb);

        if(GLFramebuffer::CheckStatus()) {

            fb->bind();

            Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
            Shaders::instance->SetWidgetModelMatrix(model_matrix_);

            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClearDepth(1.0);
            glClearStencil(0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);

            glViewport(0, 0, size().width(), size().height());

            // Draw context:
            DrawSubViewsOnce(profile);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glViewport(0, 0, profile.context()->size().width(), profile.context()->size().height());

        }

        fb->reset();
        tex->reset();

        //delete tex; tex = 0;

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &rb);

        fb->reset();
        delete fb; fb = 0;
	}

}
