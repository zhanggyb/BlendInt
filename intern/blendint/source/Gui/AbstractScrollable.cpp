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

#include <BlendInt/Gui/AbstractScrollable.hpp>

namespace BlendInt {

	AbstractScrollable::AbstractScrollable()
	: Widget()
	{
		scrolled_horizontally_.reset(new Cpp::Event<int>);
		scrolled_vertically_.reset(new Cpp::Event<int>);
	}

	AbstractScrollable::~AbstractScrollable ()
	{
	}

	void AbstractScrollable::SetScrollBar(ScrollBar* hbar, ScrollBar* vbar)
	{
		ScrollBar* hscrollbar = GetHScrollBar();
		ScrollBar* vscrollbar = GetVScrollBar();

		if(hscrollbar != hbar) {

			if(hscrollbar) {

				if(hbar != hscrollbar) {
					if(hscrollbar->managed() && (hscrollbar->reference_count() == 0)) {
						delete hscrollbar;
					} else {
						DBG_PRINT_MSG("Warning: %s", "hscrollbar is not set managed, will not be deleted");
						RemoveSubWidget(hscrollbar);
					}
				}

			}

			if(hbar) {
				hbar->SetOrientation(Horizontal);
			}
			PushFrontSubWidget(hbar);
		}

		if(vscrollbar != vbar) {

			if(vscrollbar) {

				if(vbar != vscrollbar) {
					if(vscrollbar->managed() && (vscrollbar->reference_count() == 0)) {
						delete vscrollbar;
					} else {
						DBG_PRINT_MSG("Warning: %s", "vscrollbar is not set managed, will not be deleted");
						RemoveSubWidget(vscrollbar);
					}
				}
			}

			if(vbar) {
				vbar->SetOrientation(Vertical);
			}
			PushBackSubWidget(vbar);
		}

		AdjustScrollBarGeometries(hbar, vbar);
		assert(subs_count() <= 2);
	}

	ScrollBar* AbstractScrollable::GetHScrollBar() const
	{
		ScrollBar* bar = 0;
		for(AbstractInteractiveForm* p = first_child(); p; p = p->next()) {
			bar = dynamic_cast<ScrollBar*>(p);
			if(bar && (bar->orientation() == Horizontal)) {
				break;
			}
		}

		return bar;
	}

	ScrollBar* AbstractScrollable::GetVScrollBar() const
	{
		ScrollBar* bar = 0;
		for(AbstractInteractiveForm* p = last_child(); p; p = p->previous()) {
			bar = dynamic_cast<ScrollBar*>(p);
			if(bar && (bar->orientation() == Vertical)) {
				break;
			}
		}

		return bar;
	}

	void AbstractScrollable::AdjustScrollBarGeometries (int left, int bottom,
	        int width, int height)
	{
		/*
		int bh = hbar_->visiable() ? hbar_->size().height() : 0;	// height of the bottom hbar
		int rw = vbar_->visiable() ? vbar_->size().width() : 0;	// width of the right vbar

		if(hbar_->visiable()) {
			hbar_->SetPosition(left, bottom);
			hbar_->Resize(width - rw, bh);
		} else {
			hbar_->SetPosition(left, bottom);
			hbar_->Resize(width - vbar_->size().width(), hbar_->size().height());
		}

		if(vbar_->visiable()) {
			vbar_->SetPosition(left + width - rw, bottom + bh);
			vbar_->Resize (rw, height - bh);
		} else {
			vbar_->SetPosition(left + width - vbar_->size().width(), bottom + hbar_->size().height());
			vbar_->Resize (vbar_->size().width(), height - hbar_->size().height());
		}
		*/

	}

	void AbstractScrollable::AdjustScrollBarGeometries (ScrollBar* hbar, ScrollBar* vbar)
	{
		int bh = 0;
		int rw = 0;

		if(hbar) {
			bh = hbar->visiable() ? hbar->size().height() : 0;	// height of the bottom hbar
		}

		if(vbar) {
			rw = vbar->visiable() ? vbar->size().width() : 0;	// width of the right vbar
		}

		if(hbar) {
			SetSubWidgetPosition(hbar, 0, 0);
			ResizeSubWidget(hbar, size().width() - rw, bh);
		}

		if(vbar) {
			SetSubWidgetPosition(vbar, size().width() - rw, bh);
			ResizeSubWidget(vbar, rw, size().height() - bh);
		}

	}

}
