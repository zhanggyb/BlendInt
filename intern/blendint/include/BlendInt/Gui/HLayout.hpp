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

#ifndef _BLENDINT_GUI_HBOX_HPP_
#define _BLENDINT_GUI_HBOX_HPP_

#include <BlendInt/Gui/Layout.hpp>

namespace BlendInt {

	class HLayout: public Layout
	{
		DISALLOW_COPY_AND_ASSIGN(HLayout);

	public:

		HLayout(int align = AlignHorizontalCenter, int space = 4);

		virtual ~HLayout ();

		bool Prepend (Widget* widget);

		bool Append (Widget* widget);

		bool Remove (Widget* widget);

		int alignment () const
		{
			return m_alignment;
		}

		void SetAlignment (int align);

		void SetSpace (int space);

		int space () const
		{
			return m_space;
		}

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void PerformMarginUpdate (const Margin& margin);

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		void FillSubWidgetsInHBox (const Size& out_size, const Margin& margin, int alignment, int space);

		void FillSubWidgetsProportionallyInHBox (int x, int y, int width, int height, int alignment, int space);

		/**
		 * @brief distribute horizontally with preferred size
		 */
		void DistributeWithPreferredWidth (int x,
				int space,
		        const std::deque<int>* expandable_preferred_widths,
		        const std::deque<int>* unexpandable_preferred_widths);

		void DistributeWithSmallWidth (int x,
						int width,
						int space,
						const std::deque<int>* expandable_preferred_widths,
						int expandable_prefer_sum,
						const std::deque<int>* unexpandable_preferred_widths,
						int unexpandable_prefer_sum);

		void DistributeWithLargeWidth (int x,
						int width,
						int space,
						const std::deque<int>* expandable_preferred_widths,
						int expandable_prefer_sum,
						const std::deque<int>* unexpandable_preferred_widths,
						int unexpandable_prefer_sum);

		void AlignInHBox (int y, int height, int alignment, const std::deque<int>* unexpandable_preferred_heights);

		void set_alignment (int align)
		{
			m_alignment = align;
		}

		void set_space (int space)
		{
			m_space = space;
		}

	private:

		int m_alignment;

		int m_space;

	};

}

#endif /* _BLENDINT_GUI_HBOX_HPP_ */
