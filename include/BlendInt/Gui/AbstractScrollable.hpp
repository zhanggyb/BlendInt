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

#ifndef _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_
#define _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_

#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Gui/ScrollBar.hpp>

namespace BlendInt {

	/**
	 * @brief Base class for widgets with native scroll bar
	 */
	class AbstractScrollable: public Widget
	{
	public:

		AbstractScrollable ();

		virtual ~AbstractScrollable ();

		void SetScrollBar (ScrollBar* hbar, ScrollBar* vbar);

		ScrollBar* GetHScrollBar () const;

		ScrollBar* GetVScrollBar () const;

		Cpp::EventRef<int> scrolled_horizontally ()
		{
			return *scrolled_horizontally_;
		}

		Cpp::EventRef<int> scrolled_vertically ()
		{
			return *scrolled_vertically_;
		}

	protected:

		void AdjustScrollBarGeometries (int left, int bottom, int width, int height);

		void AdjustScrollBarGeometries (ScrollBar* hbar, ScrollBar* vbar);

		inline void fire_scrolled_event (int x)
		{
			scrolled_horizontally_->fire(x);
		}

	private:

		boost::scoped_ptr<Cpp::Event<int> > scrolled_horizontally_;

		boost::scoped_ptr<Cpp::Event<int> > scrolled_vertically_;

	};

}


#endif /* _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_ */
