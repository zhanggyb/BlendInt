/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_SCROLLWIDGET_HPP_
#define _BIL_SCROLLWIDGET_HPP_

#include <BIL/Widget.hpp>

namespace BIL {

	class ScrollBar: public Widget
	{
	public:

		ScrollBar (Direction direction, Drawable* parent = 0);

		virtual ~ScrollBar ();

		void set_direction (Direction direction);

		Direction direction () const {return m_direction;}

	protected:

		virtual void update ();

		virtual void render ();

	private:

		void draw_scroll_cicle (const Rect& rect, float rad);

		Direction m_direction;

	};

	class ScrollWidget: public Widget
	{

	public:

		ScrollWidget (Drawable* parent = 0);

		virtual ~ScrollWidget ();

		void set_viewport (Widget* widget);

		Widget* viewport () const {return m_viewport;}

	protected:

		virtual void render ();

	private:

		Widget* m_viewport;

	};

}

#endif /* _BIL_SCROLLWIDGET_HPP_ */
