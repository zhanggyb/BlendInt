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

#include <cassert>

#include <core/types.hpp>
#include <opengl/opengl.hpp>
#include <gui/table-layout.hpp>
#include <gui/table-adjustment.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  Cell::Cell ()
  : AbstractWidget()
  {
    set_size(10, 10);
  }

  Cell::~Cell ()
  {
  }

  void Cell::SetWidget (AbstractWidget* widget)
  {
    if (subview_count()) {
      ClearSubViews();
      RequestRedraw();
    }

    if (PushBackSubView(widget)) {
      ResizeSubView(widget, size());
    }
  }

  bool Cell::IsExpandX () const
  {
    return subview_count() ? first()->IsExpandX() : false;
  }

  bool Cell::IsExpandY () const
  {
    return subview_count() ? first()->IsExpandY() : false;
  }

  Size Cell::GetPreferredSize () const
  {
    Size preferred_size(10, 10);

    if (subview_count()) {
      preferred_size = first()->GetPreferredSize();
    }

    return preferred_size;
  }

  void Cell::PerformSizeUpdate (const AbstractView* source,
                                const AbstractView* target,
                                int width,
                                int height)
  {
    if (target == this) {

      set_size(width, height);

      if (subview_count()) {
        ResizeSubView(first(), size());
        RequestRedraw();
      }
    }

    if (source == this) {
      report_size_update(source, target, width, height);
    }
  }

  Response Cell::Draw (AbstractWindow* context)
  {
    return subview_count() ? Ignore : Finish;
  }

  // --------------------------------------

  TableLayout::TableLayout (unsigned int row, unsigned int column, int space)
  : AbstractLayout(),
    row_(row),
    column_(column),
    space_(space)
  {
    int width = 0;
    int height = 0;

    int row_width = 0;
    int row_height = 0;

    int x = pixel_size(margin().left());
    int y = pixel_size(margin().bottom());

    Cell* cell = 0;
    for (unsigned int i = 0; i < row_; i++) {

      row_width = 0;
      row_height = 0;

      for (unsigned int j = 0; j < column_; j++) {

        cell = Manage(new Cell);
        PushBackSubView(cell);

        MoveSubViewTo(cell, j * cell->size().width() + x,
                      (row_ - i - 1) * cell->size().height() + y);

        row_width += cell->size().width();
        height = std::max(height, cell->size().height());

      }

      width = std::max(width, row_width);
      height += row_height;

    }

    width += pixel_size(margin().hsum());
    height += pixel_size(margin().vsum());

    set_size(width, height);
  }

  TableLayout::TableLayout (int width,
                            int height,
                            unsigned int row,
                            unsigned int column,
                            const Margin& margin,
                            int space)
      :
        AbstractLayout(width, height, margin),
        row_(row),
        column_(column),
        space_(space)
  {
    int cell_width = (size().width() - (column_ - 1) * space_) / column_;
    int cell_height = (size().height() - (row_ - 1) * space_) / row_;

    int x = pixel_size(this->margin().left());
    int y = pixel_size(this->margin().bottom());

    Cell* cell = 0;
    for (unsigned int i = 0; i < row_; i++) {

      for (unsigned int j = 0; j < column_; j++) {

        cell = Manage(new Cell);
        PushBackSubView(cell);

        ResizeSubView(cell, cell_width, cell_height);
        MoveSubViewTo(cell, j * cell->size().width() + x,
                      (row_ - i - 1) * cell->size().height() + y);
      }
    }
  }

  TableLayout::~TableLayout ()
  {
  }

  bool TableLayout::AddWidget (AbstractWidget* widget)
  {
    bool retval = false;

    for (AbstractView* p = first(); p; p = next(p)) {
      if (p->GetSubViewCount() == 0) {
        Cell* cell = dynamic_cast<Cell*>(p);
        DBG_ASSERT(cell);
        cell->SetWidget(widget);
        Adjust();
        retval = true;
        break;
      }
    }

    if (!retval) {
      DBG_PRINT_MSG("Error: %s", "layout is full");
    }

    return retval;
  }

  bool TableLayout::InsertWidget (int index, AbstractWidget* widget)
  {
    if (index < subview_count()) {
      Cell* cell = dynamic_cast<Cell*>(GetSubViewAt(index));
      DBG_ASSERT(cell);
      cell->SetWidget(widget);
      Adjust();
      return true;
    } else {
      DBG_PRINT_MSG("Error: %s", "index out of range");
      return false;
    }
  }

  bool TableLayout::InsertWidget (int row, int column, AbstractWidget* widget)
  {
    int index = column_ * row + column;

    Cell* cell = dynamic_cast<Cell*>(GetSubViewAt(index));

    DBG_ASSERT(cell);
    cell->SetWidget(widget);

    Adjust();
    return true;
  }

  void TableLayout::Adjust ()
  {
    int x = pixel_size(margin().left());
    int y = pixel_size(margin().bottom());
    int w = size().width() - pixel_size(margin().hsum());
    int h = size().height() - pixel_size(margin().vsum());

    TableAdjustment adjust(this, row_, column_, space_);
    adjust.Adjust(x, y, w, h);
  }

  void TableLayout::SetSpace (int space)
  {
    if (space_ == space) return;

    space_ = space;
    Adjust();
    RequestRedraw();
  }

  bool TableLayout::IsExpandX () const
  {
    for (AbstractView* p = first(); p; p = next(p)) {
      if (p->IsExpandX()) return true;
    }

    return false;
  }

  bool TableLayout::IsExpandY () const
  {
    for (AbstractView* p = first(); p; p = next(p)) {
      if (p->IsExpandY()) return true;
    }

    return false;
  }

  void TableLayout::PerformSizeUpdate (const AbstractView* source,
                                       const AbstractView* target,
                                       int width,
                                       int height)
  {
    if (target == this) {

      set_size(width, height);
      Adjust();
    }

    if (source == this) {
      report_size_update(source, target, width, height);
    }
  }

  void TableLayout::PerformMarginUpdate (const Margin& margin)
  {
    set_margin(margin);

    Adjust();
    RequestRedraw();
  }

  Size TableLayout::GetPreferredSize () const
  {
    int width = 0;
    int height = 0;

    int row_width = 0;
    int row_height = 0;

    unsigned int i = 0;	// row
    unsigned int j = 0;	// column
    Size tmp;

    for (AbstractView* p = first(); p; p = next(p)) {

      tmp = p->GetPreferredSize();

      row_width += tmp.width();
      row_height = std::max(row_height, tmp.height());

      j++;
      if (j == column_) {

        width = std::max(width, row_width);
        row_width = 0;
        height += row_height;

        j = 0;
        i++;
      }
    }

    if (column_ > 0) {
      width += (column_ - 1) * space_;
    }

    if (row_ > 0) {
      height += (row_ - 1) * space_;
    }

    width += pixel_size(margin().hsum());
    height += pixel_size(margin().vsum());

    return Size(width, height);
  }

}

