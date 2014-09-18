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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	int Menu::DefaultMenuItemHeight = 16;
	int Menu::DefaultIconSpace = 4;
	int Menu::DefaultShortcutSpace = 20;

	Menu::Menu ()
	: AbstractWidget(), m_highlight(0), inner_(0), outer_(0), m_highlight_buffer(0)
	{
		set_size (20, 20);
		InitializeMenu();
	}

	Menu::~Menu ()
	{
		glDeleteVertexArrays(3, m_vao);
	}

	void Menu::SetTitle(const String& title)
	{
		m_title = title;
	}

	void Menu::AddAction(const String& text)
	{
		RefPtr<Action> item = Action::Create(text);

		AddAction(item);
	}

	void Menu::AddAction(const String& text, const String& shortcut)
	{
		RefPtr<Action> item = Action::Create(text, shortcut);

		AddAction(item);
	}

	void Menu::AddAction(const RefPtr<AbstractIcon>& icon, const String& text)
	{
		RefPtr<Action> item = Action::Create(icon, text);

		AddAction(item);
	}

	void Menu::AddAction(const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut)
	{
		RefPtr<Action> item = Action::Create(icon, text, shortcut);

		AddAction(item);
	}

	void Menu::AddAction(const RefPtr<Action>& item)
	{
		int width = 0;

		width = item->GetTextLength(m_font);

		width += 16 + DefaultIconSpace + DefaultShortcutSpace;

		Size s;

		if(m_list.size()) {
			s.set_width(std::max(size().width(), (int)round_radius() * 2 + width));
			s.set_height(size().height() + DefaultMenuItemHeight);
		} else {
			s.set_width(round_radius() * 2 + width);
			s.set_height(round_radius() * 2 + DefaultMenuItemHeight);
		}

		Resize(s);

		m_list.push_back(item);
	}

	ResponseType Menu::MouseMoveEvent(const MouseEvent& event)
	{
		unsigned int orig = m_highlight;

		if(!Contain(event.global_position())) {
			m_highlight = 0;

			if(orig != m_highlight) {
				Refresh();
			}
			return Accept;
		}

		if(!m_list.size()) {
			m_highlight = 0;
			if(orig != m_highlight) {
				Refresh();
			}
			return Accept;
		}

		m_highlight = GetHighlightNo(static_cast<int>(event.global_position().y()));

		if(orig != m_highlight) {
			Refresh();
		}
		return Accept;
	}

	ResponseType Menu::MousePressEvent (const MouseEvent& event)
	{
		/*
		if(!m_menubin->size()) {
			return Accept;
		}

		m_triggered.fire(m_menubin->GetMenuItem(m_highlight - 1));
		*/
		if(m_highlight > 0) {

			Action* item = m_list[m_highlight - 1].get();
			m_triggered.fire(item);
		}

		return Accept;
	}

	ResponseType Menu::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	void Menu::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(), round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			ResetHighlightBuffer(request.size()->width());
			set_size(*request.size());
		}

		ReportSizeUpdate(request);
	}

	void Menu::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			set_round_type(*request.round_type());
		}

		ReportRoundTypeUpdate(request);
	}

	void Menu::PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(),
			        round_type(), *request.round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			set_round_radius(*request.round_radius());
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType Menu::Draw (Profile& profile)
	{
		using Stock::Shaders;
		using std::deque;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4fv("a_color", Theme::instance->menu().inner.data());

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		program->SetVertexAttrib4fv("a_color", Theme::instance->menu().outline.data());
		program->SetUniform1i("u_AA", 1);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		if(m_highlight) {
			program->SetUniform1i("u_AA", 0);

			glm::vec3 pos((float) position().x(), (float) position().y(), 0.f);
			pos.y = pos.y + size().height() - round_radius() - static_cast<float>(DefaultMenuItemHeight * m_highlight);

			program->SetUniform3fv("u_position", 1, glm::value_ptr(pos));

			glBindVertexArray(m_vao[2]);
			glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		}

		glBindVertexArray(0);
		program->reset();

		float h = size().height() - round_radius();

		int advance = 0;
		for(deque<RefPtr<Action> >::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			h = h - DefaultMenuItemHeight;

			if((*it)->icon()) {
				//(*it)->icon()->Draw(mvp, 8, h + 8, 16, 16);
			}
			advance = m_font.Print(position().x() + 16 + DefaultIconSpace,
			        position().y() + h - m_font.GetDescender(), (*it)->text());
			m_font.Print(
			        position().x() + 16 + DefaultIconSpace + advance
			                + DefaultShortcutSpace,
			        position().y() + h - m_font.GetDescender(),
			        (*it)->shortcut());
		}

		return Accept;
	}

	void Menu::ResetHighlightBuffer (int width)
	{
		Size size(width, DefaultMenuItemHeight);

		VertexTool tool;
		tool.GenerateVertices(size,
				DefaultBorderWidth(),
				RoundNone,
				0,
				Theme::instance->menu_item().inner_sel,
				Vertical,
				Theme::instance->menu_item().shadetop,
				Theme::instance->menu_item().shadedown
				);

		m_highlight_buffer->bind();
		m_highlight_buffer->set_data(tool.inner_size(), tool.inner_data());
		m_highlight_buffer->reset();
	}
	
	void Menu::RemoveAction (size_t index)
	{
	}
	
	void Menu::RemoveAction (const RefPtr<Action>& item)
	{
	}
	
	ResponseType Menu::FocusEvent (bool focus)
	{
		DBG_PRINT_MSG("focus %s", focus ? "on" : "off");

		if(focus) {
			SetVisible(true);
		} else {
			SetVisible(false);
		}

		Refresh();

		return Ignore;
	}

	ResponseType Menu::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Menu::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Menu::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Menu::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	unsigned int Menu::GetHighlightNo(int y)
	{
		int h = position().y() + size().height() - y;

		if(h < round_radius() || h > (size().height() - round_radius())) {
			return 0;
		}

		return (h - round_radius()) / (size().height() / m_list.size()) + 1;
	}

	void Menu::InitializeMenu ()
	{
		glGenVertexArrays(3, m_vao);

		VertexTool tool;
		tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(), round_radius());

		glBindVertexArray(m_vao[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		// Now set buffer for hightlight bar
		Size highlight_size(size().width(), DefaultMenuItemHeight);
		tool.GenerateVertices(highlight_size,
				DefaultBorderWidth(),
				RoundNone,
				0,
				Theme::instance->menu_item().inner_sel,
				Vertical,
				Theme::instance->menu_item().shadetop,
				Theme::instance->menu_item().shadedown
				);

		glBindVertexArray(m_vao[2]);
		m_highlight_buffer.reset(new GLArrayBuffer);
		m_highlight_buffer->generate();
		m_highlight_buffer->bind();

		m_highlight_buffer->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

}
