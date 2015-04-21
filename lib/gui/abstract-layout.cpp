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

#include <blendint/gui/abstract-layout.hpp>

namespace BlendInt {

AbstractLayout::AbstractLayout ()
    : AbstractWidget()
{

}

AbstractLayout::AbstractLayout (int width, int height)
    : AbstractWidget(width, height)
{

}

AbstractLayout::AbstractLayout (int width, int height, const Margin& margin)
    : AbstractWidget(width, height), margin_(margin)
{

}

AbstractLayout::~AbstractLayout ()
{
}

void AbstractLayout::SetMargin (const Margin& margin)
{
  if (margin_ == margin) return;

  PerformMarginUpdate(margin);
}

Response BlendInt::AbstractLayout::Draw (AbstractWindow* context)
{
  return subview_count() ? Ignore : Finish;
}

void AbstractLayout::PerformMarginUpdate (const Margin& margin)
{
  margin_ = margin;
}

}
