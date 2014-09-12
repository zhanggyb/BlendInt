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
	: AbstractContainer(), m_vao(0), m_space(2), m_active_button(0)
	{
		set_margin(2, 2, 2, 2);
		set_size(200, 22);
		InitializeMenuBar();
	}

	MenuBar::~MenuBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	MenuButton* MenuBar::Append (const RefPtr<Menu>& menu)
	{
		MenuButton* button = Manage (new MenuButton(menu->title()));
		DBG_SET_NAME(button, ConvertFromString(menu->title()));
		button->SetMenu(menu);

		return Append(button);
	}

	MenuButton* MenuBar::Append (const String& text, const RefPtr<Menu>& menu)
	{
		MenuButton* button = Manage (new MenuButton(text));
		DBG_SET_NAME(button, ConvertFromString(text));
		button->SetMenu(menu);

		return Append(button);
	}

	MenuButton* MenuBar::Append (MenuButton* button, const RefPtr<Menu>& menu)
	{
		if(!button) return 0;
		button->SetMenu(menu);

		return Append(button);
	}

	MenuButton* MenuBar::Append (MenuButton* button)
	{
		if(0 == button) return 0;

		int x = GetLastPosition();
		int y = position().y() + margin().bottom();
		int h = size().height() - margin().vsum();

		if(PushBackSubWidget(button)) {

			Size prefer = button->GetPreferredSize();

			SetSubWidgetPosition(button, x, y);

			if(button->IsExpandY()) {
				ResizeSubWidget(button, prefer.width(), h);
			} else {
				if(button->size().height() > h) {
					ResizeSubWidget(button, prefer.width(), h);
				} else {
					ResizeSubWidget(button, prefer.width(), button->size().height());
					SetSubWidgetPosition(button, x, y + (h - button->size().height()) / 2);
				}
			}

			events()->connect(button->clicked(), this, &MenuBar::OnMenuButtonClicked);
		}

		return button;
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

	void MenuBar::PerformMarginUpdate(const Margin& request)
	{
		// TODO: change margin
	}

	void MenuBar::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void MenuBar::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());

			set_size(*request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType MenuBar::Draw (Profile& profile)
	{
		using namespace BlendInt::Stock;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);

		program->reset();

		return Ignore;
	}

	ResponseType MenuBar::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType MenuBar::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType MenuBar::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType MenuBar::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType MenuBar::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType MenuBar::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType MenuBar::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	Size MenuBar::GetPreferredSize () const
	{
		Size preferred_size;

		if(first() == 0) {

			Font font;	// Get default font height
			preferred_size.set_width(200);

			int max_font_height = font.GetHeight();

			preferred_size.set_height(
			        max_font_height + AbstractButton::default_padding.vsum());// top padding: 2, bottom padding: 2

			preferred_size.add_height(margin().vsum());

		} else {

			Size tmp_size;

			preferred_size.set_width(-m_space);
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					preferred_size.add_width(tmp_size.width() + m_space);
					preferred_size.set_height(std::max(preferred_size.height(), tmp_size.height()));
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	bool MenuBar::IsExpandX() const
	{
		return true;
	}

	MenuButton* MenuBar::GetMenuButton (int index)
	{
		MenuButton* button = 0;

		button = dynamic_cast<MenuButton*>(GetWidgetAt(index));

		return button;
	}

	void MenuBar::InitializeMenuBar()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0);

		glBindVertexArray(m_vao);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}
	
	void MenuBar::OnMenuButtonClicked ()
	{
		MenuButton* original_active = m_active_button;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			MenuButton* menubutton = dynamic_cast<MenuButton*>(p);
			if(menubutton) {
				if(menubutton->focused()) {
					m_active_button = menubutton;
				}
			}
		}

		if(original_active) {	// If menu shows in context
			RefPtr<Menu> menu = original_active->menu();
			Context* context = Context::GetContext(this);
			context->Remove(menu.get());
			original_active->SetRoundType(RoundAll);

			menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);
		}

		if(m_active_button) {
			Context* context = Context::GetContext(this);

			RefPtr<Menu> menu = m_active_button->menu();

			int y = m_active_button->position().y();
			y = y - menu->size().height();

			if(y < 0) {
				y = 0;
			}

			menu->SetPosition(m_active_button->position().x(), y);
			context->Append(menu.get());
			m_active_button->SetRoundType(RoundTopLeft | RoundTopRight);
			context->SetFocusedWidget(menu.get());

			events()->connect(menu->triggered(), this, &MenuBar::OnMenuItemTriggered);
		}
	}

	void MenuBar::OnMenuItemTriggered(Action* item)
	{
		DBG_PRINT_MSG("menu item clicked: %s", ConvertFromString(item->text()).c_str());

		if(m_active_button) {
			if(RefPtr<Menu> menu = m_active_button->menu()) {
				menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);

				Context* context = Context::GetContext(this);
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
			menu->triggered().disconnectOne(this, &MenuBar::OnMenuItemTriggered);

			// DBG_PRINT_MSG("menu at layer: %d", menu->z());

			//if(type == WidgetVisibility) {
				Context* context = Context::GetContext(this);
				context->Remove(menu);
			//}
		}

		if(m_active_button) {
			m_active_button->SetRoundType(RoundAll);
			m_active_button = 0;
		}

	}

	int MenuBar::GetLastPosition ()
	{
		int pos = position().x() + margin().left();

		if(last()) {
			pos = last()->position().x() + last()->size().width() + m_space;
		}
		return pos;
	}
}
