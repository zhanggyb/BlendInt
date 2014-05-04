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

#include <BlendInt/Gui/TabButton.hpp>

#include <BlendInt/Gui/Tab.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	TabStack::TabStack ()
	: Stack(), m_vao(0)
	{
		set_expand_x(true);
		set_expand_y(true);
		InitOnce();
	}

	TabStack::~TabStack ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void TabStack::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch(request.type()) {

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					GenerateFormBuffer(
									*size_p,
									RoundNone,
									0.f,
									m_inner.get(),
									m_outer.get(),
									0);

					Stack::Update(request);
					break;
				}

				default: {
					Stack::Update(request);
					break;
				}
			}

		}
	}

	ResponseType TabStack::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);

		Theme* tm = Theme::instance;

		glm::vec4 color;
		color.r = tm->tab().inner.r() / 255.f;
		color.g = tm->tab().inner.g() / 255.f;
		color.b = tm->tab().inner.b() / 255.f;
		color.a = tm->tab().inner.a() / 255.f;

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));

		glEnableVertexAttribArray(0);

		//DrawTriangleFan(0, m_inner_buffer.get());
		DrawTriangleStrip(0, m_inner.get());

		color.r = tm->tab().outline.r() / 255.f;
		color.g = tm->tab().outline.g() / 255.f;
		color.b = tm->tab().outline.b() / 255.f;
		color.a = tm->tab().outline.a() / 255.f;

		program->SetUniform1i("AA", 1);

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));

		DrawTriangleStrip(0, m_outer.get());

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

		return Accept;
	}

	void TabStack::InitOnce ()
	{
		m_inner.reset(new GLArrayBuffer);
		m_outer.reset(new GLArrayBuffer);

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		GenerateFormBuffer(
						size(),
						RoundNone,
						0.f,
						m_inner.get(),
						m_outer.get(),
						0);

		glBindVertexArray(0);
	}

	// ------------------------

	Tab::Tab ()
	: AbstractDequeContainer(), m_title_height(14), m_stack(0)
	{
		set_preferred_size(400, 300);
		set_size(400, 300);

		m_stack = Manage(new TabStack);
		m_stack->SetMargin(4, 4, 4, 4);
		AppendSubWidget(m_stack);

		events()->connect(m_group.button_index_toggled(), this, &Tab::OnButtonToggled);
	}

	Tab::~Tab ()
	{
	}

	void Tab::Add (const String& title, AbstractWidget* widget)
	{
		TabButton* btn = Manage(new TabButton);
		btn->SetText(title);

		AppendSubWidget(btn);
		m_group.Add(btn);

		m_stack->Add(widget);

		AdjustGeometries();

		if(m_group.size() == 1) {
			btn->SetChecked(true);
		}
	}

	void Tab::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					MoveSubWidgets(x, y);
					break;
				}

				case FormSize: {
					break;
				}

				case WidgetRefresh: {

					Refresh();
					break;
				}

				default:
					break;
			}

		}
	}

	ResponseType Tab::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType Tab::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Tab::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	void Tab::OnButtonToggled (int index, bool toggled)
	{
		m_stack->SetIndex(index);
		Refresh();
	}

	void Tab::AdjustGeometries()
	{
		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();

		int btn_y = position().y() + size().height() - margin().top() - m_title_height;

		unsigned int w = size().width() - margin().left() - margin().right();
		unsigned int h = size().height() - margin().top() - margin().bottom();

		unsigned int temp = 0;
		for(std::vector<AbstractButton*>::iterator it = m_group.buttons()->begin(); it != m_group.buttons()->end(); it++)
		{
			SetSubWidgetPosition((*it), x + temp, btn_y);
			ResizeSubWidget((*it), (*it)->size().width(), m_title_height);
			temp += (*it)->size().width();
		}

		SetSubWidgetPosition(m_stack, x, y);
		ResizeSubWidget(m_stack, w, h - m_title_height);
	}

}
