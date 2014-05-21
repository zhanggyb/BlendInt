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
#include <BlendInt/Gui/VBox.hpp>

namespace BlendInt {

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

		if(PushBackSubWidget(widget)) {

			FillSubWidgetsInVBox(position(), size(), margin(), m_alignment, m_space);

			ret = true;
		}

		return ret;
	}

	bool VBox::Remove (AbstractWidget* widget)
	{
		bool ret = false;

		if(RemoveSubWidget(widget)) {

			FillSubWidgetsInVBox(position(), size(), margin(), m_alignment, m_space);

			ret = true;

		}

		return ret;
	}

	void VBox::SetAlignment (int align)
	{
		if(m_alignment == align) return;

		m_alignment = align;
		FillSubWidgetsInVBox(position(), size(), margin(), align, m_space);
	}

	void VBox::SetSpace (int space)
	{
		if(m_space == space) return;

		m_space = space;
		FillSubWidgetsInVBox(position(), size(), margin(), m_alignment, m_space);
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

	bool VBox::IsExpandX () const
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

	bool VBox::IsExpandY () const
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
					FillSubWidgetsInVBox(position(), *size_p, margin(), m_alignment, m_space);
					break;
				}

				case ContainerMargin: {
					const Margin* margin_p = static_cast<const Margin*>(request.data());
					FillSubWidgetsInVBox(position(), size(), *margin_p, m_alignment, m_space);
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
			int alignment, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		unsigned int width = out_size.width() - margin.left() - margin.right();
		unsigned int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgetsProportionally(x, y, width, height, alignment, space);
	}

	void VBox::FillSubWidgetsInVBox (const Point& pos, const Size& size, int alignment, int space)
	{
		FillSubWidgetsProportionally(pos.x(), pos.y(), size.width(), size.height(), alignment, space);
	}

	void VBox::FillSubWidgetsProportionally (int x, int y, unsigned int width,
					unsigned int height, int alignment, int space)
	{
		boost::scoped_ptr<std::deque<unsigned int> > expandable_preferred_heights(new std::deque<unsigned int>);
		boost::scoped_ptr<std::deque<unsigned int> > unexpandable_preferred_heights(new std::deque<unsigned int>);
		boost::scoped_ptr<std::deque<unsigned int> > unexpandable_preferred_widths(new std::deque<unsigned int>);

		unsigned int expandable_preferred_height_sum = 0;	// the height sum of the expandable widgets' size
		unsigned int unexpandable_preferred_height_sum = 0;	// the height sum of the unexpandable widgets' size

		AbstractWidget* widget = 0;
		Size tmp_size;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			widget = *it;
			if (widget->visiable()) {
				tmp_size = widget->GetPreferredSize();

				if(widget->IsExpandY()) {
					expandable_preferred_height_sum += tmp_size.height();
					expandable_preferred_heights->push_back(tmp_size.height());
				} else {
					unexpandable_preferred_height_sum += tmp_size.height();
					unexpandable_preferred_heights->push_back(tmp_size.height());
				}

				if(!widget->IsExpandX()) {
					unexpandable_preferred_widths->push_back(tmp_size.width());
				}

			}
		}

		if((expandable_preferred_heights->size() + unexpandable_preferred_heights->size()) == 0) return;	// do nothing if all sub widgets are invisible

		unsigned int total_space = ((expandable_preferred_heights->size() + unexpandable_preferred_heights->size()) - 1) * space;

		unsigned int total_preferred_height = expandable_preferred_height_sum
						+ unexpandable_preferred_height_sum
						+ total_space;

		if (total_preferred_height == height) {
			DistributeWithPreferredHeight(y, height, space,
					expandable_preferred_heights.get(),
					unexpandable_preferred_heights.get());
		} else if (total_preferred_height < height) {
			DistributeWithLargeHeight(y, height, space, expandable_preferred_heights.get(),
							expandable_preferred_height_sum, unexpandable_preferred_heights.get(),
							unexpandable_preferred_height_sum);
		} else {
			DistributeWithSmallHeight(y, height, space, expandable_preferred_heights.get(),
							expandable_preferred_height_sum, unexpandable_preferred_heights.get(),
							unexpandable_preferred_height_sum);
		}

		Align(x, width, alignment, unexpandable_preferred_widths.get());
	}

	void VBox::DistributeWithPreferredHeight (int y,
					unsigned int height,
					int space,
					const std::deque<unsigned int>* expandable_preferred_heights,
					const std::deque<unsigned int>* unexpandable_preferred_heights)
	{
		std::deque<unsigned int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<unsigned int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		WidgetDeque::iterator widget_it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		y = y + height;
		while (widget_it != sub_widgets()->end()) {

			widget = *widget_it;

			if(widget->visiable()) {

				if(widget->IsExpandY()) {
					ResizeSubWidget(widget, widget->size().width(), (*exp_it));
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(widget, widget->size().width(), (*unexp_it));
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					unexp_it++;
				}

				y = y - space;
			}

			widget_it++;
		}
	}

	void VBox::DistributeWithSmallHeight (int y,
					unsigned int height,
					int space,
					const std::deque<unsigned int>* expandable_preferred_heights,
					unsigned int expandable_prefer_sum,
					const std::deque<unsigned int>* unexpandable_preferred_heights,
					unsigned int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space;

		if(widgets_height <= 0) {
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				(*it)->Resize((*it)->size().width(), 0);
			}
			return;
		}

		unsigned int reference_height;
		std::deque<unsigned int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<unsigned int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();
		WidgetDeque::iterator it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		y = y + height;
		if(widgets_height <= unexpandable_prefer_sum) {
			reference_height = widgets_height;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->IsExpandY()) {
						ResizeSubWidget(widget, widget->size().width(), 0);
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(widget,
										widget->size().width(),
										reference_height * (*unexp_it)
														/ unexpandable_prefer_sum
										);
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						unexp_it++;
					}

					y = y - space;
				}

				it++;
			}

		} else {
			reference_height = widgets_height - unexpandable_prefer_sum;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->IsExpandY()) {
						ResizeSubWidget(widget,
										widget->size().width(),
										reference_height * (*exp_it)
														/ expandable_prefer_sum);
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(widget, widget->size().width(), (*unexp_it));
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						unexp_it++;
					}

					y = y - space;
				}

				it++;
			}

		}
	}

	void VBox::DistributeWithLargeHeight (int y,
					unsigned int height,
					int space,
					const std::deque<unsigned int>* expandable_preferred_heights,
					unsigned int expandable_prefer_sum,
					const std::deque<unsigned int>* unexpandable_preferred_heights,
					unsigned int unexpandable_prefer_sum)
	{
		unsigned int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space;

		unsigned int expandable_height = widgets_height - unexpandable_prefer_sum;

		std::deque<unsigned int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<unsigned int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		WidgetDeque::iterator it = sub_widgets()->begin();

		y = y + height;
		AbstractWidget* widget = 0;
		while (it != sub_widgets()->end()) {

			widget = (*it);

			if(widget->visiable()) {

				if (widget->IsExpandY()) {
					ResizeSubWidget(widget,
									widget->size().width(),
									expandable_height * (*exp_it)
													/ expandable_prefer_sum);
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(widget, widget->size().width(), (*unexp_it));
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					unexp_it++;
				}

				y = y - space;
			}

			it++;
		}
	}

	void VBox::Align (int x, unsigned int width, int alignment,
	        const std::deque<unsigned int>* unexpandable_preferred_widths)
	{
		std::deque<unsigned int>::const_iterator unexp_it =
				unexpandable_preferred_widths->begin();
		WidgetDeque::iterator it;
		AbstractWidget* widget = 0;
		for (it = sub_widgets()->begin(); it != sub_widgets()->end(); it++) {
			widget = *it;

			if (widget->IsExpandX()) {

				ResizeSubWidget(widget, width, widget->size().height());
				SetSubWidgetPosition(widget, x, widget->position().y());

			} else {

				if ((*unexp_it) >= width) {
					ResizeSubWidget(widget, width, widget->size().height());
					SetSubWidgetPosition(widget, x, widget->position().y());
				} else {

					ResizeSubWidget(widget, (*unexp_it),
					        widget->size().height());

					if (alignment & AlignLeft) {
						SetSubWidgetPosition(widget, x, widget->position().y());
					} else if (alignment & AlignRight) {
						SetSubWidgetPosition(widget,
						        x + (width - widget->size().width()),
						        widget->position().y());
					} else if (alignment & AlignVerticalCenter) {
						SetSubWidgetPosition(widget,
						        x + (width - widget->size().width()) / 2,
						        widget->position().y());
					}

				}

				unexp_it++;

			}
		}
	}

}
