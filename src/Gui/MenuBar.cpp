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

#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	MenuBar::MenuBar ()
	: AbstractDequeContainer(), m_vao(0), m_space(2), m_active_button(0)
	{
		set_expand_x(true);
		set_expand_y(false);

		set_margin(2, 2, 1, 1);
		set_size(200, 22);
		set_preferred_size(200, 22);

		InitOnce();
	}

	MenuBar::~MenuBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void MenuBar::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

			case FormSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				glBindVertexArray(m_vao);

				std::vector<GLfloat> vertices(12);

				GenerateFlatRectVertices(*size_p, 0.f, &vertices);

				m_buffer->Bind();
				m_buffer->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				m_buffer->Reset();

				glBindVertexArray(0);

				break;
			}

			case FormPosition: {

				const Point* pos_p = static_cast<const Point*>(request.data());
				MoveSubWidgets(pos_p->x() - position().x(), pos_p->y() - position().y());

				break;
			}

			case WidgetRefresh: {
				Refresh();
			}

			default:
				break;
			}
		}
	}

	ResponseType MenuBar::Draw (const RedrawEvent& event)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		glm::vec4 color;
		color.r = themes()->regular.inner.r() / 255.f;
		color.g = themes()->regular.inner.g() / 255.f;
		color.b = themes()->regular.inner.b() / 255.f;
		color.a = themes()->regular.inner.a() / 255.f;

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
		program->SetUniform1i("AA", 0);

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_buffer.get());

		glDisableVertexAttribArray(0);

		program->Reset();

		m_buffer->Reset();

		glBindVertexArray(0);

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

		AppendSubWidget(button);
		SetSubWidgetPosition(button, x, position().y() + margin().bottom());

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

		AppendSubWidget(button);
		SetSubWidgetPosition(button, x, position().y() + margin().bottom());

		events()->connect(button->clicked(), this, &MenuBar::OnMenuButtonClicked);
	}

	void MenuBar::AddMenuButton (MenuButton* button)
	{
		if(!button) return;

		int x = GetLastPosition();

		if(AppendSubWidget(button)) {
			SetSubWidgetPosition(button, x, position().y() + margin().bottom());
			events()->connect(button->clicked(), this, &MenuBar::OnMenuButtonClicked);
		}
	}

	void MenuBar::AddMenuButton (MenuButton* button, const RefPtr<Menu>& menu)
	{
		if(!button) return;

		int x = GetLastPosition();

		if(AppendSubWidget(button)) {
			button->SetMenu(menu);
			SetSubWidgetPosition(button, x, position().y() + margin().bottom());
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

	MenuButton* MenuBar::GetMenuButton (size_t index)
	{
		MenuButton* button = 0;

		if(index < sub_widget_size()) {
			button = dynamic_cast<MenuButton*>(sub_widgets()->at(index));
		}

		return button;
	}

	void MenuBar::InitOnce()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		std::vector<GLfloat> vertices(12);

		GenerateFlatRectVertices(size(), 0.f, &vertices);

		m_buffer.reset(new GLArrayBuffer);
		m_buffer->Generate();

		m_buffer->Bind();
		m_buffer->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_buffer->Reset();

		glBindVertexArray(0);
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
			original_active->SetRoundType(RoundAll);

			menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);
			menu->property_changed().disconnectAll();
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
			m_active_button->SetRoundType(RoundTopLeft | RoundTopRight);
			context->SetFocusedWidget(menu.get());

			events()->connect(menu->triggered(), this, &MenuBar::OnMenuItemTriggered);
			events()->connect(menu->property_changed(), this, &MenuBar::OnMenuHide);
		}
	}

	void MenuBar::OnMenuItemTriggered(ActionItem* item)
	{
		DBG_PRINT_MSG("menu item clicked: %s", ConvertFromString(item->text()).c_str());

		if(m_active_button) {
			if(RefPtr<Menu> menu = m_active_button->menu()) {
				menu->property_changed().disconnectOne(this, &MenuBar::OnMenuHide);
				menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);

				Context* context = GetContext();
				context->Remove(menu.get());
				m_active_button->SetRoundType(RoundAll);
			}

			m_active_button = 0;
		}
	}

	void MenuBar::OnMenuHide(AbstractWidget* widget, int type)
	{
		DBG_PRINT_MSG("%s", "hello");
		Menu* menu = dynamic_cast<Menu*>(widget);
		if(menu) {
			menu->property_changed().disconnectOne(this, &MenuBar::OnMenuHide);
			menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);

			DBG_PRINT_MSG("menu at layer: %d", menu->z());

			if(type == WidgetVisibility) {
				Context* context = GetContext();
				context->Remove(menu);
			}
		}

		if(m_active_button) {
			m_active_button->SetRoundType(RoundAll);
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

