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

#ifndef _BLENDINT_GUI_VLAYOUT_HPP_
#define _BLENDINT_GUI_VLAYOUT_HPP_

#include <deque>
#include <BlendInt/Gui/AbstractLayout.hpp>

namespace BlendInt {

	class VLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(VLayout);

	public:

		VLayout(int align = AlignVerticalCenter, int space = 4);

		virtual ~VLayout ();

		virtual bool AddWidget (AbstractWidget* widget);

		virtual bool InsertWidget (int index, AbstractWidget* widget);

		// column must be 0
		virtual bool InsertWidget (int row, int column, AbstractWidget* widget);

		bool Remove (AbstractWidget* widget);

		int alignment () const
		{
			return alignment_;
		}

		void SetAlignment (int align);

		void SetSpace (int space);

		int space () const
		{
			return space_;
		}

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void PerformMarginUpdate (const Margin& request);

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		void UpdateLayout ();

		/**
		 * @brief distribute horizontally with preferred size
		 */
		void DistributeWithPreferredHeight (int y,
						int height,
						const std::deque<int>* expandable_preferred_heights,
						const std::deque<int>* unexpandable_preferred_heights);

		void DistributeWithSmallHeight (int y,
						int height,
						const std::deque<int>* expandable_preferred_heights,
						int expandable_prefer_sum,
						const std::deque<int>* unexpandable_preferred_heights,
						int unexpandable_prefer_sum);

		void DistributeWithLargeHeight (int y,
						int height,
						const std::deque<int>* expandable_preferred_heights,
						int expandable_prefer_sum,
						const std::deque<int>* unexpandable_preferred_heights,
						int unexpandable_prefer_sum);

		void AlignInVBox (int x, int width, const std::deque<int>* unexpandable_preferred_widths);

		void set_alignment (int align)
		{
			alignment_ = align;
		}

		void set_space (int space)
		{
			space_ = space;
		}

	private:

		int alignment_;

		int space_;
	};

}

#endif /* _BLENDINT_GUI_VLAYOUT_HPP_ */
