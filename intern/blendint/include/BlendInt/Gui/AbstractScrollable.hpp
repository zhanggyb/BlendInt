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

namespace BlendInt {

	/**
	 * @brief Base class for widgets with native scroll bar
	 */
	class AbstractScrollable: public Widget
	{
	public:

		AbstractScrollable ();

		virtual ~AbstractScrollable ();

		Cpp::EventRef<int, int> scrolled ()
		{
			return *scrolled_;
		}

		// virtual Size GetContentSize () const;
		virtual Point GetOffset () const;

	protected:

		inline void set_offset (int x, int y)
		{
			offset_.reset(x, y);
		}

		inline void set_offset (const Point& offset)
		{
			offset_ = offset;
		}

		inline void fire_scrolled_event (int x, int y)
		{
			scrolled_->fire(x, y);
		}


	private:

		Point offset_;

		boost::scoped_ptr<Cpp::Event<int, int> > scrolled_;

	};

}


#endif /* _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_ */
