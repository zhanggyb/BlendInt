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

#include <blendint/gui/tab-button.hpp>

#include <blendint/gui/tab.hpp>
#include <blendint/gui/tab-header.hpp>
#include <blendint/stock/shaders.hpp>
#include <blendint/gui/stack.hpp>

namespace BlendInt {

Tab::Tab ()
    : AbstractWidget()
{
  set_size(400, 300);

  TabHeader* header = new TabHeader;
  DBG_SET_NAME(header, "TabHeader");
  Stack* stack = new Stack;
  DBG_SET_NAME(stack, "Stack");

  PushBackSubView(header);	// 0
  PushBackSubView(stack);	// 1

  FillSubWidgetsInTab(size());

  header->button_index_toggled().connect(this, &Tab::OnButtonToggled);
}

Tab::~Tab ()
{
}

void Tab::AddWidget (const String& title, AbstractWidget* widget)
{
  TabButton* btn = new TabButton(title);
  DBG_SET_NAME(btn, ConvertFromString(title).c_str());

  TabHeader* header = dynamic_cast<TabHeader*>(GetSubViewAt(0));
  Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

  header->AddButton(btn);
  stack->AddWidget(widget);

  if (header->GetSubViewCount() == 1) {
    btn->SetChecked(true);
  }
}

bool Tab::IsExpandX () const
{
  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandX()) return true;
  }

  return false;
}

bool Tab::IsExpandY () const
{
  for (AbstractView* p = first(); p; p = next(p)) {
    if (p->IsExpandY()) return true;
  }

  return false;
}

Size Tab::GetPreferredSize () const
{
  int w = 0;
  int h = 0;
  Size tmp;
  for (AbstractView* p = first(); p; p = next(p)) {
    tmp = p->GetPreferredSize();
    // DBG_PRINT_MSG("width: %d, height: %d", tmp.width(), tmp.height());    
    w = std::max(w, tmp.width());
    h += tmp.height();
  }

  return Size(w, h);
}

int Tab::GetIndex () const
{
  Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

  return stack->GetIndex();
}

void Tab::PerformSizeUpdate (const AbstractView* source,
                             const AbstractView* target,
                             int width,
                             int height)
{
  if (target == this) {
    set_size(width, height);
    FillSubWidgetsInTab(size());
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void Tab::OnButtonToggled (int index, bool toggled)
{
  Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

  stack->SetIndex(index);
  RequestRedraw();
}

void Tab::FillSubWidgetsInTab (const Size& out_size)
{
  int x = 0;
  int y = 0;
  int w = out_size.width();
  int h = out_size.height();

  FillSubWidgetsInTab(x, y, w, h);
}

Response Tab::Draw (AbstractWindow* context)
{
  return subview_count() ? Ignore : Finish;
}

void Tab::FillSubWidgetsInTab (int x, int y, int w, int h)
{
  int header_y = h;

  TabHeader* header = dynamic_cast<TabHeader*>(GetSubViewAt(0));
  Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

  Size header_size = header->GetPreferredSize();

  if (header_size.height() > h) {
    //header->SetVisible(false);
    //stack->SetVisible(false);

    return;
  } else {
    //header->SetVisible(true);
    //stack->SetVisible(true);
  }

  header_y = header_y - header_size.height();
  MoveSubViewTo(header, x, header_y);
  ResizeSubView(header, w, header_size.height());

  MoveSubViewTo(stack, x, y);
  ResizeSubView(stack, w, h - header_size.height());
}

}  // namespace BlendInt
