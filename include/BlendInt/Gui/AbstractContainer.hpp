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

#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	typedef std::deque<AbstractWidget*> AbstractWidgetDeque;

	class Interface;
	class Context;
	class AbstractContainer;

	class AbstractContainer: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractContainer);

		friend class Context;
		friend class Section;
		friend class AbstractWidget;

	public:

		AbstractContainer ();

		AbstractContainer (size_t size);

		virtual ~AbstractContainer ();

		const Margin& margin () const {return m_margin;}

		void SetMargin (const Margin& margin);

		void SetMargin (int left, int right, int top, int bottom);

		bool FindSubWidget (AbstractWidget* widget);

		size_t sub_widget_size () const
		{
			return m_deque.size();
		}

		const AbstractWidgetDeque& deque () const
		{
			return m_deque;
		}

		const AbstractWidget* first () const
		{
			return first_;
		}

		const AbstractWidget* last() const
		{
			return last_;
		}

		// temporary public:

		bool PushFrontSubWidgetExt (AbstractWidget* widget);

		bool InsertSubWidgetExt (int index, AbstractWidget* widget);

		bool PushBackSubWidgetExt (AbstractWidget* widget);

		bool RemoveSubWidgetExt (AbstractWidget* widget);

		int GetSubWidgetSize ();

#ifdef DEBUG

		void ListSubWidgets ();

#endif

	protected:

		bool PushFrontSubWidget (AbstractWidget* widget);

		bool PushBackSubWidget (AbstractWidget* widget);

		bool AssignSubWidget (size_t index, AbstractWidget* widget);

		bool InsertSubWidget (size_t index, AbstractWidget* widget);

		bool RemoveSubWidget (AbstractWidget* widget);

		void Clear ();

		void ClearExt ();

		void ResizeSubWidget (AbstractWidget* sub, int width, int height);

		void ResizeSubWidget (AbstractWidget* sub, const Size& size);

		void SetSubWidgetPosition (AbstractWidget* sub, int x, int y);

		void SetSubWidgetPosition (AbstractWidget* sub, const Point& pos);

		void SetSubWidgetVisibility (AbstractWidget* sub, bool visible);

		virtual void PerformMarginUpdate (const Margin& margin);

		virtual ResponseType FocusEvent (bool focus);

		void MoveSubWidgets (int offset_x, int offset_y);

		void ResizeSubWidgets (const Size& size);

		void ResizeSubWidgets (int w, int h);

		void FillSingleWidget (size_t index, const Point& out_pos, const Size& out_size, const Margin& margin);

		void FillSingleWidget (size_t index, const Point& pos, const Size& size);

		void FillSingleWidget (size_t index, int left, int bottom, int width, int height);

		void FillSubWidgetsAveragely (const Point& out_pos, const Size& out_size,
						const Margin& margin, Orientation orientation,
						int alignment, int space);

		void FillSubWidgetsAveragely (const Point& pos, const Size& size,
						Orientation orientation, int alignment, int space);

		/**
		 * @brief Fill in the container with average size
		 * @param[in] x the left position
		 * @param[in] y the bottom position
		 */
		void FillSubWidgetsAveragely (int x, int y, int width,
						int height, Orientation orientation,
						int alignment, int space);

		static bool RemoveSubWidget (AbstractContainer* container, AbstractWidget* sub)
		{
			if(container)
				return container->RemoveSubWidget(sub);
			else
				return true;
		}

		static void SetContainer (AbstractWidget* widget, AbstractContainer* container)
		{
			widget->container_ = container;
		}

		bool EnableShadow (AbstractWidget* widget);

		bool DisableShadow (AbstractWidget* widget);

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

		void OnSubWidgetDestroyed (AbstractWidget* widget);

		void DistributeHorizontally (int x, int width, int space);

		void DistributeVertically (int y, int height, int space);

		void AlignHorizontally (int y, int height, int alignment);

		void AlignVertically (int x, int width, int alignment);

		Margin m_margin;

		/**
		 * @brief Sub widgets which build a tree to accept render and device events
		 */
		AbstractWidgetDeque m_deque;

		AbstractWidget* first_;
		AbstractWidget* last_;

	};

}

#endif /* _BLENDINT_CONTAINER_HPP_ */
