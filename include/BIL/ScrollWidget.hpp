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

#include <BIL/GLBuffer.hpp>

#include <BIL/AbstractSlider.hpp>
#include <BIL/Widget.hpp>

namespace BIL {

	class ScrollBar: public Widget
	{
	public:

		ScrollBar (Orientation orientation, Drawable* parent = 0);

		virtual ~ScrollBar ();

		void set_direction (Orientation orientation);

		Orientation orientation () const {return m_orientation;}

	protected:

		virtual void update (int property);

		virtual void render ();

	private:

		void draw_scroll_cicle (const Rect& rect, float rad);

		Orientation m_orientation;

		static const float scroll_circle_vert[20][2];

		float m_vertex[22][2];

		// GLuint m_buffer;
		GLBuffer m_buffer;
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
