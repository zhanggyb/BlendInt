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

#include <BlendInt/Gui/VerticalLayout.hpp>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

	VerticalLayout::VerticalLayout (int align)
			: AbstractLayout()
	{
		set_alignment(align);
	}

	VerticalLayout::VerticalLayout (int align, AbstractWidget* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	VerticalLayout::~VerticalLayout ()
	{
		/*
		for(std::vector<AbstractWidget*>::iterator it = m_items.begin(); it != m_items.end(); it++)
		{
			Destroy(*it);
		}
		*/
		m_items.clear();
	}

	void VerticalLayout::Update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				const Point* new_pos = static_cast<const Point*>(data);

				for (size_t i = 0; i < m_items.size(); i++)
				{
					SetPosition(m_items[i].get(),
							m_items[i]->position().x() + (new_pos->x() - position().x()),
							m_items[i]->position().y() + (new_pos->y() - position().y()));
				}
				break;
			}

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				if(m_items.size())
					MakeLayout(size_p, &margin(), space());
				break;
			}

			case LayoutPropertyMargin: {
				const Margin* margin_p = static_cast<const Margin*>(data);
				if(m_items.size())
					MakeLayout(&size(), margin_p, space());
				break;
			}

			case LayoutPropertySpace: {
				const int* space_p = static_cast<const int*>(data);
				if(m_items.size())
					MakeLayout(&size(), &margin(), *space_p);
				break;
			}

			default: {
				break;
			}
		}
	}

	void VerticalLayout::Draw ()
	{
#ifdef DEBUG
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

#endif
	}

	void VerticalLayout::AddItem (AbstractWidget* object)
	{
		// don't fire events when adding a widget into a layout
		object->deactivate_events();
		deactivate_events();

		Size min = minimal_size();
		Size preferred = preferred_size();
		Size current_size = size();

		unsigned int w_plus = margin().left() + margin().right();

		if (m_items.size() == 0) {
			min.add_height(object->minimal_size().height());
			preferred.add_height(object->preferred_size().height());
		} else {
			min.add_height(object->minimal_size().height() + space());
			preferred.add_height(object->preferred_size().height() + space());
		}

		min.set_width(
		        std::max(min.width(), object->minimal_size().width() + w_plus));
		preferred.set_width(
		        std::max(preferred.width(),
		                object->preferred_size().width() + w_plus));

		if (current_size.width() < preferred.width()) {
			current_size.set_width(preferred.width());
		}
		if (current_size.height() < preferred.height()) {
			current_size.set_height(preferred.height());
		}

		RefPtr<AbstractWidget> obj(object);
		m_items.push_back(obj);

		SetPreferredSize(preferred);
		SetMinimalSize(min);

		if(object->expand_y()) m_expandable_items.insert(object);
		else m_fixed_items.insert(object);

		if(! (current_size == size()))
			Resize(this, current_size);	// call make_layout() through this function
		else
			MakeLayout(&current_size, &margin(), space());

		activate_events();
		object->activate_events();

		AddChild(object);
		LockGeometry(object, true);
	}

	void VerticalLayout::RemoveItem(AbstractWidget * object)
	{
		deactivate_events();
		RefPtr<AbstractWidget> obj(object);

		std::vector<AbstractWidgetPtr>::iterator it;
		for(it = m_items.begin(); it != m_items.end();)
		{
			if ((*it) == obj) {
				it = m_items.erase(it);
			} else {
				it++;
			}
		}

		if(object->expand_y())
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
		RemoveChild(object);
		Destroy(object);
	}

	void VerticalLayout::MakeLayout(const Size* size, const Margin* margin, int space)
	{
		if (size->height() == preferred_size().height()) {
			DistributeWithPreferredHeight(size, margin, space);			// layout along y with preferred size
		} else if (size->height() < preferred_size().height()) {
			DistributeWithSmallHeight(size, margin, space);			// layout along y with small size
		} else {
			DistributeWithLargeHeight(size, margin, space);			// layout along y with large size
		}

		Align(size, margin);
	}

	void VerticalLayout::DistributeWithPreferredHeight(const Size* size, const Margin* margin, int space)
	{
		int y = position().y() + size->height() - margin->top();

		for(std::vector<AbstractWidgetPtr>::iterator it = m_items.begin(); it != m_items.end(); it++)
		{
			Resize((*it).get(), (*it)->size().width(), (*it)->preferred_size().height());
		}

		Distribute(space, y);
	}

	void VerticalLayout::DistributeWithSmallHeight(const Size* size, const Margin* margin, int space)
	{
		unsigned int min_expd_height = GetAllMinimalExpandableHeight();
		unsigned int fixed_height = GetAllFixedHeight();
		unsigned int current_height = size->height();
		unsigned int margin_plus = margin->top() + margin->bottom();

		std::vector<AbstractWidgetPtr>::iterator it;
		AbstractWidget* child = 0;

		bool change_expd_items = (current_height - margin_plus) >=
						(min_expd_height + fixed_height + (m_items.size() - 1) * space);


		if(change_expd_items) {

			if (m_expandable_items.size()) {
				unsigned int average_expd_height = current_height - margin_plus
				        - fixed_height - (m_items.size() - 1) * space;
				average_expd_height = average_expd_height / m_expandable_items.size();

				for (it = m_items.begin(); it != m_items.end(); it++) {
					child = (*it).get();

					if (m_expandable_items.count(child)) {
						Resize(child, child->size().width(), average_expd_height);
					} else {
						Resize(child, child->size().width(),
						        child->preferred_size().height());
					}
				}
			}

		} else {

			if (m_fixed_items.size()) {

				std::list<AbstractWidget*> unminimized_items;
				unsigned int height_plus = 0;

				unsigned int total_fixed_height = current_height - margin_plus
				        - min_expd_height - (m_items.size() - 1) * space;
				unsigned int average_fixed_height = total_fixed_height / m_fixed_items.size();

				for (it = m_items.begin(); it != m_items.end(); it++) {
					child = (*it).get();

					if (m_expandable_items.count(child)) {
						Resize(child, child->size().width(),
						        child->minimal_size().height());
					} else {

						if (average_fixed_height < child->minimal_size().height()) {
							height_plus = height_plus + child->minimal_size().height() - average_fixed_height;
							Resize(child, child->size().width(),
							        child->minimal_size().height());
						} else {
							unminimized_items.push_back(child);
							Resize(child, child->size().width(), average_fixed_height);
						}

					}

				}
				if(height_plus > 0) {
					AdjustMinimalHeight(&unminimized_items, height_plus);
				}
			}

		}

		int y = position().y() + size->height() - margin->top();
		Distribute(space, y);
	}

	void VerticalLayout::DistributeWithLargeHeight(const Size* size, const Margin* margin, int space)
	{
		unsigned int fixed_height = GetAllFixedHeight();
		unsigned int current_height = size->height();
		unsigned int margin_plus = margin->top() + margin->bottom();

		std::vector<AbstractWidgetPtr>::iterator it;
		AbstractWidget* child = 0;

		if(m_expandable_items.size()) {

			unsigned int max_expd_height = GetAllMaximalExpandableHeight();
			unsigned int total_expd_height = current_height - margin_plus - fixed_height - (m_items.size() - 1) * space;
			unsigned int average_expd_height = total_expd_height / m_expandable_items.size();

			bool change_expd_items = (current_height - margin_plus) <= (max_expd_height + fixed_height
					+ (m_items.size() - 1) * space);

			if(change_expd_items) {
				std::list<AbstractWidget*> unmaximized_list;
				unsigned int height_plus = 0;
				int y = position().y() + size->height() - margin->top();

				for (it = m_items.begin(); it != m_items.end(); it++) {
					child = (*it).get();
					if(m_expandable_items.count(child)) {
						if(average_expd_height > child->maximal_size().height()) {
							height_plus = height_plus + average_expd_height - child->maximal_size().height();
							Resize(child, child->size().width(), child->maximal_size().height());
						} else {
							unmaximized_list.push_back(child);
							Resize(child, child->size().width(), average_expd_height);
						}
					}
				}

				if(height_plus > 0) {	// check the last item
					AdjustExpandableHeight(&unmaximized_list, height_plus);
				}

				Distribute(space, y);
			} else {
				int y = position().x() + size->height() - margin->top();

				y = y - (current_height - margin_plus - max_expd_height - fixed_height
		                - (m_items.size() - 1) * space) / 2;

				// resize all with the max size
				for(it = m_items.begin(); it != m_items.end(); it++)
				{
					child = (*it).get();

					if (m_expandable_items.count(child)) {
						Resize(child, child->size().width(), child->maximal_size().height());
					} else {
						Resize(child, child->size().width(), child->preferred_size().height());
					}
				}

				Distribute(space, y);
			}

		} else {
			int y = position().y() + size->height() - margin->top();

			// if no expandable items, center all items
			y = y - (current_height - margin_plus - fixed_height - (m_fixed_items.size() - 1) * space) / 2;

			// resize all with preferred width
			for(it = m_items.begin(); it != m_items.end(); it++)
			{
				child = (*it).get();
				Resize(child, child->preferred_size().width(),
					        child->size().height());
			}

			Distribute(space, y);
		}

	}

	void VerticalLayout::Distribute(int space, int start)
	{
		start += space;	// add one space to make sure no space if only 1 child in layout
		for(std::vector<AbstractWidgetPtr>::iterator it = m_items.begin(); it != m_items.end(); it++)
		{
			start -= space;

			start -= (*it)->size().height();
			SetPosition((*it).get(), (*it)->position().x(), start);
		}
	}

	void VerticalLayout::Align(const Size* size, const Margin* margin)
	{
		int x = position().x() + margin->left();

		unsigned int w = size->width() - margin->left() - margin->right();

		std::vector<AbstractWidgetPtr>::iterator it;
		AbstractWidget* child = 0;
		for(it = m_items.begin(); it != m_items.end(); it++)
		{
			child = (*it).get();

			if (child->expand_x() ||
					(child->size().width() > w)) {
				Resize(child, w, child->size().height());
				SetPosition(child, x, child->position().y());
			} else {

				if (alignment() & AlignLeft) {
					SetPosition(child, x, child->position().y());
				} else if (alignment() & AlignRight) {
					SetPosition(child, x + (w - child->size().width()), child->position().y());
				} else if (alignment() & AlignVerticalCenter) {
					SetPosition(child, x + (w - child->size().width()) / 2, child->position().y());
				}

			}
		}
	}

	unsigned int VerticalLayout::AdjustExpandableHeight(std::list<AbstractWidget*>* item_list_p, unsigned int height_plus)
	{
		if(!item_list_p) return height_plus;
		if(item_list_p->size() == 0) return height_plus;

		unsigned int remainder = 0;
		std::list<AbstractWidget*>::iterator it;
		unsigned int average_height_plus = height_plus / item_list_p->size();

		for(it = item_list_p->begin(); it != item_list_p->end(); it++)
		{
			if ((average_height_plus + (*it)->size().height()) > (*it)->maximal_size().height()) {
				Resize(*it, (*it)->size().width(), (*it)->maximal_size().height());
				remainder = remainder + average_height_plus + (*it)->size().height() - (*it)->maximal_size().height();
				it = item_list_p->erase(it);
			} else {
				Resize(*it, (*it)->size().width(), (*it)->size().height() + average_height_plus);
			}
		}

		if(remainder != 0) {
			// TODO: do not use iteration procedure
			remainder = AdjustExpandableHeight(item_list_p, remainder);
		}

		return remainder;
	}

	unsigned int VerticalLayout::AdjustMinimalHeight(std::list<AbstractWidget*>* item_list_p, unsigned int height_plus)
	{
		if(!item_list_p) return height_plus;
		if(item_list_p->size() == 0) return height_plus;

		unsigned int remainder = 0;
		std::list<AbstractWidget*>::iterator it;
		unsigned int average_height_plus = height_plus / item_list_p->size();

		for(it = item_list_p->begin(); it != item_list_p->end(); it++)
		{
			if (((*it)->size().height() - average_height_plus) < (*it)->minimal_size().height()) {
				Resize(*it, (*it)->size().width(), (*it)->minimal_size().height());
				remainder = remainder + (*it)->minimal_size().height() - ((*it)->size().height() - average_height_plus);
				it = item_list_p->erase(it);
			} else {
				Resize(*it, (*it)->size().width(), (*it)->size().height() - average_height_plus);
			}
		}

		if(remainder != 0) {
			// TODO: do not use iteration procedure
			remainder = AdjustMinimalHeight(item_list_p, remainder);
		}

		return remainder;
	}

	unsigned int VerticalLayout::GetAllMinimalExpandableHeight()
	{
		unsigned int height = 0;

		for(std::set<AbstractWidget*>::iterator it = m_expandable_items.begin(); it != m_expandable_items.end(); it++)
		{
			height += (*it)->minimal_size().height();
		}

		return height;
	}

	unsigned int VerticalLayout::GetAllMaximalExpandableHeight()
	{
		unsigned int height = 0;

		for(std::set<AbstractWidget*>::iterator it = m_expandable_items.begin(); it != m_expandable_items.end(); it++)
		{
			height += (*it)->maximal_size().height();
		}

		return height;
	}

	unsigned int VerticalLayout::GetAllFixedHeight()
	{
		unsigned int height = 0;

		for(std::set<AbstractWidget*>::iterator it = m_fixed_items.begin(); it != m_fixed_items.end(); it++)
		{
			height += (*it)->size().height();
		}

		return height;
	}

	void VerticalLayout::GetSizeHint (bool count_margin,
										bool count_space,
										Size* size,
										Size* min,
										Size* preferred)
	{
		Size size_out;
		Size min_size_out;
		Size preferred_size_out;

		AbstractWidget* child;
		std::vector<AbstractWidgetPtr>::reverse_iterator it;

		if(count_margin) {
			size_out.set_height(margin().top());
			min_size_out.set_height(margin().top());
			preferred_size_out.set_height(margin().top());
		}

		for(it = m_items.rbegin(); it != m_items.rend(); it++)
		{
			child = (*it).get();

			size_out.set_width(std::max(size_out.width(), child->size().width()));
			size_out.add_height(child->size().height());

			min_size_out.set_width(std::max(min_size_out.width(), child->minimal_size().width()));
			min_size_out.add_height(child->minimal_size().height());

			preferred_size_out.set_width(std::max(preferred_size_out.width(), child->preferred_size().width()));
			preferred_size_out.add_height(child->preferred_size().height());
		}

		if(count_margin) {
			size_out.add_height(margin().bottom());
			size_out.add_width(margin().left() + margin().right());

			min_size_out.add_height(margin().bottom());
			min_size_out.add_width(margin().left() + margin().right());

			preferred_size_out.add_height(margin().bottom());
			preferred_size_out.add_width(margin().left() + margin().right());
		}

		if(count_space) {
			if(m_items.size()) {
				size_out.add_height((m_items.size() - 1) * space());
				min_size_out.add_height((m_items.size() - 1) * space());
				preferred_size_out.add_height((m_items.size() - 1) * space());
			}
		}

		if(size) *size = size_out;
		if(min) *min = min_size_out;
		if(preferred) *preferred = preferred_size_out;
	}

}
