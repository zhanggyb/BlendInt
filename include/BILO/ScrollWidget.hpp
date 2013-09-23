/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_SCROLLWIDGET_HPP_
#define _BILO_SCROLLWIDGET_HPP_

#include <BILO/GLBuffer.hpp>

#include <BILO/ScrollBar.hpp>
#include <BILO/Widget.hpp>

namespace BILO {

	class ScrollWidget: public Widget
	{

	public:

		ScrollWidget ();

		ScrollWidget (Drawable* parent);

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
