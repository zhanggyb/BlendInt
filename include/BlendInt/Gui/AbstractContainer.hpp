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

#ifndef _BLENDINT_CONTAINER_HPP_
#define _BLENDINT_CONTAINER_HPP_

#include <deque>

#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
//#include <BlendInt/Service/ContextManager.hpp>

namespace BlendInt {

	typedef std::deque<AbstractWidget*> WidgetDeque;

	class ContextManager;
	class Interface;

	/**
	 * @brief Base class for widgets which contain other widgets
	 */
	class AbstractContainer: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractContainer);

	public:

		AbstractContainer();

		virtual ~AbstractContainer ();

		const Margin& margin () const {return m_margin;}

		void SetMargin (const Margin& margin);

		void SetMargin (int left, int right, int top, int bottom);

		bool AddSubWidget (AbstractWidget* widget);

		bool RemoveSubWidget (AbstractWidget* widget);

		size_t sub_widget_size () const {return m_sub_widgets.size();}

#ifdef DEBUG
		void print ();
#endif

	protected:

		void set_margin (const Margin& margin)
		{
			m_margin = margin;
		}

		void set_margin (int left, int right, int top, int bottom)
		{
			m_margin.set_left(left);
			m_margin.set_right(right);
			m_margin.set_top(top);
			m_margin.set_bottom(bottom);
		}

		void ClearSubWidgets ();

		WidgetDeque& sub_widgets ()
		{
			return m_sub_widgets;
		}

	private:

		friend class ContextManager;
		friend class Interface;

		void OnSubWidgetDestroyed (AbstractWidget* widget);

		void RemoveSubWidgetOnly (AbstractWidget* widget);

		Margin m_margin;

		/**
		 * @brief Sub widgets which build a tree to accept render and device events
		 */
		WidgetDeque m_sub_widgets;
	};

}

#endif /* _BLENDINT_CONTAINER_HPP_ */
