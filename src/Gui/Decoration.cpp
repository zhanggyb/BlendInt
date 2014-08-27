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
#include <BlendInt/Gui/Decoration.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Section.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	Decoration::Decoration()
	: AbstractContainer(),
	  m_space(4),
	  m_pressed(false)
	{
		set_size(200, 20);
		InitializeDecoration();
	}

	Decoration::~Decoration ()
	{
		glDeleteVertexArrays(1, m_vao);
	}

	void Decoration::PushFront (AbstractWidget* widget)
	{
	}

	void Decoration::PushBack (AbstractWidget* widget)
	{
	}

	bool Decoration::IsExpandX () const
	{
		return true;
	}

	bool Decoration::IsExpandY () const
	{
		return true;
	}

	Size Decoration::GetPreferredSize () const
	{
		return Size(200, 20);
	}

	void Decoration::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {

			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);

		}

		ReportPositionUpdate(request);
	}

	void Decoration::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, round_type(), round_radius());

			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			int x = position().x() + margin().left();
			if (first()) {
				x = first()->position().x();
			}

			int y = position().y() + margin().bottom();
			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			FillSubWidgets(x, y, w, h, m_space);

			set_size(*request.size());

		} else if (request.target()->container() == this) {

			FillSubWidgets(position(), size(), margin(), m_space);

		}

		ReportSizeUpdate(request);
	}

	ResponseType Decoration::Draw (Profile& profile)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.667f, 0.825f, 0.575f, 1.0f);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);

		program->reset();
		return Accept;
	}

	ResponseType Decoration::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Decoration::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Decoration::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Decoration::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Decoration::MousePressEvent (const MouseEvent& event)
	{
		if(container()) {
			m_last = container()->position();
			m_cursor = event.position();
			m_pressed = true;

			if(event.section() == container()->container()) {
				if(event.section()->last_hover_widget() == this) {
					event.section()->MoveToLast();
					return Accept;
				}
			}

		}

		return Ignore;
	}

	ResponseType Decoration::MouseReleaseEvent (const MouseEvent& event)
	{
		m_pressed = false;
		return Ignore;
	}

	ResponseType Decoration::MouseMoveEvent (const MouseEvent& event)
	{
		if (container() && m_pressed) {

			int offset_x = event.position().x() - m_cursor.x();
			int offset_y = event.position().y() - m_cursor.y();

			container()->SetPosition(m_last.x() + offset_x,
			        m_last.y() + offset_y);
		}
		return Ignore;
	}

	void Decoration::InitializeDecoration()
	{
		set_round_type(RoundTopLeft | RoundTopRight);
		set_round_radius(10.f);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, round_type(), round_radius());

		glGenVertexArrays(1, m_vao);

		glBindVertexArray(m_vao[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void Decoration::FillSubWidgets (const Point& out_pos, const Size& out_size,
	        const Margin& margin, int space)
	{
	}

	void Decoration::FillSubWidgets (int x, int y, int width, int height,
	        int space)
	{
	}

	void Decoration::RealignSubWidgets (const Size& size, const Margin& margin,
	        int space)
	{
	}

	int Decoration::GetLastPosition () const
	{
		return 0;
	}

}
