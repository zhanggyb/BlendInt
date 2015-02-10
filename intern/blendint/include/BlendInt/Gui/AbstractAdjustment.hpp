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

#pragma once

#include <BlendInt/Gui/AbstractView.hpp>

namespace BlendInt {

	/**
	 * @brief Used for layouts to adjust sub views' geometry
	 */
	class AbstractAdjustment
	{
	public:

		AbstractAdjustment (AbstractView* view)
		: view_(view)
		{

		}

		virtual ~AbstractAdjustment ()
		{

		}

		virtual void Adjust (int x, int y, int w, int h) = 0;

	protected:

		inline void move (AbstractView* sub, int x, int y) const
		{
			view_->MoveSubViewTo(sub, x, y);
		}

		inline void move (AbstractView* sub, const Point& point) const
		{
			view_->MoveSubViewTo(sub, point);
		}

		inline void resize (AbstractView* sub, int width, int height) const
		{
			view_->ResizeSubView(sub, width, height);
		}

		inline void resize (AbstractView* sub, const Size& size) const
		{
			view_->ResizeSubView(sub, size);
		}

		AbstractView* view() const
		{
			return view_;
		}

	private:

		AbstractView* view_;
	};

}
