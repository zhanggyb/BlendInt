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
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	Stack::Stack()
	: AbstractDequeContainer(),
	  m_index(0)
	{
		//set_preferred_size(400, 300);
		set_size(400, 300);
	}

	Stack::~Stack()
	{
	}

	void Stack::Add (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

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

		for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
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

		for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
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

	void Stack::UpdateContainer (const ContainerUpdateRequest& request)
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

			default: {
				ReportContainerUpdate(request);
				break;
			}
		}
	}

	void Stack::ProcessPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		ReportPositionUpdate(request);
	}

	void Stack::ProcessSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			set_size(*request.size());
			ResizeSubWidgets(w, h);
		}

		ReportSizeUpdate(request);
	}

	ResponseType Stack::Draw (const RedrawEvent& event)
	{
		return Ignore;
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
		return Ignore;
	}

}
