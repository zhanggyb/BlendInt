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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <algorithm>

#include <queue>
#include <iostream>

#include <BlendInt/HorizontalLayout.hpp>

namespace BlendInt {

	HorizontalLayout::HorizontalLayout (int align)
			: AbstractLayout()
	{
		set_alignment(align);
	}

	HorizontalLayout::HorizontalLayout (int align, AbstractWidget* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	HorizontalLayout::~HorizontalLayout ()
	{

	}

	void HorizontalLayout::Update (int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				if(items().size())
					MakeLayout(size_p, &margin(), space());
				return;
			}

			case LayoutPropertyMargin: {
				const Margin* margin_p = static_cast<const Margin*>(data);
				if(items().size())
					MakeLayout(&size(), margin_p, space());
				return;
			}

			case LayoutPropertySpace: {
				const int* space_p = static_cast<const int*>(data);
				if(items().size())
					MakeLayout(&size(), &margin(), *space_p);
				return;
			}

			default: {
				AbstractLayout::Update (type, data);
				break;
			}
		}
	}

	void HorizontalLayout::Draw ()
	{
		std::vector<AbstractWidget*>::const_iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			DispatchRender(*it);
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(size().width(), 0);
		glVertex2i(size().width(), size().height());
		glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		glPopMatrix();
#endif
	}

	void HorizontalLayout::KeyPressEvent (KeyEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_key_press_event(*it, event);
		}
	}

	void HorizontalLayout::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void HorizontalLayout::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void HorizontalLayout::MousePressEvent (MouseEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_mouse_press_event(*it, event);
		}
	}

	void HorizontalLayout::MouseReleaseEvent (MouseEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_mouse_release_event(*it, event);
		}
	}

	void HorizontalLayout::MouseMoveEvent (MouseEvent* event)
	{
		std::vector<AbstractWidget*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			dispatch_mouse_move_event(*it, event);
		}
	}

	void HorizontalLayout::AddItem (AbstractWidget* object)
	{
		// don't fire events when adding a widget into a layout
		object->deactivate_events();
		deactivate_events();

		Size min_size = minimal_size();
		Size prefer_size = preferred_size();
		Size current_size = size();
		// TODO: count max size

		unsigned int h_plus = margin().top() + margin().bottom();

		if (items().size() == 0) {
			min_size.add_width(object->minimal_size().width());
			prefer_size.add_width(object->preferred_size().width());
		} else {
			min_size.add_width(object->minimal_size().width() + space());
			prefer_size.add_width(object->preferred_size().width() + space());
		}

		min_size.set_height(
		        std::max(min_size.height(), object->minimal_size().height() + h_plus));
		prefer_size.set_height(
		        std::max(prefer_size.height(),
		                object->preferred_size().height() + h_plus));

		if (current_size.width() < prefer_size.width()) {
			current_size.set_width(prefer_size.width());
		}
		if (current_size.height() < prefer_size.height()) {
			current_size.set_height(prefer_size.height());
		}

		items().push_back(object);

		SetPreferredSize(prefer_size);
		SetMinimalSize(min_size);

		if(object->expand_x()) m_expandable_items.insert(object);
		else m_fixed_items.insert(object);

		if( !(current_size == size()) )
			Resize(this, current_size);
		else
			MakeLayout(&current_size, &margin(), space());

		activate_events();
		object->activate_events();

		bind(object);
		LockGeometry(object, true);
	}

	void HorizontalLayout::RemoveItem (AbstractWidget* object)
	{
		deactivate_events();

		std::vector<AbstractWidget*>::iterator it;
		for(it = items().begin(); it != items().end();)
		{
			if ((*it) == object) {
				it = items().erase(it);
			} else {
				it++;
			}
		}

		if(object->expand_x())
				m_expandable_items.erase(object);
		else
			m_fixed_items.erase(object);

		Size new_preferred_size;
		Size new_minimal_size;

		GetSizeHint(true, true, 0, &new_minimal_size, &new_preferred_size);

		SetMinimalSize(new_minimal_size);
		SetPreferredSize(new_preferred_size);

		MakeLayout(&size(), &margin(), space());

		activate_events();

		LockGeometry(object, false);
		unbind(object);
	}

	void HorizontalLayout::MakeLayout (const Size* size, const Margin* margin,
	        int space)
	{
		if (size->width() == preferred_size().width()) {
			DistributeWithPreferredWidth(margin, space);			// layout along x with preferred size
		} else if (size->width() < preferred_size().width()) {
			DistributeWithSmallWidth(size, margin, space);			// layout along x with small size
		} else {
			DistributeWithLargeWidth(size, margin, space);			// layout along x with large size
		}

		Align (size, margin);
	}

	void HorizontalLayout::DistributeWithPreferredWidth(const Margin* margin, int space)
	{
		for(std::vector<AbstractWidget*>::iterator it = items().begin(); it != items().end(); it++)
		{
			Resize(*it, (*it)->preferred_size().width(), (*it)->size().height());
		}

		int x = position().x() + margin->left();
		Distribute(space, x);
	}

	void HorizontalLayout::DistributeWithSmallWidth(const Size* size, const Margin* margin, int space)
	{
		unsigned int min_expd_width = GetAllMinimalExpandableWidth();
		unsigned int fixed_width = GetAllFixedWidth();
		unsigned int current_width = size->width();
		unsigned int margin_plus = margin->left() + margin->right();

		std::vector<AbstractWidget*>::iterator it;
		AbstractWidget* child = 0;

		bool change_expd_items = (current_width - margin_plus) >= (min_expd_width + fixed_width + (items().size() - 1) * space);

		if(change_expd_items) {

			// just change expandable widgets
			if (m_expandable_items.size()) {

				unsigned int average_expd_width = current_width - margin_plus
				        - fixed_width - (items().size() - 1) * space;
				average_expd_width = average_expd_width / m_expandable_items.size();

				for (it = items().begin(); it != items().end(); it++) {
					child = *it;

					if (m_expandable_items.count(child)) {
						Resize(child, average_expd_width, child->size().height());
					} else {
						Resize(child, child->preferred_size().width(),
						        child->size().height());
					}
				}

			}

		} else {

			if(m_fixed_items.size()) {

				std::list<AbstractWidget*> unminimized_items;
				unsigned int width_plus = 0;

				unsigned int total_fixed_width = current_width - margin_plus
				        - min_expd_width - (items().size() - 1) * space;
				unsigned int average_fixed_width = total_fixed_width / m_fixed_items.size();

				for (it = items().begin(); it != items().end(); it++) {

					child = *it;

					if (m_expandable_items.count(child)) {
						Resize(child, child->minimal_size().width(),
						        child->size().height());
					} else {
						if (average_fixed_width
								< child->minimal_size().width()) {
							width_plus = width_plus + child->minimal_size().width() - average_fixed_width;
							Resize(child, child->minimal_size().width(),
									child->size().height());
						} else {
							unminimized_items.push_back(child);
							Resize(child, average_fixed_width,
									child->size().height());
						}
					}
				}

				if(width_plus > 0) {
					AdjustMinimalWidth(&unminimized_items, width_plus);
				}

			}
		}

		int x = position().x() + margin->left();	// the x position of each child widget, update in each for loop
		Distribute(space, x);
	}

	void HorizontalLayout::DistributeWithLargeWidth(const Size* size, const Margin* margin, int space)
	{
		unsigned int fixed_width = GetAllFixedWidth();
		unsigned int current_width = size->width();
		unsigned int margin_plus = margin->left() + margin->right();

		std::vector<AbstractWidget*>::iterator it;
		AbstractWidget* child = 0;

		if(m_expandable_items.size()) {

			unsigned int max_expd_width = GetAllMaximalExpandableWidth();
			unsigned int total_expd_width = current_width - margin_plus
			        - fixed_width - (items().size() - 1) * space;
			unsigned int average_expd_width = total_expd_width / m_expandable_items.size();

			bool change_expd_items = (current_width - margin_plus)
			        <= (max_expd_width + fixed_width
			                + (items().size() - 1) * space);

			if(change_expd_items) {

				std::list<AbstractWidget*> unmaximized_list;
				unsigned int width_plus = 0;
				int x = position().x() + margin->left();

				for (it = items().begin(); it != items().end(); it++) {
					child = *it;
					if(m_expandable_items.count(child)) {
						if(average_expd_width > child->maximal_size().width()) {
							width_plus = width_plus + average_expd_width - child->maximal_size().width();
							Resize(child, child->maximal_size().width(), child->size().height());
						} else {
							unmaximized_list.push_back(child);
							Resize(child, average_expd_width, child->size().height());
						}
					}
				}

				if(width_plus > 0) {	// check the last item
					AdjustExpandableWidth(&unmaximized_list, width_plus);
				}

				Distribute(space, x);

			} else {
				int x = position().x() + margin->left();

				x = x + (current_width - margin_plus - max_expd_width - fixed_width
		                - (items().size() - 1) * space) / 2;

				// resize all with the max size
				for(it = items().begin(); it != items().end(); it++)
				{
					child = *it;

					if (m_expandable_items.count(child)) {
						Resize(child, child->maximal_size().width(), child->size().height());
					} else {
						Resize(child, child->preferred_size().width(),
						        child->size().height());
					}
				}

				Distribute(space, x);
			}

		} else {
			int x = position().x() + margin->left();

			// if no expandable items, center all items
			x = x + (current_width - margin_plus - fixed_width - (m_fixed_items.size() - 1) * space) / 2;

			// resize all with preferred width
			for(it = items().begin(); it != items().end(); it++)
			{
				child = *it;
				Resize(child, child->preferred_size().width(),
					        child->size().height());
			}

			Distribute(space, x);
		}

	}

	void HorizontalLayout::Distribute(int space, int start)
	{
		start -= space;	// subtract one space to make sure no space if only 1 child in layout
		for(std::vector<AbstractWidget*>::iterator it = items().begin(); it != items().end(); it++)
		{
			start += space;

			SetPosition(*it, start, (*it)->position().y());
			start += (*it)->size().width();
		}
	}

	void HorizontalLayout::Align(const Size* size, const Margin* margin)
	{
		int y = position().y() + margin->bottom();

		unsigned int h = size->height() - margin->top() - margin->bottom();

		std::vector<AbstractWidget*>::iterator it;
		AbstractWidget* child = 0;
		for(it = items().begin(); it != items().end(); it++)
		{
			child = *it;

			if (child->expand_y() ||
					(child->size().height() > h)) {
				Resize(child, child->size().width(), h);
				SetPosition(child, child->position().x(), y);
			} else {

				if (alignment() & AlignTop) {
					SetPosition(child, child->position().x(),
					        y + (h - child->size().height()));
				} else if (alignment() & AlignBottom) {
					SetPosition(child, child->position().x(), y);
				} else if (alignment() & AlignHorizontalCenter) {
					SetPosition(child, child->position().x(),
					        y + (h - child->size().height()) / 2);
				}

			}
		}
	}

	unsigned int HorizontalLayout::AdjustExpandableWidth(std::list<AbstractWidget*>* item_list_p, unsigned int width_plus)
	{
		if(!item_list_p) return width_plus;
		if(item_list_p->size() == 0) return width_plus;

		unsigned int remainder = 0;
		std::list<AbstractWidget*>::iterator it;
		unsigned int average_width_plus = width_plus / item_list_p->size();

		for(it = item_list_p->begin(); it != item_list_p->end(); it++)
		{
			if ((average_width_plus + (*it)->size().width()) > (*it)->maximal_size().width()) {
				Resize(*it, (*it)->maximal_size().width(), (*it)->size().height());
				remainder = remainder + average_width_plus + (*it)->size().width() - (*it)->maximal_size().width();
				it = item_list_p->erase(it);
			} else {
				Resize(*it, (*it)->size().width() + average_width_plus, (*it)->size().height());
			}
		}

		if(remainder != 0) {
			// TODO: do not use iteration procedure
			remainder = AdjustExpandableWidth(item_list_p, remainder);
		}

		return remainder;
	}

	unsigned int HorizontalLayout::AdjustMinimalWidth(std::list<AbstractWidget*>* item_list_p, unsigned int width_plus)
	{
		if(!item_list_p) return width_plus;
		if(item_list_p->size() == 0) return width_plus;

		unsigned int remainder = 0;
		std::list<AbstractWidget*>::iterator it;
		unsigned int average_width_plus = width_plus / item_list_p->size();

		for(it = item_list_p->begin(); it != item_list_p->end(); it++)
		{
			if (((*it)->size().width() - average_width_plus) < (*it)->minimal_size().width()) {
				Resize(*it, (*it)->minimal_size().width(), (*it)->size().height());
				remainder = remainder + (*it)->minimal_size().width() - ((*it)->size().width() - average_width_plus);
				it = item_list_p->erase(it);
			} else {
				Resize(*it, (*it)->size().width() - average_width_plus, (*it)->size().height());
			}
		}

		if(remainder != 0) {
			// TODO: do not use iteration procedure
			remainder = AdjustMinimalWidth(item_list_p, remainder);
		}

		return remainder;
	}

	unsigned int HorizontalLayout::GetAllMinimalExpandableWidth()
	{
		unsigned int width = 0;

		for(std::set<AbstractWidget*>::iterator it = m_expandable_items.begin(); it != m_expandable_items.end(); it++)
		{
			width += (*it)->minimal_size().width();
		}

		return width;
	}

	unsigned int HorizontalLayout::GetAllMaximalExpandableWidth()
	{
		unsigned int width = 0;

		for(std::set<AbstractWidget*>::iterator it = m_expandable_items.begin(); it != m_expandable_items.end(); it++)
		{
			width += (*it)->maximal_size().width();
		}

		return width;
	}

	unsigned int HorizontalLayout::GetAllFixedWidth()
	{
		unsigned int width = 0;

		for(std::set<AbstractWidget*>::iterator it = m_fixed_items.begin(); it != m_fixed_items.end(); it++)
		{
			width += (*it)->size().width();
		}

		return width;
	}

	void HorizontalLayout::GetSizeHint (bool count_margin,
										  bool count_space,
										  Size* size,
										  Size* min,
										  Size* preferred)
	{
		Size size_out;
		Size min_size_out;
		Size preferred_size_out;

		AbstractWidget* child;
		std::vector<AbstractWidget*>::iterator it;

		if(count_margin) {
			size_out.set_width(margin().left());
			min_size_out.set_width(margin().left());
			preferred_size_out.set_width(margin().left());
		}

		for(it = items().begin(); it != items().end(); it++)
		{
			child = *it;
			size_out.add_width(child->size().width());
			size_out.set_height(std::max(size_out.height(),
					child->size().height()));
			min_size_out.add_width(child->minimal_size().width());
			min_size_out.set_height(std::max(min_size_out.height(),
					child->minimal_size().height()));
			preferred_size_out.add_width(child->preferred_size().width());
			preferred_size_out.set_height(std::max(preferred_size_out.height(),
					child->size().height()));
		}

		if(count_margin) {
			size_out.add_width(margin().right());
			size_out.add_height(margin().top() + margin().bottom());

			min_size_out.add_width(margin().right());
			min_size_out.add_height(margin().top() + margin().bottom());

			preferred_size_out.add_width(margin().right());
			preferred_size_out.add_height(margin().top() + margin().bottom());
		}

		if(count_space) {
			if(items().size()) {
				size_out.add_width((items().size() - 1) * space());
				min_size_out.add_width((items().size() - 1) * space());
				preferred_size_out.add_width((items().size() - 1) * space());
			}
		}

		if(size) *size = size_out;
		if(min) *min = min_size_out;
		if(preferred) *preferred = preferred_size_out;
	}

}

