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
#include <BlendInt/Gui/HLayout.hpp>

namespace BlendInt {

	HLayout::HLayout (int align, int space)
	: AbstractContainer(), m_alignment(align), m_space(space)
	{
		set_size (200, 200);
	}

	HLayout::~HLayout ()
	{
	}

	bool HLayout::Prepend (AbstractWidget* widget)
	{
		if(PushFrontSubWidget(widget)) {
			FillSubWidgetsInHBox(position(), size(), margin(), m_alignment, m_space);
			return true;
		}

		return false;
	}

	bool HLayout::Append (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			FillSubWidgetsInHBox(position(), size(), margin(), m_alignment, m_space);
			return true;
		}

		return false;
	}

	bool HLayout::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {
			FillSubWidgetsInHBox(position(), size(), margin(), m_alignment, m_space);
			return true;
		}

		return false;
	}

	void HLayout::SetAlignment (int align)
	{
		if(m_alignment == align) return;

		m_alignment = align;
		FillSubWidgetsInHBox(position(), size(), margin(), align, m_space);
	}

	void HLayout::SetSpace (int space)
	{
		if(m_space == space) return;

		m_space = space;
		FillSubWidgetsInHBox(position(), size(), margin(), m_alignment, m_space);
	}

	Size BlendInt::HLayout::GetPreferredSize () const
	{
		Size preferred_size;

		if(first() == 0) {

			preferred_size.set_width(200);
			preferred_size.set_height(200);

		} else {

			Size tmp_size;

			preferred_size.set_width(-m_space);
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					preferred_size.add_width(tmp_size.width() + m_space);
					preferred_size.set_height(std::max(preferred_size.height(), tmp_size.height()));
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	bool HLayout::IsExpandX () const
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

	bool HLayout::IsExpandY () const
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

	void HLayout::PerformMarginUpdate(const Margin& request)
	{
		FillSubWidgetsInHBox(position(), size(), request, m_alignment, m_space);
	}

	bool HLayout::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		// Do not allow sub widget changing its size
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	bool HLayout::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		// Do not allow sub widget changing its position
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	void HLayout::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void HLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			FillSubWidgetsInHBox(position(), *request.size(), margin(), m_alignment, m_space);
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType HLayout::Draw (Profile& profile)
	{
		return Ignore;
	}

	ResponseType HLayout::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType HLayout::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType HLayout::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType HLayout::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType HLayout::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType HLayout::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType HLayout::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void HLayout::FillSubWidgetsInHBox (const Point& out_pos, const Size& out_size, const Margin& margin,
	        int alignment, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgetsProportionallyInHBox(x, y, width, height, alignment, space);
	}

	void HLayout::FillSubWidgetsInHBox (const Point& pos, const Size& size, int alignment, int space)
	{
		FillSubWidgetsProportionallyInHBox(pos.x(), pos.y(), size.width(), size.height(), alignment, space);
	}

	void HLayout::FillSubWidgetsProportionallyInHBox (int x, int y, int width,
					int height, int alignment, int space)
	{
		boost::scoped_ptr<std::deque<int> > expandable_preferred_widths(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_widths(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_heights(new std::deque<int>);

		int expandable_preferred_width_sum = 0;	// the width sum of the expandable widgets' size
		int unexpandable_preferred_width_sum = 0;	// the width sum of the unexpandable widgets' size

		Size tmp_size;
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if (p->visiable()) {
				tmp_size = p->GetPreferredSize();

				if(p->IsExpandX()) {
					expandable_preferred_width_sum += tmp_size.width();
					expandable_preferred_widths->push_back(tmp_size.width());
				} else {
					unexpandable_preferred_width_sum += tmp_size.width();
					unexpandable_preferred_widths->push_back(tmp_size.width());
				}

				if(!p->IsExpandY()) {
					unexpandable_preferred_heights->push_back(tmp_size.height());
				}

			}
		}

		if((expandable_preferred_widths->size() + unexpandable_preferred_widths->size()) == 0) return;	// do nothing if all sub widgets are invisible

		int total_space = ((expandable_preferred_widths->size() + unexpandable_preferred_widths->size()) - 1) * space;

		int total_preferred_width = expandable_preferred_width_sum
						+ unexpandable_preferred_width_sum
						+ total_space;

		if (total_preferred_width == width) {
			DistributeWithPreferredWidth(x, space,
							expandable_preferred_widths.get(),
							unexpandable_preferred_widths.get());
		} else if (total_preferred_width < width) {
			DistributeWithLargeWidth(x, width, space, expandable_preferred_widths.get(),
							expandable_preferred_width_sum, unexpandable_preferred_widths.get(),
							unexpandable_preferred_width_sum);
		} else {
			DistributeWithSmallWidth(x, width, space, expandable_preferred_widths.get(),
							expandable_preferred_width_sum, unexpandable_preferred_widths.get(),
							unexpandable_preferred_width_sum);
		}

		AlignInHBox(y, height, alignment, unexpandable_preferred_heights.get());
	}

	void HLayout::DistributeWithPreferredWidth (int x, int space,
					const std::deque<int>* expandable_preferred_widths,
					const std::deque<int>* unexpandable_preferred_widths)
	{
		std::deque<int>::const_iterator exp_it = expandable_preferred_widths->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_widths->begin();

		AbstractWidget* p = first();

		while (p) {

			if(p->visiable()) {

				if(p->IsExpandX()) {
					ResizeSubWidget(p, *exp_it, p->size().height());
					SetSubWidgetPosition(p, x, p->position().y());
					exp_it++;
				} else {
					ResizeSubWidget(p, *unexp_it, p->size().height());
					SetSubWidgetPosition(p, x, p->position().y());
					unexp_it++;
				}

				x = x + p->size().width() + space;
			}

			p = p->next();
		}
	}

	void HLayout::DistributeWithSmallWidth (int x,
					int width,
					int space,
					const std::deque<int>* expandable_preferred_widths,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_widths,
					int unexpandable_prefer_sum)
	{
		int widgets_width = width - (expandable_preferred_widths->size() + unexpandable_preferred_widths->size() - 1) * space;

		if(widgets_width <= 0) {
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				p->Resize(0, p->size().height());
			}
			return;
		}

		int reference_width;
		std::deque<int>::const_iterator exp_it = expandable_preferred_widths->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_widths->begin();

		AbstractWidget* p = first();

		if(widgets_width <= unexpandable_prefer_sum) {
			reference_width = widgets_width;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandX()) {
						ResizeSubWidget(p, 0, p->size().height());
						SetSubWidgetPosition(p, x, p->position().y());
						exp_it++;
					} else {
						ResizeSubWidget(p,
										reference_width * (*unexp_it)
														/ unexpandable_prefer_sum,
										p->size().height());
						SetSubWidgetPosition(p, x, p->position().y());
						unexp_it++;
					}

					x = x + p->size().width() + space;
				}

				p = p->next();
			}

		} else {
			reference_width = widgets_width - unexpandable_prefer_sum;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandX()) {
						ResizeSubWidget(p,
										reference_width * (*exp_it)
														/ expandable_prefer_sum,
										p->size().height());
						SetSubWidgetPosition(p, x, p->position().y());
						exp_it++;
					} else {
						ResizeSubWidget(p, (*unexp_it), p->size().height());
						SetSubWidgetPosition(p, x, p->position().y());
						unexp_it++;
					}

					x = x + p->size().width() + space;
				}

				p = p->next();
			}

		}
	}

	void HLayout::DistributeWithLargeWidth (int x,
					int width,
					int space,
					const std::deque<int>* expandable_preferred_widths,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_widths,
					int unexpandable_prefer_sum)
	{
		int widgets_width = width - (expandable_preferred_widths->size() + unexpandable_preferred_widths->size() - 1) * space;

		int expandable_width = widgets_width - unexpandable_prefer_sum;

		std::deque<int>::const_iterator exp_it = expandable_preferred_widths->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_widths->begin();

		AbstractWidget* p = first();
		while (p) {

			if(p->visiable()) {

				if (p->IsExpandX()) {
					ResizeSubWidget(p,
									expandable_width * (*exp_it)
													/ expandable_prefer_sum,
									p->size().height());
					SetSubWidgetPosition(p, x, p->position().y());
					exp_it++;
				} else {
					ResizeSubWidget(p, (*unexp_it), p->size().height());
					SetSubWidgetPosition(p, x, p->position().y());
					unexp_it++;
				}

				x = x + p->size().width() + space;
			}

			p = p->next();
		}
	}

	void HLayout::AlignInHBox (int y, int height, int alignment, const std::deque<int>* unexpandable_preferred_heights)
	{
		std::deque<int>::const_iterator unexp_it =
		        unexpandable_preferred_heights->begin();

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if (p->IsExpandY()) {

				ResizeSubWidget(p, p->size().width(), height);
				SetSubWidgetPosition(p, p->position().x(), y);

			} else {

				if ((*unexp_it) >= height) {
					ResizeSubWidget(p, p->size().width(), height);
					SetSubWidgetPosition(p, p->position().x(), y);
				} else {

					ResizeSubWidget(p, p->size().width(),
					        (*unexp_it));

					if (alignment & AlignTop) {
						SetSubWidgetPosition(p, p->position().x(),
						        y + (height - p->size().height()));
					} else if (alignment & AlignBottom) {
						SetSubWidgetPosition(p, p->position().x(), y);
					} else if (alignment & AlignHorizontalCenter) {
						SetSubWidgetPosition(p, p->position().x(),
						        y + (height - p->size().height()) / 2);
					}
				}

				unexp_it++;
			}
		}
	}

}
