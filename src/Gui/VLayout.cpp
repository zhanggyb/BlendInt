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

#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	VLayoutExt::VLayoutExt (Context* context)
	: AbstractLayoutExt(context),
	  m_alignment(AlignCenter)
	{

	}

	VLayoutExt::~VLayoutExt ()
	{
	}

	bool VLayoutExt::AddLayoutItem(AbstractWidget* widget)
	{
		if(!widget) return false;

		WidgetDeque::iterator it = std::find(m_widgets.begin(), m_widgets.end(), widget);
		if(it == m_widgets.end()) {
			m_widgets.push_back(widget);
			return true;
		}

		return false;
	}

	bool VLayoutExt::RemoveLayoutItem(AbstractWidget* widget)
	{
		if(!widget) return false;

		WidgetDeque::iterator it = std::find(m_widgets.begin(), m_widgets.end(), widget);
		if(it != m_widgets.end()) {
			m_widgets.erase(it);
			return true;
		}

		return false;
	}

	void VLayoutExt::Fill ()
	{
		if(m_widgets.empty()) return;

		int x = context()->margin().left();
		int y = context()->size().height() - context()->margin().top();

		unsigned int width = context()->size().width() - context()->margin().left() - context()->margin().right();
		unsigned int height = context()->size().height() - context()->margin().top() - context()->margin().bottom();

		Size preferred_size;
		AbstractWidget* widget = 0;

		std::set<AbstractWidget*> y_unexpandable_widgets;

		for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
		{
			widget = *it;
			if(!widget->expand_y()) {
				y_unexpandable_widgets.insert(widget);
			}
		}

		if(y_unexpandable_widgets.empty()) {	// all expandable along y

			unsigned int average_height = height / m_widgets.size();
			for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
			{
				widget = *it;
				y -= average_height;
				widget->SetPosition(widget->position().x(), y);
				widget->Resize(widget->size().width(), average_height);
				y -= 2;	// space
			}

		} else {

			unsigned int total_fixed_height = 0;

			for(std::set<AbstractWidget*>::iterator it = y_unexpandable_widgets.begin(); it != y_unexpandable_widgets.end(); it++)
			{
				widget = *it;
				preferred_size = widget->GetPreferredSize();
				total_fixed_height += preferred_size.height();
			}

			if(total_fixed_height < height) {

				unsigned int tmp;

				unsigned int averate_expand_height = 0;
				if(y_unexpandable_widgets.size() < m_widgets.size()) {
					averate_expand_height = (height - total_fixed_height) / (m_widgets.size() - y_unexpandable_widgets.size());
				}

				for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
				{
					widget = *it;
					if(!widget->expand_y()) {
						tmp = widget->GetPreferredSize().height();
						y -= tmp;
						widget->SetPosition(widget->position().x(), y);
						widget->Resize(widget->size().width(), tmp);
						y -= 2; // space
					} else {
						y -= averate_expand_height;
						widget->SetPosition(widget->position().x(), y);
						widget->Resize(widget->size().width(), averate_expand_height);
						y -= 2;	// space
					}
				}

			}

		}

		/*
		for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
		{
			widget = *it;
			preferred_size = widget->GetPreferredSize();
			y -= preferred_size.height();
			widget->SetPosition(x, y);
			widget->Resize(width, preferred_size.height());
			y -= 2;	// space
		}
		*/
		Align();
	}

	void VLayoutExt::Align ()
	{
		int x = context()->margin().left();
		int y = context()->size().height() - context()->margin().top();

		unsigned int width = context()->size().width() - context()->margin().left() - context()->margin().right();
		unsigned int height = context()->size().height() - context()->margin().top() - context()->margin().bottom();

		AbstractWidget* widget = 0;
		for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
		{
			widget = *it;

			if (widget->expand_x()) {
				widget->Resize(width, widget->size().height());
				widget->SetPosition(x, widget->position().y());
			} else {

				if (m_alignment & AlignLeft) {
					widget->SetPosition(x, widget->position().y());
				} else if (m_alignment & AlignRight) {
					widget->SetPosition(x + (width - widget->size().width()), widget->position().y());
				} else if (m_alignment & AlignVerticalCenter) {
					widget->SetPosition(x + (width - widget->size().width()) / 2, widget->position().y());
				}

			}
		}
	}


	// -------------------------------------------------

	VLayout::VLayout (int align)
			: AbstractLayout()
	{
		set_alignment(align);
	}

	VLayout::~VLayout ()
	{
	}

	void VLayout::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormPosition: {
					const Point* new_pos = static_cast<const Point*>(request.data());
					int x = new_pos->x() - position().x();
					int y = new_pos->y() - position().y();

					MoveSubWidgets(x, y);
					break;
				}

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					if(sub_widget_size())
						MakeLayout(size_p, &margin(), space());

					break;
				}

				case ContainerMargin: {
					const Margin* margin_p = static_cast<const Margin*>(request.data());
					if(sub_widget_size())
						MakeLayout(&size(), margin_p, space());
					break;
				}

				case LayoutPropertySpace: {
					const int* space_p = static_cast<const int*>(request.data());
					if(sub_widget_size())
						MakeLayout(&size(), &margin(), *space_p);
					break;
				}

				case WidgetRefresh: {
					Refresh();
					break;
				}

				default: {
					break;
				}
			}

		}
	}

	ResponseType VLayout::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	void VLayout::AddItem (AbstractWidget* object)
	{
		/*
		// don't fire events when adding a widget into a layout
		object->deactivate_events();
		deactivate_events();

		Size min_size = minimal_size();
		Size prefer_size = preferred_size();
		Size current_size = size();

		unsigned int w_plus = margin().left() + margin().right();

		if (sub_widget_size() == 0) {
			min_size.add_height(object->minimal_size().height());
			prefer_size.add_height(object->preferred_size().height());
		} else {
			min_size.add_height(object->minimal_size().height() + space());
			prefer_size.add_height(object->preferred_size().height() + space());
		}

		min_size.set_width(
		        std::max(min_size.width(), object->minimal_size().width() + w_plus));
		prefer_size.set_width(
		        std::max(prefer_size.width(),
		                object->preferred_size().width() + w_plus));

		if (current_size.width() < prefer_size.width()) {
			current_size.set_width(prefer_size.width());
		}
		if (current_size.height() < prefer_size.height()) {
			current_size.set_height(prefer_size.height());
		}

		SetPreferredSize(prefer_size);
		SetMinimalSize(min_size);

		if(object->expand_y()) {
			set_expand_y(true);
		}

		AppendSubWidget(object);

		if(! (current_size == size()))
			Resize(current_size);	// call make_layout() through this function
		else
			MakeLayout(&current_size, &margin(), space());

		activate_events();
		object->activate_events();
		*/
	}

	void VLayout::RemoveItem(AbstractWidget * object)
	{
		deactivate_events();

		if(!RemoveSubWidget(object)) return;

		Size new_preferred_size;
		Size new_minimal_size;

		GetSizeHint(true, true, 0, &new_minimal_size, &new_preferred_size);

		//SetMinimalSize(new_minimal_size);
		//SetPreferredSize(new_preferred_size);

		set_expand_y(false);
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->expand_y()) {
				set_expand_y(true);
				break;
			}
		}

		MakeLayout(&size(), &margin(), space());

		activate_events();

		RemoveSubWidget(object);
	}

	void VLayout::MakeLayout(const Size* size, const Margin* margin, int space)
	{
		/*
		if (size->height() == preferred_size().height()) {
			DistributeWithPreferredHeight(size, margin, space);			// layout along y with preferred size
		} else if (size->height() < preferred_size().height()) {
			DistributeWithSmallHeight(size, margin, space);			// layout along y with small size
		} else {
			DistributeWithLargeHeight(size, margin, space);			// layout along y with large size
		}

		Align(size, margin);
		*/
	}

	void VLayout::DistributeWithPreferredHeight(const Size* size, const Margin* margin, int space)
	{
		/*
		int y = position().y() + size->height() - margin->top();

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			ResizeSubWidget(*it, (*it)->size().width(), (*it)->preferred_size().height());
		}

		Distribute(space, y);
		*/
	}

	void VLayout::DistributeWithSmallHeight(const Size* size, const Margin* margin, int space)
	{
		/*
		unsigned int min_expd_height = GetAllMinimalExpandableHeight();
		unsigned int fixed_height = GetAllFixedHeight();
		unsigned int current_height = size->height();
		unsigned int margin_plus = margin->top() + margin->bottom();

		WidgetDeque::iterator it;
		AbstractWidget* child = 0;

		bool change_expd_items = (current_height - margin_plus) >=
						(min_expd_height + fixed_height + (sub_widget_size() - 1) * space);

		unsigned int exp_num = CountVExpandableNumber();

		if(change_expd_items) {

			if (exp_num) {
				unsigned int average_expd_height = current_height - margin_plus
				        - fixed_height - (sub_widget_size() - 1) * space;
				average_expd_height = average_expd_height / exp_num;

				for (it = sub_widgets()->begin(); it != sub_widgets()->end(); it++) {
					child = *it;

					if (child->expand_y()) {
						ResizeSubWidget(child, child->size().width(), average_expd_height);
					} else {
						ResizeSubWidget(child, child->size().width(),
						        child->preferred_size().height());
					}
				}
			}

		} else {

			unsigned int fixed_num = sub_widget_size() - exp_num;

			if (fixed_num) {

				std::list<AbstractWidget*> unminimized_items;
				unsigned int height_plus = 0;

				unsigned int total_fixed_height = current_height - margin_plus
				        - min_expd_height - (sub_widget_size() - 1) * space;
				unsigned int average_fixed_height = total_fixed_height / fixed_num;

				for (it = sub_widgets()->begin(); it != sub_widgets()->end(); it++) {
					child = *it;

					if (child->expand_y()) {
						ResizeSubWidget(child, child->size().width(),
						        child->minimal_size().height());
					} else {

						if (average_fixed_height < child->minimal_size().height()) {
							height_plus = height_plus + child->minimal_size().height() - average_fixed_height;
							ResizeSubWidget(child, child->size().width(),
							        child->minimal_size().height());
						} else {
							unminimized_items.push_back(child);
							ResizeSubWidget(child, child->size().width(), average_fixed_height);
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
		*/
	}

	void VLayout::DistributeWithLargeHeight(const Size* size, const Margin* margin, int space)
	{
		/*
		unsigned int fixed_height = GetAllFixedHeight();
		unsigned int current_height = size->height();
		unsigned int margin_plus = margin->top() + margin->bottom();

		WidgetDeque::iterator it;
		AbstractWidget* child = 0;

		unsigned int exp_num = CountVExpandableNumber();

		if(exp_num) {

			unsigned int max_expd_height = GetAllMaximalExpandableHeight();
			unsigned int total_expd_height = current_height - margin_plus - fixed_height - (sub_widget_size() - 1) * space;
			unsigned int average_expd_height = total_expd_height / exp_num;

			bool change_expd_items = (current_height - margin_plus) <= (max_expd_height + fixed_height
					+ (sub_widget_size() - 1) * space);

			if(change_expd_items) {
				std::list<AbstractWidget*> unmaximized_list;
				unsigned int height_plus = 0;
				int y = position().y() + size->height() - margin->top();

				for (it = sub_widgets()->begin(); it != sub_widgets()->end(); it++) {
					child = *it;
					if(child->expand_y()) {
						if(average_expd_height > child->maximal_size().height()) {
							height_plus = height_plus + average_expd_height - child->maximal_size().height();
							ResizeSubWidget(child, child->size().width(), child->maximal_size().height());
						} else {
							unmaximized_list.push_back(child);
							ResizeSubWidget(child, child->size().width(), average_expd_height);
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
		                - (sub_widget_size() - 1) * space) / 2;

				// resize all with the max size
				for(it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
				{
					child = *it;

					if (child->expand_y()) {
						ResizeSubWidget(child, child->size().width(), child->maximal_size().height());
					} else {
						ResizeSubWidget(child, child->size().width(), child->preferred_size().height());
					}
				}

				Distribute(space, y);
			}

		} else {
			int y = position().y() + size->height() - margin->top();

			// if no expandable items, center all items
			y = y - (current_height - margin_plus - fixed_height - (sub_widget_size() - exp_num - 1) * space) / 2;

			// resize all with preferred width
			for(it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				child = *it;
				ResizeSubWidget(child, child->preferred_size().width(),
					        child->size().height());
			}

			Distribute(space, y);
		}
	*/
	}

	void VLayout::Distribute(int space, int start)
	{
		start += space;	// add one space to make sure no space if only 1 child in layout
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			start -= space;

			start -= (*it)->size().height();
			SetSubWidgetPosition(*it, (*it)->position().x(), start);
		}
	}

	void VLayout::Align(const Size* size, const Margin* margin)
	{
		int x = position().x() + margin->left();

		unsigned int w = size->width() - margin->left() - margin->right();

		WidgetDeque::iterator it;
		AbstractWidget* child = 0;
		for(it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			child = *it;

			if (child->expand_x() ||
					(child->size().width() > w)) {
				ResizeSubWidget(child, w, child->size().height());
				SetSubWidgetPosition(child, x, child->position().y());
			} else {

				if (alignment() & AlignLeft) {
					SetSubWidgetPosition(child, x, child->position().y());
				} else if (alignment() & AlignRight) {
					SetSubWidgetPosition(child, x + (w - child->size().width()), child->position().y());
				} else if (alignment() & AlignVerticalCenter) {
					SetSubWidgetPosition(child, x + (w - child->size().width()) / 2, child->position().y());
				}

			}
		}
	}

	unsigned int VLayout::AdjustExpandableHeight(std::list<AbstractWidget*>* item_list_p, unsigned int height_plus)
	{
		if(!item_list_p) return height_plus;
		if(item_list_p->size() == 0) return height_plus;

		unsigned int remainder = 0;
		std::list<AbstractWidget*>::iterator it;
		unsigned int average_height_plus = height_plus / item_list_p->size();

		for(it = item_list_p->begin(); it != item_list_p->end(); it++)
		{
			/*
			if ((average_height_plus + (*it)->size().height()) > (*it)->maximal_size().height()) {
				ResizeSubWidget(*it, (*it)->size().width(), (*it)->maximal_size().height());
				remainder = remainder + average_height_plus + (*it)->size().height() - (*it)->maximal_size().height();
				it = item_list_p->erase(it);
			} else {
				ResizeSubWidget(*it, (*it)->size().width(), (*it)->size().height() + average_height_plus);
			}
			*/
		}

		if(remainder != 0) {
			// TODO: do not use iteration procedure
			remainder = AdjustExpandableHeight(item_list_p, remainder);
		}

		return remainder;
	}

	unsigned int VLayout::AdjustMinimalHeight(std::list<AbstractWidget*>* item_list_p, unsigned int height_plus)
	{
		if(!item_list_p) return height_plus;
		if(item_list_p->size() == 0) return height_plus;

		unsigned int remainder = 0;
		std::list<AbstractWidget*>::iterator it;
		unsigned int average_height_plus = height_plus / item_list_p->size();

		for(it = item_list_p->begin(); it != item_list_p->end(); it++)
		{
			/*
			if (((*it)->size().height() - average_height_plus) < (*it)->minimal_size().height()) {
				ResizeSubWidget(*it, (*it)->size().width(), (*it)->minimal_size().height());
				remainder = remainder + (*it)->minimal_size().height() - ((*it)->size().height() - average_height_plus);
				it = item_list_p->erase(it);
			} else {
				ResizeSubWidget(*it, (*it)->size().width(), (*it)->size().height() - average_height_plus);
			}
			*/
		}

		if(remainder != 0) {
			// TODO: do not use iteration procedure
			remainder = AdjustMinimalHeight(item_list_p, remainder);
		}

		return remainder;
	}

	unsigned int VLayout::GetAllMinimalExpandableHeight()
	{
		unsigned int height = 0;

		/*
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->expand_y())
				height += (*it)->minimal_size().height();
		}
		*/

		return height;
	}

	unsigned int VLayout::GetAllMaximalExpandableHeight()
	{
		unsigned int height = 0;

		/*
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->expand_y())
				height += (*it)->maximal_size().height();
		}
		*/

		return height;
	}

	unsigned int VLayout::GetAllFixedHeight()
	{
		unsigned int height = 0;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if(!(*it)->expand_y())
				height += (*it)->size().height();
		}

		return height;
	}
	
	unsigned int VLayout::CountVExpandableNumber ()
	{
		unsigned int num = 0;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->expand_y()) {
				num++;
			}
		}

		return num;
	}

	void VLayout::GetSizeHint (bool count_margin,
										bool count_space,
										Size* size,
										Size* min,
										Size* preferred)
	{
		/*
		Size size_out;
		Size min_size_out;
		Size preferred_size_out;

		AbstractWidget* child;
		WidgetDeque::reverse_iterator it;

		if(count_margin) {
			size_out.set_height(margin().top());
			min_size_out.set_height(margin().top());
			preferred_size_out.set_height(margin().top());
		}

		for(it = sub_widgets()->rbegin(); it != sub_widgets()->rend(); it++)
		{
			child = *it;

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
			if(sub_widget_size()) {
				size_out.add_height((sub_widget_size() - 1) * space());
				min_size_out.add_height((sub_widget_size() - 1) * space());
				preferred_size_out.add_height((sub_widget_size() - 1) * space());
			}
		}

		if(size) *size = size_out;
		if(min) *min = min_size_out;
		if(preferred) *preferred = preferred_size_out;
		*/
	}

}
