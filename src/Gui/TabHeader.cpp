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

#include <BlendInt/Gui/TabHeader.hpp>
#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

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

		events()->connect(m_group.button_index_clicked(), this, &TabHeader::OnButtonIndexClicked);
		events()->connect(m_group.button_index_toggled(), this, &TabHeader::OnButtonIndexToggled);
	}

	TabHeader::~TabHeader()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void TabHeader::PushBack (TabButton* button)
	{
		int x = GetLastPosition ();
		int y = position().y() + margin().bottom();
		int h = size().height() - margin().vsum();

		if (PushBackSubWidget(button)) {

			SetSubWidgetPosition(button, x, y);
			if (button->IsExpandY()) {
				ResizeSubWidget(button, button->size().width(), h);
			} else {

				if (button->size().height() > h) {
					ResizeSubWidget(button, button->size().width(), h);
				}

			}

			m_group.Add(button);

			if(m_group.size() == 1) {
				button->SetChecked(true);
			}
		}
	}

	bool TabHeader::IsExpandX () const
	{
		return true;
	}

	Size TabHeader::GetPreferredSize () const
	{
		Size prefer(400, 24);

		if(sub_widget_size() == 0) {
			Font font;
			int max_font_height = font.GetHeight();
			prefer.set_height(max_font_height + margin().vsum());
		} else {
			AbstractWidget* widget = 0;
			Size tmp_size;

			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				widget = *it;

				if(widget->visiable()) {
					tmp_size = widget->GetPreferredSize();

					prefer.add_width(tmp_size.width());
					prefer.set_height(std::max(prefer.height(), tmp_size.height()));
				}
			}

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

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
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();

		glBindVertexArray(m_vao);

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color", 0.208f, 0.208f, 0.208f, 1.0f);
		program->SetUniform1i("Gamma", 0);

		glEnableVertexAttribArray(0);
		DrawTriangleFan(0, m_buffer.get());
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

	void TabHeader::OnButtonIndexClicked(int index)
	{
		m_button_index_clicked.fire(index);
	}

	void TabHeader::OnButtonIndexToggled(int index, bool toggled)
	{
		m_button_index_toggled.fire(index, toggled);
	}

	int TabHeader::GetLastPosition() const
	{
		int x = position().x() + margin().left();

		if(sub_widget_size()) {
			x = sub_widgets()->back()->position().x();
			x += sub_widgets()->back()->size().width();
		}

		return x;
	}

}
