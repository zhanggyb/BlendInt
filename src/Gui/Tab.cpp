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
#include <BlendInt/Gui/TabButton.hpp>

#include <BlendInt/Gui/Tab.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	TabHeader::TabHeader()
	: AbstractDequeContainer(),
	  m_vao(0)
	{
		set_size(400, 24);
		set_margin(5, 5, 5, 0);

		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0, false);

		m_buffer = tool.GenerateInnerBuffer();
	}

	TabHeader::~TabHeader()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool TabHeader::IsExpandX () const
	{
		return true;
	}

	Size TabHeader::GetPreferredSize () const
	{
		Size prefer(400, 24);

		// TODO: check sub widgets

		return prefer;
	}

	void TabHeader::UpdateContainer (const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerMargin: {
				// TODO: reset sub widgets
				break;
			}

			case ContainerRefresh: {
				Refresh();
				break;
			}

		}
	}

	void TabHeader::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetPosition: {
				const Point* pos_p = static_cast<const Point*>(request.data());
				int x = pos_p->x() - position().x();
				int y = pos_p->y() - position().y();
				MoveSubWidgets(x, y);
				break;
			}

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				VertexTool tool;
				tool.Setup(*size_p, 0, RoundNone, 0, false);
				tool.UpdateInnerBuffer(m_buffer.get());
				break;
			}

			default:
				break;
		}
	}

	ResponseType TabHeader::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();

		glBindVertexArray(m_vao);

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);
		program->SetUniform1i("Gamma", 0);

		glEnableVertexAttribArray(0);
		m_buffer->Bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
				m_buffer->GetBufferSize()
				/ (2 * sizeof(GLfloat)));
		m_buffer->Reset();

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);
		return Accept;
	}

	ResponseType TabHeader::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType TabHeader::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType TabHeader::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	// -------

	TabStack::TabStack ()
	: Stack(), m_vao(0)
	{
		InitOnce();
	}

	TabStack::~TabStack ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool TabStack::IsExpandX() const
	{
		return true;
	}

	bool TabStack::IsExpandY() const
	{
		return true;
	}

	void TabStack::UpdateGeometry (const WidgetUpdateRequest& request)
	{
			switch(request.type()) {

				case WidgetSize: {

					const Size* size_p = static_cast<const Size*>(request.data());
					VertexTool tool;
					tool.Setup(*size_p, DefaultBorderWidth(), RoundNone, 0);
					tool.UpdateInnerBuffer(m_inner.get());
					tool.UpdateOuterBuffer(m_outer.get());
					Stack::UpdateGeometry(request);
					break;
				}

				default: {
					Stack::UpdateGeometry(request);
					break;
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

		Color color = tm->tab().inner;

		program->SetVertexAttrib4fv("Color", color.data());

		glEnableVertexAttribArray(0);

		//DrawTriangleFan(0, m_inner_buffer.get());
		DrawTriangleStrip(0, m_inner.get());

		color = tm->tab().outline;

		program->SetUniform1i("AA", 1);

		program->SetVertexAttrib4fv("Color", color.data());

		DrawTriangleStrip(0, m_outer.get());

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

		return Accept;
	}

	void TabStack::InitOnce ()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), RoundNone, 0);
		m_inner = tool.GenerateInnerBuffer();
		m_outer = tool.GenerateOuterBuffer();
	}

	// ------------------------

	Tab::Tab ()
	: AbstractDequeContainer(), m_title_height(14), m_stack(0)
	{
		set_size(400, 300);

		m_stack = Manage(new TabStack);
		m_stack->SetMargin(4, 4, 4, 4);
		PushBackSubWidget(m_stack);

		events()->connect(m_group.button_index_toggled(), this, &Tab::OnButtonToggled);
	}

	Tab::~Tab ()
	{
	}

	void Tab::Add (const String& title, AbstractWidget* widget)
	{
		TabButton* btn = Manage(new TabButton);
		btn->SetText(title);

		PushBackSubWidget(btn);
		m_group.Add(btn);

		m_stack->Add(widget);

		AdjustGeometries();

		if(m_group.size() == 1) {
			btn->SetChecked(true);
		}
	}

	void Tab::UpdateContainer (const WidgetUpdateRequest& request)
	{
		switch(request.type()) {
			case ContainerRefresh: {

				Refresh();
				break;
			}

			default:
				break;
		}

	}

	void Tab::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetPosition: {
				const Point* pos_p = static_cast<const Point*>(request.data());

				int x = pos_p->x() - position().x();
				int y = pos_p->y() - position().y();

				MoveSubWidgets(x, y);
				break;
			}

			case WidgetSize: {
				break;
			}

			default:
				break;
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

		int w = size().width() - margin().hsum();
		int h = size().height() - margin().vsum();

		int temp = 0;
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
