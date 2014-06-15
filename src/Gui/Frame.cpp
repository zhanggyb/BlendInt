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

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	Frame::Frame ()
	: AbstractSingleContainer(), m_vao(0)
	{
		set_size(400, 300);

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

	Frame::~Frame ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool Frame::Setup (AbstractWidget* widget)
	{
		bool ret = false;

		if (SetSubWidget(widget)) {

			int x = position().x() + margin().left();
			int y = position().y() + margin().bottom();

			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			FillSubWidget(x, y, w, h);

			ret = true;
		}

		return ret;
	}

	bool Frame::Remove (AbstractWidget* widget)
	{
		bool ret = false;

		if(RemoveSubWidget(widget)) {

			ret = true;
		}

		return ret;
	}

	bool Frame::IsExpandX() const
	{
		if(sub_widget())
			return sub_widget()->IsExpandX();
		else
			return false;
	}

	bool Frame::IsExpandY() const
	{
		if(sub_widget())
			return sub_widget()->IsExpandY();
		else
			return false;
	}

	Size Frame::GetPreferredSize() const
	{
		Size prefer(400, 300);

		if(sub_widget()) {
			prefer = sub_widget()->GetPreferredSize();

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void Frame::UpdateContainer (const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerMargin: {

				if (sub_widget()) {
					const Margin* margin_p =
									static_cast<const Margin*>(request.data());
					set_margin(*margin_p);

					FillSubWidget(position(), size(), *margin_p);
				}
				break;
			}

			case ContainerRefresh: {
				Refresh();
				break;
			}

			default:
				break;

		}
	}

	bool Frame::UpdateGeometryTest (const WidgetUpdateRequest& request)
	{
		if(request.source() == this) {

			return AbstractSingleContainer::UpdateGeometryTest(request);

		} else if (request.source() == container()) {

			return true;

		} else {	// called by sub widget

			switch(request.type()) {
				case WidgetSize:
					return false;

				case WidgetPosition:
					return false;

				default:
					return false;
			}
		}
	}

	void Frame::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(request.source() == this || request.source() == container()) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
									static_cast<const Size*>(request.data());
					VertexTool tool;
					tool.Setup(*size_p, 0, RoundNone, 0);
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_inner->Reset();
					set_size(*size_p);

					if (sub_widget()) {
						FillSubWidget(position(), *size_p, margin());
					}

					break;
				}

				case WidgetPosition: {
					if (sub_widget()) {
						const Point* pos_p =
										static_cast<const Point*>(request.data());
						SetSubWidgetPosition(sub_widget(),
										pos_p->x() + margin().left(),
										pos_p->y() + margin().bottom());
					}
					break;
				}

				default:
					break;
			}

		}
	}

	ResponseType Frame::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType Frame::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 0);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		program->Reset();

		return AcceptAndContinue;
	}

} /* namespace BlendInt */
