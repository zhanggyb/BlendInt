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

#pragma once

#include <vector>
#include <deque>

#include <blendint/gui/abstract-adjustment.hpp>

namespace BlendInt {

	class TableAdjustment: public AbstractAdjustment
	{
	public:

		TableAdjustment (AbstractView* view, unsigned int row, unsigned int column, int space);

		virtual ~TableAdjustment ();

		virtual void Adjust (int x, int y, int w, int h);

	private:

		void Scan ();

		Size GetTotalPreferredSize () const;

		void DistributeHorizontallyWithSmallWidth (int x, int width) const;

		void DistributeHorizontallyWithPreferredWidth (int x, int width) const;

		void DistributeHorizontallyWithLargeWidth (int x, int width) const;

		void DistributeVerticallyWithSmallHeight (int y, int height) const;

		void DistributeVerticallyWithPreferredHeight(int y, int height) const;

		void DistributeVerticallyWithLargeHeight (int y, int height) const;

		unsigned int row_;

		unsigned int column_;

		int space_;

		std::vector<bool> column_expand_status_;
		std::vector<bool> row_expand_status_;

		std::vector<int> column_width_list_;
		std::vector<int> row_height_list_;

		int total_fixed_column_width_;
		int total_fixed_row_height_;

		int fixed_column_num_;
		int fixed_row_num_;

		bool expand_x_;
		bool expand_y_;
	};

}
