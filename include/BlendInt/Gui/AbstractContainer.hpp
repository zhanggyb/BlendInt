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
//#include <BlendInt/Service/ContextManager.hpp>

namespace BlendInt {

	typedef std::deque<AbstractWidget*> WidgetDeque;

	class ContextManager;
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

	typedef RefPtr<AbstractContainerIterator> IteratorPtr;

	class AbstractContainer: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractContainer);

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

		friend class Context;

		virtual bool AddSubWidget (AbstractWidget* widget) = 0;

		virtual bool RemoveSubWidget (AbstractWidget* widget) = 0;

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

		virtual IteratorPtr CreateIterator () = 0;

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

	private:

		Margin m_margin;

	};

}

#endif /* _BLENDINT_CONTAINER_HPP_ */
