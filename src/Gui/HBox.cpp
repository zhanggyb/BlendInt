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
#endif  // __UNIX__

#include <boost/smart_ptr.hpp>
#include <BlendInt/Gui/HBox.hpp>

namespace BlendInt {

	HBox::HBox (int align, int space)
	: AbstractDequeContainer(), m_alignment(align), m_space(space)
	{
		set_size (200, 200);
	}

	HBox::~HBox ()
	{
	}

	bool HBox::PushBack (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			FillSubWidgetsInHBox(position(), size(), margin(), m_alignment, m_space);
			return true;
		}

		return false;
	}

	bool HBox::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {
			FillSubWidgetsInHBox(position(), size(), margin(), m_alignment, m_space);
			return true;
		}

		return false;
	}

	void HBox::SetAlignment (int align)
	{
		if(m_alignment == align) return;

		m_alignment = align;
		FillSubWidgetsInHBox(position(), size(), margin(), align, m_space);
	}

	void HBox::SetSpace (int space)
	{
		if(m_space == space) return;

		m_space = space;
		FillSubWidgetsInHBox(position(), size(), margin(), m_alignment, m_space);
	}

	Size BlendInt::HBox::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {

			preferred_size.set_width(200);
			preferred_size.set_height(200);

		} else {

			AbstractWidget* widget = 0;
			Size tmp_size;

			preferred_size.set_width(-m_space);
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				widget = *it;

				if(widget->visiable()) {
					tmp_size = widget->GetPreferredSize();

					preferred_size.add_width(tmp_size.width() + m_space);
					preferred_size.set_height(std::max(preferred_size.height(), tmp_size.height()));
				}
			}

			preferred_size.add_width(margin().left() + margin().right());
			preferred_size.add_height(margin().top() + margin().bottom());
		}

		return preferred_size;
	}

	bool HBox::IsExpandX () const
	{
		bool expand = false;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool HBox::IsExpandY () const
	{
		bool expand = false;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	void HBox::UpdateContainer(const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerMargin: {
				const Margin* margin_p = static_cast<const Margin*>(request.data());
				FillSubWidgetsInHBox(position(), size(), *margin_p, m_alignment, m_space);
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

	bool HBox::UpdateGeometryTest (const WidgetUpdateRequest& request)
	{
		if(request.source() == this) {

			return AbstractDequeContainer::UpdateGeometryTest(request);

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

	void HBox::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(request.source() == this || request.source() == container()) {

			switch (request.type()) {

				case WidgetPosition: {
					const Point* new_pos = static_cast<const Point*>(request.data());
					int x = new_pos->x() - position().x();
					int y = new_pos->y() - position().y();
					MoveSubWidgets(x, y);
					break;
				}

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					FillSubWidgetsInHBox(position(), *size_p, margin(), m_alignment, m_space);
					break;
				}

				default: {
					break;
				}
			}

		}
	}

	ResponseType HBox::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType HBox::CursorEnterEvent (bool entered)
	{
		return IgnoreAndContinue;
	}

	ResponseType HBox::KeyPressEvent (const KeyEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType HBox::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType HBox::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType HBox::MousePressEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType HBox::MouseReleaseEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType HBox::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

}
