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

#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	int Menu::kDefaultIconSpace = 4;
	int Menu::kDefaultShortcutSpace = 20;

	Menu::Menu ()
	: AbstractFloatingFrame(),
	  focused_widget_(nullptr),
	  hovered_widget_(nullptr),
	  cursor_range_(0)
	{
		set_size (240, 360);
		set_round_type(RoundAll);
		set_refresh(true);

		InitializeMenu();

		projection_matrix_  = glm::ortho(
				0.f, (float)size().width(),
				0.f, (float)size().height(),
				100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
	}

	Menu::~Menu ()
	{
		glDeleteVertexArrays(3, vao_);

		if(focused_widget_) {
			delegate_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Menu::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &Menu::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}

	}

	void Menu::SetTitle(const String& title)
	{
		title_ = title;
	}

	void Menu::AddAction(const String& text)
	{
		MenuItem* item = Manage(new MenuItem(text));

		AddMenuItem(item);
	}

	void Menu::AddAction(const String& text, const String& shortcut)
	{
		MenuItem* item = Manage(new MenuItem(text, shortcut));

		AddMenuItem(item);
	}

	void Menu::AddAction(const RefPtr<AbstractIcon>& icon, const String& text)
	{
		MenuItem* item = Manage(new MenuItem(icon, text));

		AddMenuItem(item);
	}

	void Menu::AddAction(const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut)
	{
		MenuItem* item = Manage(new MenuItem(icon, text, shortcut));

		AddMenuItem(item);
	}

	void Menu::AddAction(const RefPtr<Action>& action)
	{
		if(action) {
			MenuItem* item = Manage(new MenuItem(action));
			AddMenuItem(item);
		}
	}

	bool Menu::AddMenuItem(MenuItem* item)
	{
		int x = 0;
		int y = 0 + size().height() - (round_radius() + 0.5f);
		if(subs_count()) {
			y = last_subview()->position().y();
		}

		if(PushBackSubView(item)) {
			y -= item->size().height();

			MoveSubViewTo(item, x, y);
			ResizeSubView(item, size().width(), item->size().height());

			RequestRedraw();
			return true;
		}

		return false;
	}

	bool Menu::InsertMenuItem (int index, MenuItem* item)
	{
		if(InsertMenuItem(index, item)) {
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool Menu::AddButton(Button* btn)
	{
		int x = 0;
		int y = 0 + size().height() - (round_radius() + 0.5f);
		if(subs_count()) {
			y = last_subview()->position().y();
		}

		if(PushBackSubView(btn)) {
			y -= btn->size().height();

			MoveSubViewTo(btn, x, y);
			ResizeSubView(btn, size().width(), btn->size().height());

			RequestRedraw();
			return true;
		}

		return false;
	}

	ResponseType Menu::PerformMouseMove(const Context* context)
	{
		return Finish;
	}

	ResponseType Menu::PerformMousePress (const Context* context)
	{
		const_cast<Context*>(context)->register_active_frame(this);

		if(cursor_range_ == InsideRectangle) {

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
			}

		} else if (cursor_range_ == OutsideRectangle) {
			set_pressed(false);
			delete this;
			return Finish;
		}

		return Finish;
	}

	ResponseType Menu::PerformMouseRelease (const Context* context)
	{
		cursor_range_ = InsideRectangle;
		set_pressed(false);

		if(focused_widget_) {
			const_cast<Context*>(context)->register_active_frame(this);
			return delegate_mouse_release_event(focused_widget_, context);
		}

		return Ignore;
	}

	void Menu::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			set_size(*request.size());

			projection_matrix_  = glm::ortho(
					0.f,
					0.f + size().width(),
					0.f,
					0.f + size().height(),
					100.f, -100.f);

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Context::theme->menu_back().shaded) {
				GenerateRoundedVertices(Vertical,
						Context::theme->menu_back().shadetop,
						Context::theme->menu_back().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			buffer_.bind(0);
			buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			buffer_.reset();

			buffer_.bind(2);
			float* ptr = (float*)buffer_.map();
			*(ptr + 4) = (float)size().width();
			*(ptr + 9) = (float)size().height();
			*(ptr + 12) = (float)size().width();
			*(ptr + 13) = (float)size().height();
			buffer_.unmap();

			//ResetHighlightBuffer(request.size()->width());
			RequestRedraw();

			shadow_->Resize(size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Menu::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->menu_back().shadetop,
					Context::theme->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		shadow_->SetRoundType(round_type);
	}

	void Menu::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->menu_back().shadetop,
					Context::theme->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		shadow_->SetRadius(radius);
	}

	void Menu::ResetHighlightBuffer (int width)
	{
		Size size(width, 24);

		std::vector<GLfloat> inner_verts;

		GenerateVertices(size,
				default_border_width(),
				RoundNone,
				0.f,
				Vertical,
				Context::theme->menu_item().shadetop,
				Context::theme->menu_item().shadedown,
				&inner_verts,
				nullptr
				);

//		highlight_buffer_->bind();
//		highlight_buffer_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
//		highlight_buffer_->reset();
	}
	
	void Menu::PerformFocusOn (const Context* context)
	{
	}

	void Menu::PerformFocusOff (const Context* context)
	{
	}

	bool Menu::PreDraw(const Context* context)
	{
		if(!visiable()) return false;

		const_cast<Context*>(context)->register_active_frame(this);

		if(refresh()) {
			RenderSubFramesToTexture(this, context, projection_matrix_, model_matrix_, &texture_buffer_);
		}

		return true;
	}

	ResponseType Menu::Draw (const Context* context)
	{
		shadow_->Draw(position().x(), position().y());

		Context::shaders->frame_inner_program()->use();

		glUniform2f(Context::shaders->location(Shaders::FRAME_INNER_POSITION), (float) position().x(), (float) position().y());
		glUniform1i(Context::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4fv(Context::shaders->location(Shaders::FRAME_INNER_COLOR), 1, Context::theme->menu_back().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		Context::shaders->frame_outer_program()->use();

		glUniform2f(Context::shaders->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		glUniform4fv(Context::shaders->location(Shaders::FRAME_OUTER_COLOR), 1,
		        Context::theme->menu_back().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		// Draw texture buffer
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
		GLSLProgram::reset();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Finish;
	}

	void Menu::PostDraw(const Context* context)
	{
	}

	void Menu::PerformHoverIn(const Context* context)
	{
	}

	void Menu::PerformHoverOut(const Context* context)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &Menu::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_, context);
			hovered_widget_ = 0;
		}
	}

	ResponseType Menu::PerformKeyPress(const Context* context)
	{
		if(context->key() == Key_Escape) {
			RequestRedraw();
			delete this;
			return Finish;
		}

		return Ignore;
	}

	ResponseType Menu::PerformContextMenuPress(const Context* context)
	{
		return Ignore;
	}

	ResponseType Menu::PerformContextMenuRelease(const Context* context)
	{
		return Ignore;
	}

	ResponseType Menu::DispatchHoverEvent(const Context* context)
	{
		if(pressed_ext()) return Finish;

		if(Contain(context->cursor_position())) {

			cursor_range_ = InsideRectangle;

			if(!hover()) {
				set_hover(true);
				PerformHoverIn(context);
			}

			AbstractWidget* new_hovered_widget = DispatchHoverEventsInSubWidgets(hovered_widget_, context);

			if(new_hovered_widget != hovered_widget_) {

				if(hovered_widget_) {
					hovered_widget_->destroyed().disconnectOne(this,
							&Menu::OnHoverWidgetDestroyed);
				}

				hovered_widget_ = new_hovered_widget;
				if(hovered_widget_) {
					events()->connect(hovered_widget_->destroyed(), this,
							&Menu::OnHoverWidgetDestroyed);
				}

			}

		} else {
			cursor_range_ = OutsideRectangle;
			if(hover()) {
				set_hover(false);
				PerformHoverOut(context);
			}
		}

		return Finish;
	}

	void Menu::InitializeMenu ()
	{
		glGenVertexArrays(3, vao_);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->menu_back().shadetop,
					Context::theme->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.generate();

		glBindVertexArray(vao_[0]);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(Context::shaders->location(Shaders::FRAME_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::FRAME_INNER_COORD), 3,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(Context::shaders->location(Shaders::FRAME_OUTER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::FRAME_OUTER_COORD), 2,	GL_FLOAT, GL_FALSE, 0, 0);

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

		// Now set buffer for hightlight bar
//		Size highlight_size(size().width(), DefaultMenuItemHeight);
//		tool.GenerateVertices(highlight_size,
//				DefaultBorderWidth(),
//				RoundNone,
//				0,
//				Context::theme->menu_item().inner_sel,
//				Vertical,
//				Context::theme->menu_item().shadetop,
//				Context::theme->menu_item().shadedown
//				);
//
//		glBindVertexArray(vao_[2]);
//		m_highlight_buffer.reset(new GLArrayBuffer);
//		m_highlight_buffer->generate();
//		m_highlight_buffer->bind();
//
//		m_highlight_buffer->set_data(tool.inner_size(), tool.inner_data());
//
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
//		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

	}

	void Menu::OnFocusedWidgetDestroyed(AbstractWidget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Menu::OnFocusedWidgetDestroyed);

		focused_widget_ = nullptr;
	}

	void Menu::OnHoverWidgetDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Menu::OnHoverWidgetDestroyed);

		hovered_widget_ = nullptr;
	}

	void Menu::SetFocusedWidget(AbstractWidget* widget, const Context* context)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			delegate_focus_off(focused_widget_, context);
			focused_widget_->destroyed().disconnectOne(this, &Menu::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			delegate_focus_on(focused_widget_, context);
			events()->connect(focused_widget_->destroyed(), this, &Menu::OnFocusedWidgetDestroyed);
		}
	}

}
