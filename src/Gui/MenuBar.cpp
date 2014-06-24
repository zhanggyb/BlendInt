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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	MenuBar::MenuBar ()
	: AbstractDequeContainer(), m_vao(0), m_space(2), m_active_button(0)
	{
		set_margin(2, 2, 1, 1);
		set_size(200, 22);

		InitializeMenuBar();
	}

	MenuBar::~MenuBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void MenuBar::UpdateContainer(const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerRefresh: {
				ReportUpdateRequest(request);
				break;
			}

			default:
				break;
		}
	}

	void MenuBar::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					VertexTool tool;
					tool.Setup(*size_p, 0, RoundNone, 0);
					m_buffer->Bind();
					tool.SetInnerBufferData(m_buffer.get());
					break;
				}

				case WidgetPosition: {

					const Point* pos_p = static_cast<const Point*>(request.data());
					MoveSubWidgets(pos_p->x() - position().x(),
									pos_p->y() - position().y());

					break;
				}

				default:
					break;
			}

		}
	}

	ResponseType MenuBar::Draw (const RedrawEvent& event)
	{
		using namespace BlendInt::Stock;

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.f);
		program->SetUniform1i("AA", 0);
		program->SetUniform1i("Gamma", 0);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);
		glBindVertexArray(0);

		program->Reset();

		return Accept;
	}

	ResponseType MenuBar::CursorEnterEvent (bool entered)
	{
		return IgnoreAndContinue;
	}

	ResponseType MenuBar::KeyPressEvent (const KeyEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType MenuBar::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType MenuBar::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType MenuBar::MousePressEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType MenuBar::MouseReleaseEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType MenuBar::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	void MenuBar::AddMenu (const RefPtr<Menu>& menu)
	{
		MenuButton* button = Manage (new MenuButton(menu->title()));

#ifdef DEBUG
		button->set_name(ConvertFromString(menu->title()));
#endif
		button->SetMenu(menu);

		int x = GetLastPosition();

		int h = size().height() - margin().top() - margin().bottom();
		h = std::max(h, button->size().height());
		int w = -m_space;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			w += (*it)->size().width() + m_space;
		}
		w += margin().left() + margin().right() + button->size().width();

		PushBackSubWidget(button);
		SetSubWidgetPosition(button, x, position().y() + margin().bottom());

		h = h + margin().top() + margin().bottom();
		Resize(w, h);
		//SetPreferredSize(w, h);

		events()->connect(button->clicked(), this, &MenuBar::OnMenuButtonClicked);
	}

	void MenuBar::AddMenu (const String& text, const RefPtr<Menu>& menu)
	{
		MenuButton* button = Manage (new MenuButton(text));
#ifdef DEBUG
		button->set_name(ConvertFromString(text));
#endif
		button->SetMenu(menu);

		int x = GetLastPosition();

		int h = size().height() - margin().top() - margin().bottom();
		h = std::max(h, button->size().height());
		int w = -m_space;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			w += (*it)->size().width() + m_space;
		}
		w += margin().left() + margin().right() + button->size().width();

		PushBackSubWidget(button);
		SetSubWidgetPosition(button, x, position().y() + margin().bottom());

		h = h + margin().top() + margin().bottom();
		Resize(w, h);
		//SetPreferredSize(w, h);

		events()->connect(button->clicked(), this, &MenuBar::OnMenuButtonClicked);
	}

	void MenuBar::AddMenuButton (MenuButton* button)
	{
		if(!button) return;

		int x = GetLastPosition();

		if(PushBackSubWidget(button)) {

			int h = size().height() - margin().top() - margin().bottom();
			h = std::max(h, button->size().height());
			int w = -m_space;
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				w += (*it)->size().width() + m_space;
			}
			w += margin().left() + margin().right();

			SetSubWidgetPosition(button, x, position().y() + margin().bottom());

			h = h + margin().top() + margin().bottom();
			Resize(w, h);
			//SetPreferredSize(w, h);

			events()->connect(button->clicked(), this, &MenuBar::OnMenuButtonClicked);
		}
	}

	void MenuBar::AddMenuButton (MenuButton* button, const RefPtr<Menu>& menu)
	{
		if(!button) return;

		int x = GetLastPosition();

		if(PushBackSubWidget(button)) {
			button->SetMenu(menu);

			int h = size().height() - margin().top() - margin().bottom();
			h = std::max(h, button->size().height());
			int w = -m_space;
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				w += (*it)->size().width() + m_space;
			}
			w += margin().left() + margin().right();

			SetSubWidgetPosition(button, x, position().y() + margin().bottom());

			h = h + margin().top() + margin().bottom();
			Resize(w, h);

			events()->connect(button->clicked(), this, &MenuBar::OnMenuButtonClicked);
		}
	}

	void MenuBar::SetMenu (size_t index, const RefPtr<Menu>& menu)
	{
		MenuButton* button = GetMenuButton(index);

		if(button) {
			button->SetMenu(menu);
		}
	}

	void MenuBar::SetMenu (MenuButton* button, const RefPtr<Menu>& menu)
	{
		if(!button || button->container() != this) return;

		button->SetMenu(menu);
	}

	Size MenuBar::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {

			Font font;	// Get default font height
			preferred_size.set_width(200);

			int max_font_height = font.GetHeight();

			preferred_size.set_height(
			        max_font_height
			                + AbstractButton::DefaultButtonPadding().top()
			                + AbstractButton::DefaultButtonPadding().bottom());	// top padding: 2, bottom padding: 2

			preferred_size.add_height(margin().top() + margin().bottom());

		} else {

			AbstractWidget* widget = 0;
			Size tmp_size;

			preferred_size.set_width(-m_space);
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				widget = *it;

				if(widget->visiable()) {
					tmp_size = widget->GetPreferredSize();

					preferred_size.add_width(tmp_size.width() + m_space);
					preferred_size.set_height(std::max(preferred_size.height(), tmp_size.height()));
				}
			}

			preferred_size.add_width(margin().left() + margin().right());
			preferred_size.add_height(margin().top() + margin().bottom());
		}

		return preferred_size;
	}

	bool MenuBar::IsExpandX() const
	{
		return true;
	}

	MenuButton* MenuBar::GetMenuButton (size_t index)
	{
		MenuButton* button = 0;

		if(index < sub_widget_size()) {
			button = dynamic_cast<MenuButton*>(sub_widgets()->at(index));
		}

		return button;
	}

	void MenuBar::InitializeMenuBar()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		glBindVertexArray(m_vao);

		m_buffer.reset(new GLArrayBuffer);
		m_buffer->Generate();
		m_buffer->Bind();

		tool.SetInnerBufferData(m_buffer.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}
	
	void MenuBar::OnMenuButtonClicked ()
	{
		MenuButton* original_active = m_active_button;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			MenuButton* menubutton = dynamic_cast<MenuButton*>(*it);
			if(menubutton) {
				if(menubutton->focused()) {
					m_active_button = menubutton;
				}
			}
		}

		if(original_active) {	// If menu shows in context
			RefPtr<Menu> menu = original_active->menu();
			Context* context = GetContext();
			context->Remove(menu.get());
			original_active->SetRoundCornerType(RoundAll);

			menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);
			//menu->property_changed().disconnectAll();
		}

		if(m_active_button) {
			Context* context = GetContext();

			int max_layer = context->GetMaxLayer();
			RefPtr<Menu> menu = m_active_button->menu();
			menu->SetLayer(max_layer + 1);	// show menu in the top layer

			int y = m_active_button->position().y();
			y = y - menu->size().height();

			if(y < 0) {
				y = 0;
			}

			menu->SetPosition(m_active_button->position().x(), y);
			context->Add(menu.get());
			m_active_button->SetRoundCornerType(RoundTopLeft | RoundTopRight);
			context->SetFocusedWidget(menu.get());

			events()->connect(menu->triggered(), this, &MenuBar::OnMenuItemTriggered);
			//events()->connect(menu->property_changed(), this, &MenuBar::OnMenuHide);
		}
	}

	void MenuBar::OnMenuItemTriggered(Action* item)
	{
		DBG_PRINT_MSG("menu item clicked: %s", ConvertFromString(item->text()).c_str());

		if(m_active_button) {
			if(RefPtr<Menu> menu = m_active_button->menu()) {
				//menu->property_changed().disconnectOne(this, &MenuBar::OnMenuHide);
				menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);

				Context* context = GetContext();
				context->Remove(menu.get());
				m_active_button->SetRoundCornerType(RoundAll);
			}

			m_active_button = 0;
		}
	}

	void MenuBar::OnMenuHide(AbstractWidget* widget, int type)
	{
		DBG_PRINT_MSG("%s", "hello");
		Menu* menu = dynamic_cast<Menu*>(widget);
		if(menu) {
			//menu->property_changed().disconnectOne(this, &MenuBar::OnMenuHide);
			menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);

			DBG_PRINT_MSG("menu at layer: %d", menu->z());

			if(type == WidgetVisibility) {
				Context* context = GetContext();
				context->Remove(menu);
			}
		}

		if(m_active_button) {
			m_active_button->SetRoundCornerType(RoundAll);
			m_active_button = 0;
		}

	}

	int MenuBar::GetLastPosition ()
	{
		int pos = position().x() + margin().left();

		if(sub_widgets()->size()) {
			pos -= m_space;

			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				pos += m_space;
				pos = pos + (*it)->size().width();
			}
		}
		return pos;
	}
}
