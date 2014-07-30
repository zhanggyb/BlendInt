/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/ListView.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ListView::ListView ()
	: AbstractItemView(),
	  m_vao(0)
	{
		set_size(400, 300);
		set_drop_shadow(true);

		InitializeListView();

		AdjustScrollBarGeometries(position().x(), position().y(), size().width(), size().height());
	}

	ListView::~ListView ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool ListView::IsExpandX () const
	{
		return true;
	}

	bool ListView::IsExpandY () const
	{
		return true;
	}

	ResponseType ListView::Draw (const RedrawEvent& event)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(),
				(float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);
		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.447f,
				0.447f, 0.447f, 1.f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->Reset();

		DispatchDrawEvent(hbar(), event);
		DispatchDrawEvent(vbar(), event);

		return Accept;
	}

	ResponseType ListView::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ListView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ListView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ListView::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ListView::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ListView::MousePressEvent (const MouseEvent& event)
	{
		if (hbar()->visiable() && hbar()->Contain(event.position())) {
			return DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->visiable() && vbar()->Contain(event.position())) {
			return DispatchMousePressEvent(vbar(), event);
		}

		return Accept;
	}

	ResponseType ListView::MouseReleaseEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			return DispatchMouseReleaseEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			return DispatchMouseReleaseEvent(vbar(), event);
		}

		return Accept;
	}

	ModelIndex ListView::GetIndexAt (const Point& point) const
	{
		return ModelIndex();
	}

	ResponseType ListView::MouseMoveEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			return DispatchMouseMoveEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			return DispatchMouseMoveEvent(vbar(), event);
		}

		return Accept;
	}

	void ListView::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			AdjustScrollBarGeometries(request.position()->x(),
					request.position()->y(), size().width(), size().height());
		}

		ReportPositionUpdate(request);
	}

	void ListView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			AdjustScrollBarGeometries(position().x(), position().y(),
					request.size()->width(), request.size()->height());
		}

		ReportSizeUpdate(request);
	}

	void ListView::InitializeListView ()
	{
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);
		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		m_inner->Reset();
	}

}
