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
	: AbstractContainer(),
	  m_active_widget(0)
	{
		set_size(400, 300);
	}

	Stack::~Stack()
	{
	}

	void Stack::PushBack (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			if(CountSubWidgets() == 1) {
				m_active_widget = widget;
				m_active_widget->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void Stack::Insert (int index, AbstractWidget* widget)
	{
		if(InsertSubWidget(index, widget)) {
			int w = size().width() - margin().left() - margin().right();
			int h = size().height() - margin().top() - margin().bottom();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			widget->SetVisible(false);
		}
	}

	void Stack::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {

			if(m_active_widget == widget) {

				if(CountSubWidgets() == 0) {
					m_active_widget = 0;
				} else {
					m_active_widget = first();
					m_active_widget->SetVisible(true);
				}

			}
		}
	}

	void Stack::SetIndex (int index)
	{
		int count = CountSubWidgets();

		if(index > (count - 1)) return;

		if(count) {

			AbstractWidget* widget = GetWidgetAt(index);
			if(m_active_widget == widget) {
				return;
			}

			m_active_widget->SetVisible(false);
			m_active_widget = widget;
			m_active_widget->SetVisible(true);
		}
	}

	bool Stack::IsExpandX () const
	{
		bool ret = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	bool Stack::IsExpandY () const
	{
		bool ret = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	Size Stack::GetPreferredSize () const
	{
		Size prefer(400, 300);

		if(first()) {

			prefer.set_width(0);
			prefer.set_height(0);

			Size tmp;
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				tmp = p->GetPreferredSize();
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
		return m_active_widget;
	}

	AbstractWidget* Stack::GetWidget (int index)
	{
		int count = CountSubWidgets();

		if(index > (count - 1)) return 0;

		return GetWidgetAt(index);
	}

	void Stack::HideSubWidget(int index)
	{
		int count = CountSubWidgets();

		if(count && index < (count - 1)) {
			AbstractWidget* p = GetWidgetAt(index);
			p->SetVisible(false);
		}
	}

	void Stack::PerformMarginUpdate(const Margin& request)
	{
		int w = size().width() - request.hsum();
		int h = size().height() - request.vsum();

		ResizeSubWidgets(w, h);
	}

	bool Stack::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	bool Stack::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	void Stack::PerformPositionUpdate (
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

	void Stack::PerformSizeUpdate (const SizeUpdateRequest& request)
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
	
	ResponseType Stack::MouseMoveEvent(const MouseEvent& event)
	{
		return Ignore;
	}

}
