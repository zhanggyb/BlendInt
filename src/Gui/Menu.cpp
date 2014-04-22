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

#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Gui/FontCache.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	int Menu::DefaultMenuItemHeight = 16;
	int Menu::DefaultIconSpace = 4;
	int Menu::DefaultShortcutSpace = 20;

	Menu::Menu ()
	: RoundWidget(), m_highlight(0), m_inner_buffer(0), m_outer_buffer(0), m_highlight_buffer(0)
	{
		set_size (20, 20);
		set_preferred_size(20, 20);

		activate_events();

		InitOnce();
	}

	Menu::~Menu ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Menu::SetTitle(const String& title)
	{
		m_title = title;
	}

	void Menu::AddActionItem(const String& text)
	{
		RefPtr<ActionItem> item = ActionItem::Create(text);

		AddActionItem(item);
	}

	void Menu::AddActionItem(const String& text, const String& shortcut)
	{
		RefPtr<ActionItem> item = ActionItem::Create(text, shortcut);

		AddActionItem(item);
	}

	void Menu::AddActionItem(const RefPtr<Icon>& icon, const String& text)
	{
		RefPtr<ActionItem> item = ActionItem::Create(icon, text);

		AddActionItem(item);
	}

	void Menu::AddActionItem(const RefPtr<Icon>& icon, const String& text, const String& shortcut)
	{
		RefPtr<ActionItem> item = ActionItem::Create(icon, text, shortcut);

		AddActionItem(item);
	}

	void Menu::AddActionItem(const RefPtr<ActionItem>& item)
	{
		unsigned int width = 0;

		width = item->GetTextLength(m_font);

		width += 16 + DefaultIconSpace + DefaultShortcutSpace;

		if(m_list.size()) {
			set_preferred_size(std::max(size().width(), static_cast<unsigned int>(radius() * 2) + width),
					size().height() + DefaultMenuItemHeight);
		} else {
			set_preferred_size(radius() * 2 + width,
					radius() * 2 + DefaultMenuItemHeight);
		}
		Resize(preferred_size());

		m_list.push_back(item);
	}

	ResponseType Menu::MouseMoveEvent(const MouseEvent& event)
	{
		unsigned int orig = m_highlight;

		if(!Contain(event.position())) {
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

		m_highlight = GetHighlightNo(static_cast<int>(event.position().y()));

		if(orig != m_highlight) {
			Refresh();
		}
		return AcceptAndBreak;
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

			ActionItem* item = m_list[m_highlight - 1].get();
			m_triggered.fire(item);
		}

		return Accept;
	}

	ResponseType Menu::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	void Menu::Update(const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					GenerateFormBuffer(*size_p, round_type(), radius(), m_inner_buffer.get(), m_outer_buffer.get(), 0);
					ResetHighlightBuffer(size_p->width());
					m_shadow->Resize(*size_p);
					break;
				}

				case FormRoundType: {
					const int* type_p = static_cast<const int*>(request.data());
					GenerateFormBuffer(size(), *type_p, radius(), m_inner_buffer.get(), m_outer_buffer.get(), 0);
					break;
				}

				case FormRoundRadius: {
					const float* radius_p = static_cast<const float*>(request.data());
					GenerateFormBuffer(size(), round_type(), *radius_p, m_inner_buffer.get(), m_outer_buffer.get(), 0);
					break;
				}

				default:
					RoundWidget::Update(request);
			}

		}
	}

	ResponseType Menu::Draw (const RedrawEvent& event)
	{
		using std::deque;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		if(m_shadow)
			m_shadow->Draw(mvp);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color",
				themes()->menu.inner.r()/255.f,
				themes()->menu.inner.g()/255.f,
				themes()->menu.inner.b()/255.f,
				themes()->menu.inner.a()/255.f);

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_inner_buffer.get());

		program->SetVertexAttrib4f("Color",
				themes()->menu.outline.r()/255.f,
				themes()->menu.outline.g()/255.f,
				themes()->menu.outline.b()/255.f,
				themes()->menu.outline.a() / 255.f
				);
		program->SetUniform1i("AA", 1);

		DrawTriangleStrip(0, m_outer_buffer.get());

		if(m_highlight) {
			program->SetUniform1i("AA", 0);

			glEnableVertexAttribArray(1);

			glm::mat4 h_mvp = glm::translate(mvp, glm::vec3(0.f, size().height() - radius() - static_cast<float>(DefaultMenuItemHeight * m_highlight), 0.f));
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(h_mvp));

			DrawShadedTriangleFan(0, 1, m_highlight_buffer.get());

			glDisableVertexAttribArray(1);
		}

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		float h = size().height() - radius();

		int advance = 0;
		for(deque<RefPtr<ActionItem> >::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			h = h - DefaultMenuItemHeight;

			if((*it)->icon()) {
				(*it)->icon()->Draw(mvp, 8, h + 8, 16, 16);
			}
			advance = m_font.Print(mvp, 16 + DefaultIconSpace, h - m_font.get_descender(), (*it)->text());
			m_font.Print(mvp, 16 + DefaultIconSpace + advance + DefaultShortcutSpace, h - m_font.get_descender(), (*it)->shortcut());
		}

		return Accept;
	}

	void Menu::ResetHighlightBuffer (unsigned int width)
	{
		Size size(width, DefaultMenuItemHeight);
		GenerateShadedFormBuffer (&size,
				DefaultBorderWidth(),
				RoundNone,
				0.0,
				themes()->menu_item.inner_sel,
				themes()->menu_item.shadetop,
				themes()->menu_item.shadedown,
				Vertical,
				m_highlight_buffer.get());
	}
	
	void Menu::RemoveActionItem (size_t index)
	{
	}
	
	void Menu::RemoveActionItem (const RefPtr<ActionItem>& item)
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

	unsigned int Menu::GetHighlightNo(int y)
	{
		int h = position().y() + size().height() - y;

		if(h < radius() || h > (size().height() - radius())) {
			return 0;
		}

		return (h - radius()) / (size().height() / m_list.size()) + 1;
	}

	void Menu::InitOnce ()
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_highlight_buffer.reset(new GLArrayBuffer);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		GenerateFormBuffer(size(), round_type(), radius(),
		        m_inner_buffer.get(), m_outer_buffer.get(), 0);

		ResetHighlightBuffer(20);

		glBindVertexArray(0);

		m_shadow.reset(new Shadow);
		m_shadow->Resize(size());
	}

}

