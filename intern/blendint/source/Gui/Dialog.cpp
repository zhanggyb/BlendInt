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

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Core/Rect.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/Dialog.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Gui/FlowLayout.hpp>

namespace BlendInt {

	Dialog::Dialog (AbstractLayout* layout, bool modal)
	: AbstractFloatingFrame(),
	  focused_widget_(nullptr),
	  hovered_widget_(nullptr),
	  decoration_(nullptr),
	  layout_(nullptr),
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

		if(layout != nullptr) {
			layout_ = layout;
		} else {
			layout_ = Manage(new FlowLayout);
		}

		PushBackSubView(layout_);
		layout_->Resize(size());

		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
	}

	Dialog::Dialog(const String& title, AbstractLayout* layout, bool modal)
	: AbstractFloatingFrame(),
	  focused_widget_(nullptr),
	  hovered_widget_(nullptr),
	  decoration_(nullptr),
	  layout_(nullptr),
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

		DBG_PRINT_MSG("%s", "create decoration");
		decoration_ = Manage(new Decoration(title));
		decoration_->Resize(size().width(), decoration_->GetPreferredSize().height());
		decoration_->MoveTo(0, size().height() - decoration_->size().height());
		PushBackSubView(decoration_);
		events()->connect(decoration_->close_triggered(), this, &Dialog::OnCloseButtonClicked);

		if(layout != nullptr) {
			layout_ = layout;
		} else {
			layout_ = Manage(new FlowLayout);
		}

		PushBackSubView(layout_);
		layout_->Resize(size().width(), size().height() - decoration_->size().height());

		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
	}

	Dialog::Dialog (AbstractDecoration* decoration, AbstractLayout* layout,
	        bool modal)
	: AbstractFloatingFrame(),
	  focused_widget_(nullptr),
	  hovered_widget_(nullptr),
	  decoration_(nullptr),
	  layout_(nullptr),
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

		if(layout != nullptr) {
			layout_ = layout;
		} else {
			layout_ = Manage(new FlowLayout);
		}

		DBG_PRINT_MSG("%s", "create decoration");
		if(decoration != nullptr) {
			decoration_ = decoration;

			decoration_->Resize(size().width(), decoration_->GetPreferredSize().height());
			decoration_->MoveTo(0, size().height() - decoration_->size().height());

			PushBackSubView(decoration_);
			events()->connect(decoration_->close_triggered(), this, &Dialog::OnCloseButtonClicked);

			PushBackSubView(layout_);
			layout_->Resize(size().width(), size().height() - decoration_->size().height());

		} else {

			PushBackSubView(layout_);
			layout_->Resize(size());

		}

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

	void Dialog::SetDecoration(AbstractDecoration* decoration)
	{
		if((decoration == 0) || (decoration == decoration_)) return;

		if(decoration_) {

			if(decoration_->managed() && (decoration_->reference_count() == 0)) {
				delete decoration_;
			} else {
				DBG_PRINT_MSG("Warning: %s", "Decoration is not set managed, remove this will not delete it");
				AbstractFloatingFrame::RemoveSubView(decoration_);
			}

		}

		if(InsertSubView(0, decoration)) {
			decoration_ = decoration;

			// TODO: check decoration_ size

			ResizeSubView(decoration_, size().width(), decoration_->size().height());

			if(layout_) {

				MoveSubViewTo(decoration_, 0, size().height() - decoration_->size().height());

				ResizeSubView(layout_, size().width(), size().height() - decoration_->size().height());
				MoveSubViewTo(layout_, 0, 0);

			} else {

				MoveSubViewTo(decoration_, 0, size().height() - decoration_->size().height());

			}
		} else {
			DBG_PRINT_MSG("Warning: %s", "Fail to set layout");
		}

		RequestRedraw();
	}

	void Dialog::SetLayout(AbstractLayout* layout)
	{
		if((layout == 0) || (layout == layout_)) return;

		if(layout_) {

			for(AbstractView* p = layout_->first_subview(); p; p = p->next_view()) {
				layout->AddWidget(dynamic_cast<AbstractWidget*>(p));
			}

			if(layout_->managed() && (layout_->reference_count() == 0)) {
				delete layout_;
			} else {
				DBG_PRINT_MSG("Warning: %s", "Layout is not set managed, remove this will not delete it");
				AbstractFloatingFrame::RemoveSubView(layout_);
			}
		}

		// index 1 is for layout
		if(InsertSubView(1, layout)) {
			layout_ = layout;
			MoveSubViewTo(layout_, 0, 0);

			if(decoration_) {
				ResizeSubView(layout_, size().width(), size().height() - decoration_->size().height());
			} else {
				ResizeSubView(layout_, size());
			}
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

			if (Context::theme->dialog().shaded) {
				GenerateRoundedVertices(Vertical,
						Context::theme->dialog().shadetop,
						Context::theme->dialog().shadedown,
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

	bool Dialog::PreDraw(const Context* context)
	{
		if(!visiable()) return false;

		const_cast<Context*>(context)->register_active_frame(this);

		if(refresh()) {
			RenderSubFramesToTexture(this, context, projection_matrix_, model_matrix_, &texture_buffer_);
		}

		return true;
	}

	ResponseType Dialog::Draw(const Context* context)
	{
		shadow_->Draw(position().x(), position().y());

		Context::shaders->frame_inner_program()->use();

		glUniform2f(Context::shaders->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(Context::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4f(Context::shaders->location(Shaders::FRAME_INNER_COLOR), 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        Context::shaders->frame_image_program()->use();

        texture_buffer_.bind();
        glUniform2f(Context::shaders->location(Shaders::FRAME_IMAGE_POSITION), position().x(), position().y());
        glUniform1i(Context::shaders->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
        glUniform1i(Context::shaders->location(Shaders::FRAME_IMAGE_GAMMA), 0);

        glBindVertexArray(vao_[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        texture_buffer_.reset();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Context::shaders->frame_outer_program()->use();

		glUniform2f(Context::shaders->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		//glUniform4fv(Context::shaders->location(Shaders::FRAME_OUTER_COLOR), 1,
		//        Context::theme->dialog().outline.data());
		glUniform4f(Context::shaders->location(Shaders::FRAME_OUTER_COLOR), 0.f, 0.f, 0.f, 1.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		GLSLProgram::reset();

		return Finish;
	}

	void Dialog::PostDraw (const Context* context)
	{
	}

	void Dialog::PerformFocusOn (const Context* context)
	{
	}

	void Dialog::PerformFocusOff (const Context* context)
	{

	}

	void Dialog::PerformHoverIn(const Context* context)
	{
	}

	void Dialog::PerformHoverOut(const Context* context)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &Dialog::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_, context);
			hovered_widget_ = 0;
		}
	}

	ResponseType Dialog::PerformKeyPress(const Context* context)
	{
		ResponseType response = Ignore;

		if(context->key() == Key_Escape) {
			RequestRedraw();
			delete this;
			return Finish;
		}

		if(focused_widget_) {
			const_cast<Context*>(context)->register_active_frame(this);
			response = DispatchKeyEvent(focused_widget_, context);
		}

		return response;
	}

	ResponseType Dialog::PerformContextMenuPress(
			const Context* context)
	{
		return Ignore;
	}

	ResponseType Dialog::PerformContextMenuRelease(
			const Context* context)
	{
		return Ignore;
	}

	ResponseType Dialog::PerformMousePress(const Context* context)
	{
		const_cast<Context*>(context)->register_active_frame(this);

		if(cursor_position_ == InsideRectangle) {

			last_position_ = position();
			cursor_point_ = context->cursor_position();

			if(hovered_widget_) {

				AbstractView* widget = 0;	// widget may be focused

				widget = DispatchMousePressEvent(hovered_widget_, context);

				if(widget == 0) {
					//DBG_PRINT_MSG("%s", "widget 0");
					set_mouse_button_pressed(true);
				} else if (widget == layout_) {
					DBG_PRINT_MSG("%s", "hovered is layout");
					set_mouse_button_pressed(true);
				} else {
					SetFocusedWidget(dynamic_cast<AbstractWidget*>(widget), context);
				}

			} else {
				set_mouse_button_pressed(true);
			}

			if(!modal()) {
				const_cast<Context*>(context)->MoveFrameToTop(this);
			}

			return Finish;

		} else if (cursor_position_ != OutsideRectangle) {

			set_mouse_button_pressed(true);

			last_position_ = position();
			last_size_ = size();
			cursor_point_ = context->cursor_position();

			return Finish;
		}

		if(modal()) {
			return Finish;
		}

		return Ignore;
	}

	ResponseType Dialog::PerformMouseRelease(const Context* context)
	{
		cursor_position_ = InsideRectangle;
		set_mouse_button_pressed(false);

		if(focused_widget_) {
			const_cast<Context*>(context)->register_active_frame(this);
			return delegate_mouse_release_event(focused_widget_, context);
		}

		return Ignore;
	}

	ResponseType Dialog::PerformMouseMove(const Context* context)
	{
		ResponseType retval = Ignore;

		if(mouse_button_pressed()) {

			int ox = context->cursor_position().x() - cursor_point_.x();
			int oy = context->cursor_position().y() - cursor_point_.y();

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

				const_cast<Context*>(context)->register_active_frame(this);
				retval = delegate_mouse_move_event(focused_widget_, context);

			}
		}

		return retval;
	}

	ResponseType Dialog::DispatchHoverEvent(const Context* context)
	{
		if(mouse_button_pressed()) return Finish;

		ResponseType retval = Finish;
		int border = 4;

		Rect valid_rect(position().x() - border, position().y() - border,
			size().width() + 2 * border, size().height() + 2 * border);

		if(valid_rect.contains(context->cursor_position())) {

			if(Contain(context->cursor_position())) {

				cursor_position_ = InsideRectangle;

				// DBG_PRINT_MSG("Cursor position: (%d, %d)", context->cursor_position().x(), context->cursor_position().y());

				AbstractWidget* new_hovered_widget = DispatchHoverEventsInSubWidgets(hovered_widget_, context);

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

//				if(hovered_widget_) {
//					 DBG_PRINT_MSG("hovered widget: %s", hovered_widget_->name().c_str());
//				}

				// set cursor shape
				if(cursor_on_border()) {
					set_cursor_on_border(false);
					Context::cursor->PopCursor();
				}

			} else {

				set_cursor_on_border(true);
				cursor_position_ = InsideRectangle;

				if(context->cursor_position().x() <= position().x()) {
					cursor_position_ |= OnLeftBorder;
				} else if (context->cursor_position().x() >= (position().x() + size().width())) {
					cursor_position_ |= OnRightBorder;
				}

				if (context->cursor_position().y() >= (position().y() + size().height())) {
					cursor_position_ |= OnTopBorder;
				} else if (context->cursor_position().y () <= position().y()) {
					cursor_position_ |= OnBottomBorder;
				}

				// set cursor shape
				switch(cursor_position_) {

					case OnLeftBorder:
					case OnRightBorder: {
						Context::cursor->PushCursor();
						Context::cursor->SetCursor(SplitHCursor);

						break;
					}

					case OnTopBorder:
					case OnBottomBorder: {
						Context::cursor->PushCursor();
						Context::cursor->SetCursor(SplitVCursor);
						break;
					}

					case OnTopLeftCorner:
					case OnBottomRightCorner: {
						Context::cursor->PushCursor();
						Context::cursor->SetCursor(SizeFDiagCursor);
						break;
					}

					case OnTopRightCorner:
					case OnBottomLeftCorner: {
						Context::cursor->PushCursor();
						Context::cursor->SetCursor(SizeBDiagCursor);
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
				Context::cursor->PopCursor();
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
		int h = 0;
		if(decoration_) {
			decoration_->Resize(size().width(), decoration_->size().height());
			decoration_->MoveTo(0, size().height() - decoration_->size().height());
			h = decoration_->size().height();
		}

		layout_->MoveTo(0, 0);
		layout_->Resize(size().width(), size().height() - h);
	}

	void Dialog::SetFocusedWidget(AbstractWidget* widget, const Context* context)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			delegate_focus_off(focused_widget_, context);
			focused_widget_->destroyed().disconnectOne(this, &Dialog::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			delegate_focus_on(focused_widget_, context);
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

	void Dialog::InitializeDialogOnce()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->dialog().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->dialog().shadetop,
					Context::theme->dialog().shadedown,
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

		glEnableVertexAttribArray(Context::shaders->location(Shaders::FRAME_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::FRAME_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(Context::shaders->location(Shaders::FRAME_OUTER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::FRAME_OUTER_COORD),
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
				Context::shaders->location (Shaders::FRAME_IMAGE_COORD));
		glEnableVertexAttribArray (
				Context::shaders->location (Shaders::FRAME_IMAGE_UV));
		glVertexAttribPointer (Context::shaders->location (Shaders::FRAME_IMAGE_COORD),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (Context::shaders->location (Shaders::FRAME_IMAGE_UV), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		buffer_.reset();
	}

	bool Dialog::RemoveSubView (AbstractView* view)
	{
		if(view == layout_) {
			layout_ = nullptr;
		} else if(view == decoration_) {
			decoration_ = nullptr;
		}

		return AbstractFloatingFrame::RemoveSubView(view);
	}

	void Dialog::OnCloseButtonClicked()
	{
		//assert(button == decoration_->close_button());
		delete this;
	}

}
