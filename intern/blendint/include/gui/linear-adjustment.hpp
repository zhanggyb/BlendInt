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

#include <deque>

#include <gui/abstract-adjustment.hpp>

namespace BlendInt {

  class LinearAdjustment: public AbstractAdjustment
  {
  public:

    LinearAdjustment (AbstractView* view,
                      Orientation orient,
                      int alignment,
                      int space);

    virtual ~LinearAdjustment ();

    virtual void Adjust (int x, int y, int w, int h);

  private:

    void AdjustHorizontally (int x, int y, int w, int h);

    /**
     * @brief distribute horizontally with preferred size
     */
    void DistributeWithPreferredWidth (int x);

    void DistributeWithSmallWidth (int x,
                                   int width,
                                   int expandable_prefer_sum,
                                   int unexpandable_prefer_sum);

    void DistributeWithLargeWidth (int x,
                                   int width,
                                   int expandable_prefer_sum,
                                   int unexpandable_prefer_sum);

    void AlignHorizontally (int y, int height);

    void AdjustVertically (int x, int y, int w, int h);

    /**
     * @brief distribute horizontally with preferred size
     */
    void DistributeWithPreferredHeight (int y, int height);

    void DistributeWithSmallHeight (int y,
                                    int height,
                                    int expandable_prefer_sum,
                                    int unexpandable_prefer_sum);

    void DistributeWithLargeHeight (int y,
                                    int height,
                                    int expandable_prefer_sum,
                                    int unexpandable_prefer_sum);

    void AlignVertically (int x, int width);

    Orientation orientation_;

    int alignment_;

    int space_;

    std::deque<int> expandable_preferred_width_list_;
    std::deque<int> expandable_preferred_height_list_;
    std::deque<int> unexpandable_preferred_width_list_;
    std::deque<int> unexpandable_preferred_height_list_;

  };

}
