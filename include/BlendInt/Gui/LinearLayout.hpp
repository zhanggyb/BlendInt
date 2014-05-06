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

#ifndef _BLENDINT_LINEARLAYOUT_HPP_
#define _BLENDINT_LINEARLAYOUT_HPP_

#include <BlendInt/Gui/AbstractLayout.hpp>

namespace BlendInt {

	class LinearLayout: public AbstractLayoutExt
	{
	public:

		explicit LinearLayout (AbstractContainer* container);

		LinearLayout (AbstractContainer* container,
						Orientation orientation = Horizontal,
						int alignment = AlignCenter,
						int space = 2);

		virtual ~LinearLayout ();

		/**
		 * Not use
		 */
		virtual void Fill ();

		void Fill (const WidgetDeque* sub_widgets);

		void set_alignment (int alignment)
		{
			m_alignment = alignment;
		}

		void set_space (int space)
		{
			m_space = space;
		}

	protected:

		void Distribute (const WidgetDeque* sub_widgets);

		void Align (const WidgetDeque* sub_widgets);

	private:

		Orientation m_orientation;
		int m_alignment;
		int m_space;
	};

}

#endif /* _BLENDINT_LINEARLAYOUT_HPP_ */
