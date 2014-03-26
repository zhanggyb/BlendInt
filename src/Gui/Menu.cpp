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

	int Menu::DefaultMenuItemHeight = 20;

	Menu::Menu ()
	: RoundWidget(), m_highlight(0), m_inner_buffer(0), m_outer_buffer(0), m_highlight_buffer(0)
	{
		set_size (20, 20);
		set_preferred_size(20, 20);

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

		m_list.push_back(item);

		// Resize
		//Resize(200, DefaultMenuItemHeight * m_menubin->size() + radius() * 2);
	}

	void Menu::AddActionItem(const RefPtr<Icon>& icon, const String& text)
	{
		RefPtr<ActionItem> item = ActionItem::Create(icon, text);

		m_list.push_back(item);

		//Resize(200, DefaultMenuItemHeight * m_menubin->size() + radius() * 2);
	}

	void Menu::AddActionItem(const RefPtr<Icon>& icon, const String& text, const String& shortcut)
	{
		RefPtr<ActionItem> item = ActionItem::Create(icon, text, shortcut);
		m_list.push_back(item);

		//Resize(200, DefaultMenuItemHeight * m_menubin->size() + radius() * 2);
	}

	void Menu::AddActionItem(const RefPtr<ActionItem>& item)
	{
		m_list.push_back(item);
	}

	ResponseType Menu::MouseMoveEvent(const MouseEvent& event)
	{
		if(!contain(event.position())) {
			m_highlight = 0;
			return Accept;
		}

		/*
		if(!m_menubin->size()) {
			m_highlight = 0;
			return Accept;
		}
		*/

		m_highlight = GetHighlightNo(static_cast<int>(event.position().y()));

		return Accept;
	}

	ResponseType Menu::MousePressEvent (const MouseEvent& event)
	{
		if(!contain(event.position())) {
			return Ignore;
		}

		/*
		if(!m_menubin->size()) {
			return Accept;
		}

		m_triggered.fire(m_menubin->GetMenuItem(m_highlight - 1));
		*/

		return Accept;
	}

	ResponseType Menu::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	bool Menu::Update(const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					GenerateFormBuffer(*size_p, round_type(), radius(), m_inner_buffer.get(), m_outer_buffer.get(), 0);
					ResetHighlightBuffer(size_p->width());
					return true;
				}

				case FormRoundType: {
					const int* type_p = static_cast<const int*>(request.data());
					GenerateFormBuffer(size(), *type_p, radius(), m_inner_buffer.get(), m_outer_buffer.get(), 0);
					return true;
				}

				case FormRoundRadius: {
					const float* radius_p = static_cast<const float*>(request.data());
					GenerateFormBuffer(size(), round_type(), *radius_p, m_inner_buffer.get(), m_outer_buffer.get(), 0);
					return true;
				}

				default:
					return RoundWidget::Update(request);
			}

		} else {
			return false;
		}
	}

	ResponseType Menu::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib1f("z", (float) z());

		program->SetVertexAttrib4f("color",
				themes()->menu.inner.r()/255.f,
				themes()->menu.inner.g()/255.f,
				themes()->menu.inner.b()/255.f,
				themes()->menu.inner.a()/255.f
				);

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_inner_buffer.get());

		program->SetVertexAttrib4f("color",
				themes()->menu.outline.r()/255.f,
				themes()->menu.outline.g()/255.f,
				themes()->menu.outline.b()/255.f,
				themes()->menu.outline.a() / WIDGET_AA_JITTER /255.f
				);

		DrawTriangleStrip(program, mvp, 0, m_outer_buffer.get());

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		/*
		//RoundWidget::Render();
		// draw inner, simple fill
		glColor4ub(themes()->menu.inner.r(),
		        themes()->menu.inner.g(),
		        themes()->menu.inner.b(),
		        themes()->menu.inner.a());
		DrawInnerBuffer(m_inner_buffer.get());

		// draw outline
		unsigned char tcol[4] = { themes()->menu.outline.r(),
		        themes()->menu.outline.g(),
		        themes()->menu.outline.b(),
		        themes()->menu.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		DrawOutlineBuffer(m_outer_buffer.get());

		FontCache* fc = FontCache::create(Font("Sans"));

		int h = 0;
		glTranslatef(0.0, size().height() - radius(), 0.0);

		glColor4ub(225, 225, 225, 255);

		for (std::list<MenuItem*>::iterator it = m_menubin->list().begin(); it != m_menubin->list().end(); it++) {
			h = - DefaultMenuItemHeight;
			glTranslatef(0.0, h, 0.0);
			//DispatchRender(*it);
			//glRectf(0.0, 0.0, 200, DefaultMenuItemHeight);
			if((*it)->icon()) {
				//DispatchRender ((*it)->icon());
			}
			fc->print(5 + 16, 5, (*it)->text());
			fc->print(120, 5, (*it)->shortcut());
		}

		if(m_highlight) {
			glPopMatrix();
			glPushMatrix();
			glTranslatef(position().x(),
						 position().y() + size().height() - radius() - static_cast<float>(DefaultMenuItemHeight * m_highlight),
						 z());

//			glColor4ub(0, 0, 225, 25);
//			glRectf(0.0, 0.0, 200, DefaultMenuItemHeight);

			m_highlight_buffer->Bind();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));
			//glDrawArrays(GL_POLYGON, 0, m_highlight_buffer->vertices());

			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			m_highlight_buffer->Reset();

			MenuItem* item = m_menubin->GetMenuItem(m_highlight - 1);

			if(item->icon()) {
				//DispatchRender (item->icon());
			}
			fc->print(5 + 16, 5, item->text());
			fc->print(120, 5, item->shortcut());

		}
		*/

		return Accept;
	}

	void Menu::ResetHighlightBuffer (unsigned int width)
	{
		Size size(width, DefaultMenuItemHeight);

//		GenerateShadedFormBuffer (&size,
//				DefaultBorderWidth(),
//				RoundNone,
//				0.0,
//				&(themes()->menu_item),
//				Vertical,
//				m_highlight_buffer.get());

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
	}

}

