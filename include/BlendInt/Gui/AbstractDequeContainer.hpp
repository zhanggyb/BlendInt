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

	class DequeIterator: public AbstractContainerIterator
	{
	public:

		DequeIterator (WidgetDeque* deque)
		: AbstractContainerIterator(), m_deque_ptr(deque)
		{
			m_it = deque->begin();
		}

		DequeIterator (const DequeIterator& orig)
		: AbstractContainerIterator(), m_deque_ptr(0)
		{
			m_deque_ptr = orig.m_deque_ptr;
			m_it = orig.m_it;
		}

		virtual ~DequeIterator ()
		{

		}

		DequeIterator& operator = (const DequeIterator& orig)
		{
			m_deque_ptr = orig.m_deque_ptr;
			m_it = orig.m_it;

			return *this;
		}

		void set_iterator (const WidgetDeque::iterator& it)
		{
			m_it = it;
		}

		virtual AbstractWidget* GetWidget () const
		{
			return *m_it;
		}

		virtual void First ()
		{
			m_it = m_deque_ptr->begin();
		}

		virtual void Next ()
		{
			++m_it;
		}

		virtual bool End ()
		{
			return m_it == m_deque_ptr->end();
		}

	private:

		WidgetDeque* m_deque_ptr;
		WidgetDeque::iterator m_it;
	};

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

		bool AppendSubWidget (AbstractWidget* widget);

		bool InsertSubWidget (size_t index, AbstractWidget* widget);

		void MoveSubWidgets (int offset_x, int offset_y);

		void ResizeSubWidgets (const Size& size);

		void ResizeSubWidgets (unsigned int w, unsigned int h);

		void ClearSubWidgets ();

		WidgetDeque* sub_widgets () const
		{
			return m_sub_widgets.get();
		}

	private:

		void OnSubWidgetDestroyed (AbstractWidget* widget);

		/**
		 * @brief Sub widgets which build a tree to accept render and device events
		 */
		boost::scoped_ptr<WidgetDeque> m_sub_widgets;

		//static DequeIterator default_iterator;
	};
}


#endif /* _BLENDINT_GUI_ABSTRACTDEQUECONTAINER_HPP_ */
