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

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

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

		virtual ~AbstractContainer ();

		const Margin& margin () const {return margin_;}

		void SetMargin (const Margin& margin);

		void SetMargin (int left, int right, int top, int bottom);

		AbstractWidget* first () const
		{
			return first_;
		}

		AbstractWidget* last() const
		{
			return last_;
		}

		// temporary public:
#ifdef DEBUG
		bool PushFrontSubWidget (AbstractWidget* widget);

		bool InsertSubWidget (int index, AbstractWidget* widget);

		bool PushBackSubWidget (AbstractWidget* widget);

		bool RemoveSubWidget (AbstractWidget* widget);

		void ListSubWidgets ();
#endif

		int widget_count () const
		{
			return widget_count_;
		}

		AbstractWidget* operator [] (int i) const;

		AbstractWidget* GetWidgetAt (int i) const;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool RoundTypeUpdateTest (const RoundTypeUpdateRequest& request);

		virtual bool RoundRadiusUpdateTest (const RoundRadiusUpdateRequest& request);

#ifndef DEBUG
		bool PushFrontSubWidget (AbstractWidget* widget);

		bool InsertSubWidget (int index, AbstractWidget* widget);

		bool PushBackSubWidget (AbstractWidget* widget);

		bool RemoveSubWidget (AbstractWidget* widget);
#endif

		void Clear ();

		void ResizeSubWidget (AbstractWidget* sub, int width, int height);

		void ResizeSubWidget (AbstractWidget* sub, const Size& size);

		void SetSubWidgetPosition (AbstractWidget* sub, int x, int y);

		void SetSubWidgetPosition (AbstractWidget* sub, const Point& pos);

		void SetSubWidgetRoundType (AbstractWidget* sub, int type);

		void SetSubWidgetRoundRadius (AbstractWidget* sub, float radius);

		void SetSubWidgetVisibility (AbstractWidget* sub, bool visible);

		virtual void PerformMarginUpdate (const Margin& margin);

		virtual ResponseType FocusEvent (bool focus);

		void MoveSubWidgets (int offset_x, int offset_y);

		void ResizeSubWidgets (const Size& size);

		void ResizeSubWidgets (int w, int h);

		void FillSingleWidget (int index, const Point& out_pos, const Size& out_size, const Margin& margin);

		void FillSingleWidget (int index, const Point& pos, const Size& size);

		void FillSingleWidget (int index, int left, int bottom, int width, int height);

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

		void set_margin (const Margin& margin)
		{
			margin_ = margin;
		}

		void set_margin (int left, int right, int top, int bottom)
		{
			margin_.set_left(left);
			margin_.set_right(right);
			margin_.set_top(top);
			margin_.set_bottom(bottom);
		}

	private:

		void DistributeHorizontally (int x, int width, int space);

		void DistributeVertically (int y, int height, int space);

		void AlignHorizontally (int y, int height, int alignment);

		void AlignVertically (int x, int width, int alignment);

		Margin margin_;

		AbstractWidget* first_;
		AbstractWidget* last_;

		int widget_count_;

	};

}

#endif /* _BLENDINT_CONTAINER_HPP_ */
