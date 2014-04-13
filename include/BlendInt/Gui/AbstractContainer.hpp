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
#include <boost/smart_ptr.hpp>

#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
//#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Gui/AbstractWidgetIterator.hpp>

namespace BlendInt {

	typedef std::deque<AbstractWidget*> WidgetDeque;
	typedef std::deque<AbstractWidgetExt*> WidgetDequeExt;

	class ContextManager;
	class Interface;

	class AbstractContainerExt: public AbstractWidgetExt
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractContainerExt);

	public:

		AbstractContainerExt ()
		{

		}

		virtual ~AbstractContainerExt ()
		{

		}

	protected:

		virtual bool AddSubWidget (AbstractWidgetExt* widget) = 0;

		virtual bool RemoveSubWidget (AbstractWidgetExt* widget) = 0;

		static bool RemoveSubWidget (AbstractContainerExt* container, AbstractWidgetExt* sub)
		{
			return container->RemoveSubWidget(sub);
		}

		static bool AddSubWidget (AbstractContainerExt* container, AbstractWidgetExt* sub)
		{
			return container->AddSubWidget(sub);
		}

		static void SetContainer (AbstractWidgetExt* widget, AbstractContainerExt* container)
		{
			if(container) {
				widget->m_container = container;
				widget->m_flag.set(WidgetFlagInContainerExt);
			} else {
				widget->m_container = 0;
				widget->m_flag.reset(WidgetFlagInContainerExt);
			}
		}

		virtual AbstractWidgetIterator* First (const DeviceEvent& event) = 0;

		virtual bool End (const DeviceEvent& event, AbstractWidgetIterator* iter) = 0;

	};

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

		bool FindSubWidget (AbstractWidget* widget);

		size_t sub_widget_size () const
		{
			return m_sub_widgets->size();
		}

#ifdef DEBUG
		void print ();
#endif

	protected:

		bool AppendSubWidget (AbstractWidget* widget);

		bool InsertSubWidget (size_t index, AbstractWidget* widget);

		bool RemoveSubWidget (AbstractWidget* widget);

		void MoveSubWidgets (int offset_x, int offset_y);

		void ResizeSubWidgets (const Size& size);

		void ResizeSubWidgets (unsigned int w, unsigned int h);

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

		WidgetDeque* sub_widgets () const
		{
			return m_sub_widgets.get();
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
		boost::scoped_ptr<WidgetDeque> m_sub_widgets;
	};

}

#endif /* _BLENDINT_CONTAINER_HPP_ */
