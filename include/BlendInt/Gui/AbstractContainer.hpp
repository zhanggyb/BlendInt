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

#include <assert.h>

#include <deque>
#include <boost/smart_ptr.hpp>

#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	typedef std::deque<AbstractWidget*> WidgetDeque;

	class Interface;
	class Context;

	/**
	 * @brief A virtual iterator to be instanced for widgets in container
	 */
	class AbstractContainerIterator: public Object
	{
	public:

		AbstractContainerIterator()
		: Object()
		{
		}

		virtual ~AbstractContainerIterator ()
		{
		}

		virtual AbstractWidget* GetWidget () const = 0;

		virtual void First () = 0;

		virtual void Next () = 0;

		virtual bool End () = 0;
	};


	class SingleIterator: public AbstractContainerIterator
	{
	public:

		SingleIterator (AbstractWidget* widget);

		SingleIterator (const SingleIterator& orig);

		virtual ~SingleIterator ();

		SingleIterator& operator = (const SingleIterator& orig);

		virtual AbstractWidget* GetWidget () const;

		virtual void First ();

		virtual void Next ();

		virtual bool End ();

	private:

		AbstractWidget* m_widget;
		bool m_once;
	};


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

	typedef RefPtr<AbstractContainerIterator> IteratorPtr;

	class AbstractContainer: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractContainer);

		friend class Context;
		friend class ContainerProxy;

	public:

		AbstractContainer ()
		{

		}

		virtual ~AbstractContainer ()
		{

		}

		const Margin& margin () const {return m_margin;}

		void SetMargin (const Margin& margin);

		void SetMargin (int left, int right, int top, int bottom);

	protected:

		virtual bool UpdateTest (const UpdateRequest& request);

		virtual void BroadcastUpdate (const UpdateRequest& request);

		virtual bool AddSubWidget (AbstractWidget* widget) = 0;

		virtual bool RemoveSubWidget (AbstractWidget* widget) = 0;

		virtual IteratorPtr CreateIterator (const DeviceEvent& event) = 0;

		void ResizeSubWidget (AbstractWidget* sub, unsigned int x, unsigned int y);

		void ResizeSubWidget (AbstractWidget* sub, const Size& size);

		void SetSubWidgetPosition (AbstractWidget* sub, int x, int y);

		void SetSubWidgetPosition (AbstractWidget* sub, const Point& pos);

		static bool RemoveSubWidget (AbstractContainer* container, AbstractWidget* sub)
		{
			return container->RemoveSubWidget(sub);
		}

		static bool AddSubWidget (AbstractContainer* container, AbstractWidget* sub)
		{
			return container->AddSubWidget(sub);
		}

		static void SetContainer (AbstractWidget* widget, AbstractContainer* container)
		{
			widget->m_container = container;
		}

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

		inline unsigned int horizontal_margins ()
		{
			return m_margin.left() + m_margin.right();
		}

		unsigned int vertical_margins ()
		{
			return m_margin.top() + m_margin.bottom();
		}

	private:

		Margin m_margin;

	};

}

#endif /* _BLENDINT_CONTAINER_HPP_ */
