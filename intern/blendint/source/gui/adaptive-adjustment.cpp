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

#include <gui/adaptive-adjustment.hpp>

namespace BlendInt {

	AdaptiveAdjustment::AdaptiveAdjustment(AbstractView* view, Orientation orient, int alignment, int space)
	: AbstractAdjustment(view),
	  orientation_(orient),
	  alignment_(alignment),
	  space_(space)
	{

	}

	AdaptiveAdjustment::~AdaptiveAdjustment ()
	{
	}

	void AdaptiveAdjustment::Adjust (int x, int y, int w, int h)
	{
		if(orientation_ == Horizontal) {
			AdjustHorizontally(x, y, w, h);
		} else {
			AdjustVertically(x, y, w, h);
		}
	}

	void AdaptiveAdjustment::AdjustHorizontally (int x, int y, int w, int h)
	{
		Size tmp;

		for(AbstractView* p = view()->first_subview(); p; p = p->next_view()) {

			tmp = p->GetPreferredSize();
			resize(p, tmp);
			move(p, x, y + h - p->size().height());

			x += p->size().width() + space_;
		}
	}

	void AdaptiveAdjustment::AdjustVertically (int x, int y, int w, int h)
	{
		Size tmp;

		int pos_y = y + h;
		for(AbstractView* p = view()->first_subview(); p; p = p->next_view()) {

			tmp = p->GetPreferredSize();
			resize(p, tmp);
			pos_y = pos_y - p->size().height();

			resize(p, x, pos_y);
			pos_y -= space_;

		}
	}

}
