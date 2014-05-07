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

#ifndef _BLENDINT_GUI_ABSTRACTDEQUECONTAINER_HPP_
#define _BLENDINT_GUI_ABSTRACTDEQUECONTAINER_HPP_

#include <deque>
#include <boost/smart_ptr.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	/**
	 * @brief A container holds sub widgets with std::deque
	 */
	class AbstractDequeContainer: public AbstractContainer
	{
	public:

		AbstractDequeContainer ();

		virtual ~AbstractDequeContainer ();

		bool FindSubWidget (AbstractWidget* widget);

		size_t sub_widget_size () const
		{
			return m_sub_widgets->size();
		}

	protected:

		virtual bool AddSubWidget (AbstractWidget* widget);

		virtual bool RemoveSubWidget (AbstractWidget* widget);

		virtual IteratorPtr CreateIterator (const DeviceEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		bool AppendSubWidget (AbstractWidget* widget);

		bool InsertSubWidget (size_t index, AbstractWidget* widget);

		void MoveSubWidgets (int offset_x, int offset_y);

		void ResizeSubWidgets (const Size& size);

		void ResizeSubWidgets (unsigned int w, unsigned int h);

		void ClearSubWidgets ();

		void FillSubWidgetsAveragely (const Point& pos, const Size& out_size, const Margin& margin, Orientation orientation, int alignment, int space);

		void FillSubWidgetsAveragely (const Point& pos, const Size& size, Orientation orientation, int alignment, int space);

		void FillSubWidgetsAveragely (int x, int y, unsigned int width, unsigned int height, Orientation orientation, int alignment, int space);

		WidgetDeque* sub_widgets () const
		{
			return m_sub_widgets.get();
		}

	private:

		void OnSubWidgetDestroyed (AbstractWidget* widget);

		void DistributeHorizontally (int x, unsigned int width, int space);

		void DistributeVertically (int y, unsigned int height, int space);

		void AlignHorizontally (int y, unsigned int height, int alignment);

		void AlignVertically (int x, unsigned int width, int alignment);

		/**
		 * @brief Sub widgets which build a tree to accept render and device events
		 */
		boost::scoped_ptr<WidgetDeque> m_sub_widgets;

		//static DequeIterator default_iterator;
	};
}


#endif /* _BLENDINT_GUI_ABSTRACTDEQUECONTAINER_HPP_ */
