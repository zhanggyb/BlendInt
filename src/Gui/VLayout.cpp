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

	VLayoutExt::VLayoutExt (Context* context, int alignment, int space)
	: AbstractLayoutExt(context),
	  m_alignment(alignment),
	  m_space(space)
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

		Distribute();

		Align();
	}

	void VLayoutExt::Distribute()
	{
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
				y -= m_space;	// space
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
						y -= m_space; // space
					} else {
						y -= averate_expand_height;
						widget->SetPosition(widget->position().x(), y);
						widget->Resize(widget->size().width(), averate_expand_height);
						y -= m_space;	// space
					}
				}

			}

		}
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

	VBox::VBox (int align, int space)
	: AbstractDequeContainer(), m_alignment(align), m_space(space)
	{
		set_size (200, 200);
	}

	VBox::~VBox ()
	{
	}

	bool VBox::Add (AbstractWidget* widget)
	{
		bool ret = false;

		if(AddSubWidget(widget)) {

			FillSubWidgetsInVBox(position(), size(), margin(), m_space);

			ret = true;
		}

		return ret;
	}

	bool VBox::Remove (AbstractWidget* widget)
	{
		bool ret = false;

		if(RemoveSubWidget(widget)) {

			FillSubWidgetsInVBox(position(), size(), margin(), m_space);

			ret = true;

		}

		return ret;
	}

	void VBox::SetAlignment (int align)
	{
	}

	void VBox::SetSpace (int space)
	{
	}

	Size BlendInt::VBox::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {

			preferred_size.set_width(200);
			preferred_size.set_height(200);

		} else {

			AbstractWidget* widget = 0;
			Size tmp_size;

			preferred_size.set_height(-m_space);
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				widget = *it;

				if(widget->visiable()) {
					tmp_size = widget->GetPreferredSize();

					preferred_size.set_width(std::max(preferred_size.width(), tmp_size.width()));
					preferred_size.add_height(tmp_size.height() + m_space);
				}
			}

			preferred_size.add_width(margin().left() + margin().right());
			preferred_size.add_height(margin().top() + margin().bottom());
		}

		return preferred_size;
	}

	bool VBox::UpdateTest (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case SubWidgetSize:
					return false;	// DO not allow sub widget geometry reset outside

				case SubWidgetPosition:
					return false;

				default:
					return AbstractDequeContainer::UpdateTest(request);

			}

		} else {
			return false;
		}
	}

	void VBox::Update (const UpdateRequest& request)
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
					FillSubWidgetsInVBox(position(), *size_p, margin(), m_space);
					break;
				}

				case ContainerMargin: {
					const Margin* margin_p = static_cast<const Margin*>(request.data());
					FillSubWidgetsInVBox(position(), size(), *margin_p, m_space);
					break;
				}

				case LayoutPropertySpace: {
					const int* space_p = static_cast<const int*>(request.data());
					FillSubWidgetsInVBox(position(), size(), margin(), *space_p);
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

	ResponseType VBox::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBox::CursorEnterEvent (bool entered)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBox::KeyPressEvent (const KeyEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBox::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBox::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBox::MousePressEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBox::MouseReleaseEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBox::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	void VBox::FillSubWidgetsInVBox (const Point& out_pos, const Size& out_size, const Margin& margin,
	        int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		unsigned int width = out_size.width() - margin.left() - margin.right();
		unsigned int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgetsProportionally(x, y, width, height, space);
	}

	void VBox::FillSubWidgetsInVBox (const Point& pos, const Size& size, int space)
	{
		FillSubWidgetsProportionally(pos.x(), pos.y(), size.width(), size.height(), space);
	}

	void VBox::FillSubWidgetsProportionally (int x, int y, unsigned int width,
					unsigned int height, int space)
	{
		boost::scoped_ptr<std::deque<Size> > expandable_prefers(new std::deque<Size>);
		boost::scoped_ptr<std::deque<Size> > unexpandable_prefers(new std::deque<Size>);

		unsigned int expandable_prefer_sum = 0;	// the width sum of the expandable widgets' size
		unsigned int unexpandable_prefer_sum = 0;	// the width sum of the unexpandable widgets' size

		Size tmp_size;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if ((*it)->visiable()) {
				tmp_size = (*it)->GetPreferredSize();

				if((*it)->expand_y()) {
					expandable_prefers->push_back(tmp_size);
					expandable_prefer_sum += tmp_size.height();
				} else {
					unexpandable_prefers->push_back(tmp_size);
					unexpandable_prefer_sum += tmp_size.height();
				}
			}
		}

		if((expandable_prefers->size() + unexpandable_prefers->size()) == 0) return;	// do nothing if all sub widgets are invisible

		unsigned int total_space = ((expandable_prefers->size() + unexpandable_prefers->size()) - 1) * space;

		unsigned int total_preferred_height = expandable_prefer_sum
						+ unexpandable_prefer_sum
						+ total_space;

		if (total_preferred_height == height) {
			DistributeWithPreferredHeight(y, height, space,
							expandable_prefers.get(),
							unexpandable_prefers.get());
		} else if (total_preferred_height < height) {
			DistributeWithLargeHeight(y, height, space, expandable_prefers.get(),
							expandable_prefer_sum, unexpandable_prefers.get(),
							unexpandable_prefer_sum);
		} else {
			DistributeWithSmallHeight(y, height, space, expandable_prefers.get(),
							expandable_prefer_sum, unexpandable_prefers.get(),
							unexpandable_prefer_sum);
		}

		Align(y, height);
	}

	void VBox::DistributeWithPreferredHeight (int y,
					unsigned int height,
					int space,
					const std::deque<Size>* expandable_prefers,
					const std::deque<Size>* unexpandable_prefers)
	{
		std::deque<Size>::const_iterator exp_it = expandable_prefers->begin();
		std::deque<Size>::const_iterator unexp_it = unexpandable_prefers->begin();

		WidgetDeque::iterator widget_it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		y = y + height;
		while (widget_it != sub_widgets()->end()) {

			widget = *widget_it;

			if(widget->visiable()) {

				if(widget->expand_x()) {
					ResizeSubWidget(widget, widget->size().width(), exp_it->height());
					SetSubWidgetPosition(widget, widget->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(widget, widget->size().width(), unexp_it->height());
					SetSubWidgetPosition(widget, widget->position().x(), y);
					unexp_it++;
				}

				y -= widget->size().height() + space;
			}

			widget_it++;
		}
	}

	void VBox::DistributeWithSmallHeight (int y,
					unsigned int height,
					int space,
					const std::deque<Size>* expandable_prefers,
					unsigned int expandable_prefer_sum,
					const std::deque<Size>* unexpandable_prefers,
					unsigned int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_prefers->size() + unexpandable_prefers->size() - 1) * space;

		if(widgets_height <= 0) {
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				(*it)->Resize((*it)->size().height(), 0);
			}
			return;
		}

		unsigned int reference_height;
		std::deque<Size>::const_iterator exp_it = expandable_prefers->begin();
		std::deque<Size>::const_iterator unexp_it = unexpandable_prefers->begin();
		WidgetDeque::iterator it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		y = y + height;
		if(widgets_height <= unexpandable_prefer_sum) {
			reference_height = widgets_height;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->expand_y()) {
						ResizeSubWidget(widget, widget->size().width(), 0);
						SetSubWidgetPosition(widget, widget->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(widget,
										widget->size().width(),
										reference_height * unexp_it->height()
														/ unexpandable_prefer_sum
										);
						SetSubWidgetPosition(widget, widget->position().x(), y);
						unexp_it++;
					}

					y -= widget->size().height() + space;
				}

				it++;
			}

		} else {
			reference_height = widgets_height - unexpandable_prefer_sum;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->expand_y()) {
						ResizeSubWidget(widget,
										widget->size().width(),
										reference_height * exp_it->height()
														/ expandable_prefer_sum);
						SetSubWidgetPosition(widget, widget->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(widget, widget->size().width(), unexp_it->height());
						SetSubWidgetPosition(widget, widget->position().x(), y);
						unexp_it++;
					}

					y -= widget->size().height() + space;
				}

				it++;
			}

		}
	}

	void VBox::DistributeWithLargeHeight (int y,
					unsigned int height,
					int space,
					const std::deque<Size>* expandable_prefers,
					unsigned int expandable_prefer_sum,
					const std::deque<Size>* unexpandable_prefers,
					unsigned int unexpandable_prefer_sum)
	{
		unsigned int widgets_height = height - (expandable_prefers->size() + unexpandable_prefers->size() - 1) * space;

		unsigned int expandable_height = widgets_height - unexpandable_prefer_sum;

		std::deque<Size>::const_iterator exp_it = expandable_prefers->begin();
		std::deque<Size>::const_iterator unexp_it = unexpandable_prefers->begin();

		WidgetDeque::iterator it = sub_widgets()->begin();

		y = y + height;
		AbstractWidget* widget = 0;
		while (it != sub_widgets()->end()) {

			widget = (*it);

			if(widget->visiable()) {

				if (widget->expand_y()) {
					ResizeSubWidget(widget,
									widget->size().width(),
									expandable_height * exp_it->height()
													/ expandable_prefer_sum);
					SetSubWidgetPosition(widget, widget->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(widget, widget->size().width(), unexp_it->height());
					SetSubWidgetPosition(widget, widget->position().x(), y);
					unexp_it++;
				}

				y -= widget->size().height() + space;
			}

			it++;
		}
	}

	void VBox::Align(int x, unsigned int width)
	{
		WidgetDeque::iterator it;
		AbstractWidget* widget = 0;
		for(it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			widget = *it;

			if (widget->expand_x() ||
					(widget->size().width() > width)) {
				ResizeSubWidget(widget, width, widget->size().height());
				SetSubWidgetPosition(widget, x, widget->position().y());
			} else {

				if (alignment() & AlignLeft) {
					SetSubWidgetPosition(widget, x, widget->position().y());
				} else if (alignment() & AlignRight) {
					SetSubWidgetPosition(widget, x + (width - widget->size().width()), widget->position().y());
				} else if (alignment() & AlignVerticalCenter) {
					SetSubWidgetPosition(widget, x + (width - widget->size().width()) / 2, widget->position().y());
				}

			}
		}
	}

}
