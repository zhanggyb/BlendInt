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

#ifndef _BLENDINT_FRAME_HPP_
#define _BLENDINT_FRAME_HPP_

/**
 * @defgroup widgets Widgets
 *
 * @ingroup gui
 */

#include <BlendInt/Widget.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	/**
	 * @brief A widget usually contains other Form or widget in a box with padding
	 *
	 * @ingroup widgets
	 */
	class Frame: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(Frame);

	public:

		Frame ();

		Frame (AbstractWidget* parent);

		virtual ~Frame ();

		const Margin& margin () const {return m_margin;}

		void set_margin (const Margin& margin);

		void set_margin (int left, int right, int top, int bottom);

		void set_widget (AbstractWidget* widget);

		AbstractWidget* widget () const {return m_widget;}

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void ContextMenuPressEvent (ContextMenuEvent* event);

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		/**
		 * @check if geometry contains the cursor, exclude padding
		 * @param cursor
		 * @return
		 */
		bool contain_no_margin (const Coord2d& cursor);

	private:

		Margin m_margin;

		AbstractWidget* m_widget;

	};

} /* namespace BlendInt */
#endif /* _BLENDINT_FRAME_HPP_ */
