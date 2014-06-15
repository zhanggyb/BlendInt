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

#include <algorithm>

#include <BlendInt/Gui/Stack.hpp>
#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	Stack::Stack()
	: AbstractDequeContainer(),
	  m_vao(0),
	  m_index(0)
	{
		//set_preferred_size(400, 300);
		set_size(400, 300);

		InitializeStack();
	}

	Stack::~Stack()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Stack::Add (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			// TODO: lock widget's geometry

			int w = size().width() - margin().left() - margin().right();
			int h = size().height() - margin().top() - margin().bottom();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			if((sub_widget_size() - 1) != m_index) {
				widget->SetVisible(false);
			}
		}
	}

	void Stack::Insert (size_t index, AbstractWidget* widget)
	{
		if(InsertSubWidget(index, widget)) {
			// TODO: lock widget's geometry

			int w = size().width() - margin().left() - margin().right();
			int h = size().height() - margin().top() - margin().bottom();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());
		}
	}

	void Stack::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {
			m_index--;
		}
	}

	void Stack::SetIndex (size_t index)
	{
		size_t size = sub_widget_size();

		if(index > (size - 1)) return;

		if(size) {
			sub_widgets()->at(m_index)->SetVisible(false);

			m_index = index;
			sub_widgets()->at(m_index)->SetVisible(true);
		}
	}

	bool Stack::IsExpandX () const
	{
		bool ret = false;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandX()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	bool Stack::IsExpandY () const
	{
		bool ret = false;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandY()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	Size Stack::GetPreferredSize () const
	{
		Size prefer(400, 300);

		if(sub_widget_size()) {

			prefer.set_width(0);
			prefer.set_height(0);

			Size tmp;
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
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

	AbstractWidget* Stack::GetActiveWidget () const
	{
		if(sub_widget_size()) {
			return sub_widgets()->at(m_index);
		} else {
			return 0;
		}
	}

	AbstractWidget* Stack::GetWidget (size_t index)
	{
		size_t size = sub_widget_size();

		if(index > (size - 1)) return 0;

		return sub_widgets()->at(index);
	}

	void Stack::HideSubWidget(size_t index)
	{
		size_t size = sub_widget_size();

		if(size && index < (size - 1)) {
			AbstractWidget* p = sub_widgets()->at(index);
			p->SetVisible(false);
		}
	}

	void Stack::UpdateContainer (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case ContainerMargin: {

				const Margin* margin_p =
								static_cast<const Margin*>(request.data());

				int w = size().width() - margin_p->hsum();
				int h = size().height() - margin_p->vsum();

				ResizeSubWidgets(w, h);

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

	void Stack::UpdateGeometry (const WidgetUpdateRequest& request)
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
				const Size* new_size = static_cast<const Size*>(request.data());

				int w = new_size->width() - margin().hsum();
				int h = new_size->height() - margin().vsum();
				ResizeSubWidgets(w, h);

				VertexTool tool;
				tool.Setup(*new_size, 0, RoundNone, 0);
				tool.UpdateInnerBuffer(m_inner.get());

				break;
			}

			default:
				break;
		}

	}

	ResponseType Stack::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);
		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);

		glEnableVertexAttribArray(0);
		DrawTriangleStrip(0, m_inner.get());
		glDisableVertexAttribArray(0);

		program->Reset();
		glBindVertexArray(0);

		return AcceptAndContinue;
	}

	ResponseType Stack::CursorEnterEvent(bool entered)
	{
		return Ignore;
	}

	ResponseType Stack::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::ContextMenuPressEvent(const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::ContextMenuReleaseEvent(const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::MousePressEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::MouseReleaseEvent(const MouseEvent& event)
	{
		return Ignore;
	}
	
	IteratorPtr Stack::CreateIterator (const DeviceEvent& event)
	{
		RefPtr<SingleIterator> ret;
		if(sub_widget_size()) {
			ret.reset(new SingleIterator(sub_widgets()->at(m_index)));
		} else {
			ret.reset(new SingleIterator(0));
		}

		return ret;
	}

	ResponseType Stack::MouseMoveEvent(const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	void Stack::InitializeStack()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);
		m_inner = tool.GenerateInnerBuffer();
	}

}
