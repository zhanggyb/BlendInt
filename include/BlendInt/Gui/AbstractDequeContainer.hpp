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

		DequeIterator ()
		{

		}

		DequeIterator (const DequeIterator& orig)
		{
			m_it = orig.m_it;
		}

		virtual ~DequeIterator ()
		{

		}

		void set_it (const WidgetDequeExt::iterator it)
		{
			m_it = it;
		}

		DequeIterator& operator = (const DequeIterator& orig)
		{
			m_it = orig.m_it;

			return *this;
		}

		virtual AbstractWidgetExt* GetWidget () const
		{
			return *m_it;
		}

		virtual void Next ()
		{
			++m_it;
		}

		WidgetDequeExt::iterator m_it;
	};

	class AbstractDequeContainer: public AbstractContainerExt
	{
	public:

		AbstractDequeContainer ();

		virtual ~AbstractDequeContainer ();

	protected:

		virtual bool AddSubWidget (AbstractWidgetExt* widget);

		virtual bool RemoveSubWidget (AbstractWidgetExt* widget);

		virtual RefPtr<AbstractContainerIterator> First (const DeviceEvent& event);

		virtual bool End (const DeviceEvent& event, AbstractContainerIterator* iter);

	private:

		void OnSubWidgetDestroyed (AbstractWidgetExt* widget);

		/**
		 * @brief Sub widgets which build a tree to accept render and device events
		 */
		boost::scoped_ptr<WidgetDequeExt> m_sub_widgets;

		static RefPtr<DequeIterator> default_iterator;
	};
}


#endif /* _BLENDINT_GUI_ABSTRACTDEQUECONTAINER_HPP_ */
