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
 * License along with BlendInt.	 If not, see
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
#include <BlendInt/Gui/Workspace.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	Workspace::Workspace()
	: AbstractDequeContainer()
	{
		set_size(500, 400);
		set_scissor_test(true);
		set_drop_shadow(true);

		InitializeWorkspace();
	}

	Workspace::~Workspace()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void Workspace::PushBack (VirtualWindow* window)
	{
		if(PushBackSubWidget(window)) {

			EnableShadow(window);

			int x = position().x() + margin().left();
			int y = position().y() + margin().bottom();

			window->SetPosition(x, y);
		}
	}

	void Workspace::PushFront (VirtualWindow* window)
	{
		if (PushFrontSubWidget(window)) {

			EnableShadow(window);

			int x = position().x() + margin().left();
			int y = position().y() + margin().bottom();

			window->SetPosition(x, y);
		}
	}

	void Workspace::Remove (VirtualWindow* window)
	{
		RemoveSubWidget(window);
	}

	bool Workspace::IsExpandX () const
	{
		return true;
	}

	bool Workspace::IsExpandY () const
	{
		return true;
	}

	Size Workspace::GetPreferredSize () const
	{
		Size prefer;

		if(sub_widget_size() == 0) {
			prefer.reset(500, 400);
		} else {
			Size tmp;
			for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				tmp = (*it)->GetPreferredSize();
				prefer.set_width(std::max(prefer.width(), tmp.width()));
				prefer.set_height(std::max(prefer.height(), tmp.height()));
			}

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void Workspace::UpdateContainer (const ContainerUpdateRequest& request)
	{
		ReportContainerUpdate(request);
	}

	void Workspace::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.source()->container() == this) {
			EnableShadow(request.source());
		}

		ReportPositionUpdate(request);
	}

	void Workspace::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			Size inner_size(request.size()->width() - margin().hsum(),
			        request.size()->height() - margin().vsum());

			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0.f);
			m_background->Bind();
			tool.SetInnerBufferData(m_background.get());

			tool.Setup(inner_size, 0, RoundNone, 0.f);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());

			m_inner->Reset();

			set_size(*request.size());
		} else if (request.source()->container() == this) {

			EnableShadow(request.source());
		}

		ReportSizeUpdate(request);
	}

	void Workspace::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if (request.source()->container() == this) {
			EnableShadow(request.source());
		}

		ReportRoundTypeUpdate(request);
	}

	void Workspace::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if (request.source()->container() == this) {
			EnableShadow(request.source());
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType Workspace::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.407f, 0.407f, 0.407f, 1.0f);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		program->SetUniform3f("u_position",
		        (float) position().x() + margin().left(),
		        (float) position().y() + margin().bottom(), 0.f);

		program->SetVertexAttrib4f("a_color", 0.208f, 0.208f, 0.208f, 1.0f);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		glBindVertexArray(0);

		program->Reset();

		return Ignore;
	}

	ResponseType Workspace::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Workspace::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Workspace::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Workspace::InitializeWorkspace ()
	{
		Size inner_size(size().width() - margin().hsum(), size().height() - margin().vsum());

		glGenVertexArrays(2, m_vao);

		// index 0 for background
		glBindVertexArray(m_vao[0]);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0.f);

		m_background.reset(new GLArrayBuffer);
		m_background->Generate();
		m_background->Bind();

		tool.SetInnerBufferData(m_background.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(m_vao[1]);
		tool.Setup(inner_size, 0, RoundNone, 0.f);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();

		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

}

