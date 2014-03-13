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

#ifndef _BLENDINT_STACKEDWIDGET_HPP_
#define _BLENDINT_STACKEDWIDGET_HPP_

#include <vector>

#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	/**
	 * @brief class for stacked widgets
	 */
	class StackedWidget: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(StackedWidget);

	public:

		StackedWidget ();

		virtual ~StackedWidget ();

		void Add (Widget* widget);

		void Insert (size_t index, Widget* widget);

		void Remove (Widget* widget);

		size_t count () const {return m_stack.size();}

		void SetIndex (size_t index);

		Widget* GetActiveWidget () const;

		Widget* GetWidget (size_t index);

	protected:

		virtual bool Update (const UpdateRequest& request);

		virtual void Draw (RedrawEvent* event);

	private:

		std::vector<Widget*> m_stack;

		size_t m_index;

	};

}

#endif /* _BLENDINT_STACKEDWIDGET_HPP_ */
