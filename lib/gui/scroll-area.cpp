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

#include <blendint/gui/scroll-area.hpp>
#include <blendint/gui/abstract-scrollable.hpp>
#include <blendint/gui/table-layout.hpp>
#include <blendint/gui/abstract-window.hpp>
#include <blendint/gui/scroll-bar.hpp>

namespace BlendInt {

ScrollArea::ScrollArea ()
    : AbstractWidget(),
      layout_(nullptr)
{
  set_size(360, 240);

  layout_ = Manage(new TableLayout(2, 2));

  ScrollBar* vbar = Manage(new ScrollBar(Vertical));
  ScrollBar* hbar = Manage(new ScrollBar(Horizontal));

  PushBackSubView(layout_);
  ResizeSubView(layout_, size());

  layout_->InsertWidget(0, 1, vbar);
  layout_->InsertWidget(1, 0, hbar);
}

ScrollArea::ScrollArea (int width, int height, const Margin& margin, int space)
    : AbstractWidget(width, height),
      layout_(nullptr)
{
  layout_ = Manage(new TableLayout(width, height, 2, 2, margin, space));

  ScrollBar* vbar = Manage(new ScrollBar(Vertical));
  ScrollBar* hbar = Manage(new ScrollBar(Horizontal));

  PushBackSubView(layout_);

  layout_->InsertWidget(0, 1, vbar);
  layout_->InsertWidget(1, 0, hbar);
}

ScrollArea::~ScrollArea ()
{
}

void ScrollArea::SetScrollableWidget (AbstractScrollable* widget)
{
  layout_->InsertWidget(0, 0, widget);
}

bool ScrollArea::IsExpandX() const
{
  return layout_->IsExpandX();
}

bool ScrollArea::IsExpandY() const
{
  return layout_->IsExpandY();
}

Size ScrollArea::GetPreferredSize () const
{
  return layout_->GetPreferredSize();
}

void ScrollArea::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
{
  if(target == this) {

    set_size(width, height);
    ResizeSubView(layout_, size());

  }

  if(source == this) {
    report_size_update(source, target, width, height);
  }
}

Response ScrollArea::Draw (AbstractWindow* context)
{
  return Ignore;
}

}
