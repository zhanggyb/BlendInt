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
#include <BlendInt/Gui/VLayout.hpp>

namespace BlendInt {

	VBox::VBox (int align, int space)
	: AbstractContainer(), m_alignment(align), m_space(space)
	{
		set_size (200, 200);
	}

	VBox::~VBox ()
	{
	}

	bool VBox::PushBack (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			FillSubWidgetsInVBox(position(), size(), margin(), m_alignment, m_space);
			return true;
		}

		return false;
	}

	bool VBox::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {
			FillSubWidgetsInVBox(position(), size(), margin(), m_alignment, m_space);
			return true;
		}

		return false;
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

		if(first() == 0) {

			preferred_size.set_width(200);
			preferred_size.set_height(200);

		} else {

			Size tmp_size;

			preferred_size.set_height(-m_space);
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					preferred_size.set_width(std::max(preferred_size.width(), tmp_size.width()));
					preferred_size.add_height(tmp_size.height() + m_space);
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	bool VBox::IsExpandX () const
	{
		bool expand = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool VBox::IsExpandY () const
	{
		bool expand = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	void VBox::PerformMarginUpdate(const Margin& request)
	{
		FillSubWidgetsInVBox(position(), size(), request, m_alignment, m_space);
	}

	bool VBox::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		// Do not allow sub widget changing its size
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	bool VBox::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		// Do not allow sub widget changing its position
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	void VBox::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		ReportPositionUpdate(request);
	}

	void VBox::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			FillSubWidgetsInVBox(position(), *request.size(), margin(), m_alignment,
											m_space);		}

		ReportSizeUpdate(request);
	}

	ResponseType VBox::Draw (Profile& profile)
	{
		return Ignore;
	}

	ResponseType VBox::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType VBox::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType VBox::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType VBox::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType VBox::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType VBox::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType VBox::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}


	void VBox::FillSubWidgetsInVBox (const Point& out_pos, const Size& out_size, const Margin& margin,
			int alignment, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgetsProportionallyInVBox(x, y, width, height, alignment, space);
	}

	void VBox::FillSubWidgetsInVBox (const Point& pos, const Size& size, int alignment, int space)
	{
		FillSubWidgetsProportionallyInVBox(pos.x(), pos.y(), size.width(), size.height(), alignment, space);
	}

	void VBox::FillSubWidgetsProportionallyInVBox (int x, int y, int width,
					int height, int alignment, int space)
	{
		boost::scoped_ptr<std::deque<int> > expandable_preferred_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_widths(new std::deque<int>);

		int expandable_preferred_height_sum = 0;	// the height sum of the expandable widgets' size
		int unexpandable_preferred_height_sum = 0;	// the height sum of the unexpandable widgets' size

		Size tmp_size;
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if (p->visiable()) {
				tmp_size = p->GetPreferredSize();

				if(p->IsExpandY()) {
					expandable_preferred_height_sum += tmp_size.height();
					expandable_preferred_heights->push_back(tmp_size.height());
				} else {
					unexpandable_preferred_height_sum += tmp_size.height();
					unexpandable_preferred_heights->push_back(tmp_size.height());
				}

				if(!p->IsExpandX()) {
					unexpandable_preferred_widths->push_back(tmp_size.width());
				}

			}
		}

		if((expandable_preferred_heights->size() + unexpandable_preferred_heights->size()) == 0) return;	// do nothing if all sub widgets are invisible

		int total_space = ((expandable_preferred_heights->size() + unexpandable_preferred_heights->size()) - 1) * space;

		int total_preferred_height = expandable_preferred_height_sum
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

		AlignInVBox(x, width, alignment, unexpandable_preferred_widths.get());
	}

	void VBox::DistributeWithPreferredHeight (int y,
					int height,
					int space,
					const std::deque<int>* expandable_preferred_heights,
					const std::deque<int>* unexpandable_preferred_heights)
	{
		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		AbstractWidget* p = first();

		y = y + height;
		while (p) {

			if(p->visiable()) {

				if(p->IsExpandY()) {
					ResizeSubWidget(p, p->size().width(), (*exp_it));
					y = y - p->size().height();
					SetSubWidgetPosition(p, p->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(p, p->size().width(), (*unexp_it));
					y = y - p->size().height();
					SetSubWidgetPosition(p, p->position().x(), y);
					unexp_it++;
				}

				y = y - space;
			}

			p = p->next();
		}
	}

	void VBox::DistributeWithSmallHeight (int y,
					int height,
					int space,
					const std::deque<int>* expandable_preferred_heights,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_heights,
					int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space;

		if(widgets_height <= 0) {
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				p->Resize(p->size().width(), 0);
			}
			return;
		}

		int reference_height;
		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		AbstractWidget* p = first();

		y = y + height;
		if(widgets_height <= unexpandable_prefer_sum) {
			reference_height = widgets_height;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandY()) {
						ResizeSubWidget(p, p->size().width(), 0);
						y = y - p->size().height();
						SetSubWidgetPosition(p, p->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(p,
										p->size().width(),
										reference_height * (*unexp_it)
														/ unexpandable_prefer_sum
										);
						y = y - p->size().height();
						SetSubWidgetPosition(p, p->position().x(), y);
						unexp_it++;
					}

					y = y - space;
				}

				p = p->next();
			}

		} else {
			reference_height = widgets_height - unexpandable_prefer_sum;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandY()) {
						ResizeSubWidget(p,
										p->size().width(),
										reference_height * (*exp_it)
														/ expandable_prefer_sum);
						y = y - p->size().height();
						SetSubWidgetPosition(p, p->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(p, p->size().width(), (*unexp_it));
						y = y - p->size().height();
						SetSubWidgetPosition(p, p->position().x(), y);
						unexp_it++;
					}

					y = y - space;
				}

				p = p->next();
			}

		}
	}

	void VBox::DistributeWithLargeHeight (int y,
					int height,
					int space,
					const std::deque<int>* expandable_preferred_heights,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_heights,
					int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space;

		int expandable_height = widgets_height - unexpandable_prefer_sum;

		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		AbstractWidget* p = first();

		y = y + height;
		while (p) {

			if(p->visiable()) {

				if (p->IsExpandY()) {
					ResizeSubWidget(p,
									p->size().width(),
									expandable_height * (*exp_it)
													/ expandable_prefer_sum);
					y = y - p->size().height();
					SetSubWidgetPosition(p, p->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(p, p->size().width(), (*unexp_it));
					y = y - p->size().height();
					SetSubWidgetPosition(p, p->position().x(), y);
					unexp_it++;
				}

				y = y - space;
			}

			p = p->next();
		}
	}

	void VBox::AlignInVBox (int x, int width, int alignment,
	        const std::deque<int>* unexpandable_preferred_widths)
	{
		std::deque<int>::const_iterator unexp_it =
				unexpandable_preferred_widths->begin();

		for (AbstractWidget* p = first(); p; p = p->next()) {

			if (p->IsExpandX()) {

				ResizeSubWidget(p, width, p->size().height());
				SetSubWidgetPosition(p, x, p->position().y());

			} else {

				if ((*unexp_it) >= width) {
					ResizeSubWidget(p, width, p->size().height());
					SetSubWidgetPosition(p, x, p->position().y());
				} else {

					ResizeSubWidget(p, (*unexp_it),
					        p->size().height());

					if (alignment & AlignLeft) {
						SetSubWidgetPosition(p, x, p->position().y());
					} else if (alignment & AlignRight) {
						SetSubWidgetPosition(p,
						        x + (width - p->size().width()),
						        p->position().y());
					} else if (alignment & AlignVerticalCenter) {
						SetSubWidgetPosition(p,
						        x + (width - p->size().width()) / 2,
						        p->position().y());
					}

				}

				unexp_it++;

			}
		}
	}


}
