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

		ScrollBar (Drawable* parent = 0);

		virtual ~ScrollBar ();

	protected:

		virtual void render ();

	};

	class ScrollWidget: public Widget
	{

	public:

		ScrollWidget (Drawable* parent = 0);

		virtual ~ScrollWidget ();

		void add_widget (Widget* widget);

	protected:

		virtual void render ();

	};

}

#endif /* _BIL_SCROLLWIDGET_HPP_ */
