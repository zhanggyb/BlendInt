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

#include <opengl/opengl.hpp>

#include <gui/dialog.hpp>
#include <gui/filesystem-model.hpp>

#include <gui/abstract-frame.hpp>
#include <gui/abstract-window.hpp>

#include <gui/combo-box.hpp>
#include <gui/menu.hpp>

namespace BlendInt {

  ComboBoxModel::ComboBoxModel()
  : AbstractListModel(),
    rows_(0),
    columns_(0)
  {

  }

  ComboBoxModel::~ComboBoxModel ()
  {
  }

  bool ComboBoxModel::InsertColumns (int column,
                                     int count,
                                     const ModelIndex& parent)
  {
    if (AbstractListModel::InsertColumns(column, count, parent)) {
      if(rows_ == 0) rows_++;

      columns_ += count;
      return true;
    }

    return false;
  }

  bool ComboBoxModel::RemoveColumns (int column,
                                     int count,
                                     const ModelIndex& parent)
  {
    if (AbstractListModel::RemoveColumns(column, count, parent)) {
      columns_ -= count;
      if (columns_ < 0) {
        rows_ = 0;
        columns_ = 0;
      }

      return true;
    }

    return false;
  }

  bool ComboBoxModel::InsertRows (int row, int count, const ModelIndex& parent)
  {
    if (AbstractListModel::InsertRows(row, count, parent)) {
      if (columns_ == 0) columns_++;
      rows_ += count;
      return true;
    }

    return false;
  }

  bool ComboBoxModel::RemoveRows (int row, int count, const ModelIndex& parent)
  {
    if (AbstractListModel::RemoveRows(row, count, parent)) {
      rows_ -= count;
      if (rows_ < 0) {
        rows_ = 0;
        columns_ = 0;
      }
      return true;
    }

    return false;
  }

  int ComboBoxModel::GetRowCount (const ModelIndex& parent) const
  {
    return rows_;
  }

  int ComboBoxModel::GetColumnCount (const ModelIndex& parent) const
  {
    return columns_;
  }

  void ComboBoxModel::SetIcon (const ModelIndex& index,
                               const RefPtr<AbstractIcon>& icon)
  {
    if (index.valid()) {
      if (index.GetLeftIndex().valid()) {
        DBG_PRINT_MSG("Error: %s", "the icon should be the first column");
      } else {
        set_index_data(index, icon);

        if (icon) {
          max_icon_size_.set_width(
              std::max(max_icon_size_.width(), icon->size().width()));
          max_icon_size_.set_height(
              std::max(max_icon_size_.height(), icon->size().height()));
        }
      }
    }
  }

  void ComboBoxModel::SetText (const ModelIndex& index,
                               const RefPtr<Text>& text)
  {
    if (index.valid()) {
      if (index.GetRightIndex().valid()) {
        DBG_PRINT_MSG("Error: %s", "the text should be the second column");
      } else {
        set_index_data(index, text);

        if (text) {
          max_text_size_.set_width(
              std::max(max_text_size_.width(), text->size().width()));
          max_text_size_.set_height(
              std::max(max_text_size_.height(), text->size().height()));
        }
      }
    }
  }

  // ----------------

  Margin ComboBox::kPadding = Margin(2, 2, 2, 2);

  ComboBox::ComboBox (DisplayMode display_mode)
  : AbstractRoundWidget(),
    status_down_(false),
    display_mode_(display_mode),
    last_round_status_(0),
    popup_(0)
  {
    set_round_type(RoundAll);
    last_round_status_ = round_type();

    Font font;	// default font
    int h = font.height();

    set_size(h + pixel_size(kPadding.hsum()), h + pixel_size(kPadding.vsum()));

    InitializeComboBox();
  }

  ComboBox::~ComboBox ()
  {
    glDeleteVertexArrays(2, vaos_);
  }

  Size ComboBox::GetPreferredSize () const
  {
    int w = 0;
    int h = 0;

    w = Font::default_height();
    h = Font::default_height();

    if (model_) {

      h = model_->GetFont().height();
      w = h;

      switch (display_mode_) {

        case IconMode: {

          w = std::max(w, model_->max_icon_size().width());
          h = std::max(h, model_->max_icon_size().height());

          break;
        }

        case TextMode: {

          w = model_->max_text_size().width();
          h = std::max(model_->max_text_size().height(), h);

          break;
        }

        case IconTextMode:
        case TextIconMode: {

          w = std::max(w, model_->max_icon_size().width()) + model_->max_text_size().width();
          h = std::max(model_->max_icon_size().height(),
                       model_->max_text_size().height());
          h = std::max(h, Font::default_height());

          break;
        }

        default: {

          w = std::max(w, model_->max_icon_size().width()) + model_->max_text_size().width();
          h = std::max(model_->max_icon_size().height(),
                       model_->max_text_size().height());
          h = std::max(h, Font::default_height());

          break;
        }

      }

    }

    w += AbstractWindow::icons()->menu()->size().width();

    w += pixel_size(kPadding.hsum());
    h += pixel_size(kPadding.vsum());

    return Size(w, h);
  }

  void ComboBox::SetModel (const RefPtr<ComboBoxModel>& model)
  {
    if (model_) {
      model_ = model;
      RequestRedraw();
    } else if (model) {
      model_ = model;
      RequestRedraw();
    }

    if (model_) {
      ModelIndex root = model_->GetRootIndex();
      current_index_ = root.GetChildIndex(0, 0);
    } else {
      current_index_ = ModelIndex();
    }
  }

  void ComboBox::SetCurrentIndex(int index)
  {
    if (model_ && index >= 0) {

      ModelIndex root = model_->GetRootIndex();

      if (index > (model_->GetRowCount(root) - 1)) {
        DBG_PRINT_MSG("%s", "invalid index");
        return;
      }

      ModelIndex tmp = root.GetChildIndex(0, 0);

      while ((index > 0) && tmp.valid()) {
        tmp = tmp.GetDownIndex();
        index--;
      }

      if (current_index_ != tmp) {
        current_index_ = tmp;
        RequestRedraw();
      }

    }
  }

  void ComboBox::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {

      set_size(*request.size());

      std::vector<GLfloat> inner_verts;
      std::vector<GLfloat> outer_verts;

      if (AbstractWindow::theme()->menu().shaded) {
        GenerateRoundedVertices(Vertical,
            AbstractWindow::theme()->menu().shadetop,
            AbstractWindow::theme()->menu().shadedown, &inner_verts,
            &outer_verts);
      } else {
        GenerateRoundedVertices(&inner_verts, &outer_verts);
      }

      vbo_.bind(0);
      vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
          &inner_verts[0]);
      vbo_.bind(1);
      vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
          &outer_verts[0]);
      vbo_.reset();

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  void ComboBox::PerformRoundTypeUpdate (int round_type)
  {
    set_round_type(round_type);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->menu().shaded) {
      GenerateRoundedVertices(Vertical, AbstractWindow::theme()->menu().shadetop,
          AbstractWindow::theme()->menu().shadedown, &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  void ComboBox::PerformRoundRadiusUpdate (float radius)
  {
    set_round_radius(radius);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->menu().shaded) {
      GenerateRoundedVertices(Vertical, AbstractWindow::theme()->menu().shadetop,
          AbstractWindow::theme()->menu().shadedown, &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  Response ComboBox::Draw (AbstractWindow* context)
  {
    // draw inner
    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
        1, AbstractWindow::theme()->menu().inner.data());

    if (status_down_) {
      glUniform1i(
          AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 20);
    } else {
      glUniform1i(
          AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    }
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
        context->theme()->menu().shaded);

    glBindVertexArray(vaos_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

    // draw outer:

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
        1, AbstractWindow::theme()->menu().outline.data());
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET), 0.f,
        0.f);

    glBindVertexArray(vaos_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
        GetOutlineVertices(round_type()) * 2 + 2);

//		if (emboss()) {
//			glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
//			        1.0f, 1.0f, 0.16f);
//			glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
//			        0.f, - 1.f);
//			glDrawArrays(GL_TRIANGLE_STRIP, 0,
//			        GetHalfOutlineVertices(round_type()) * 2);
//		}

    Rect rect(pixel_size(kPadding.left()),
              pixel_size(kPadding.bottom()),
              size().width() - pixel_size(kPadding.hsum()) - context->icons()->menu()->size().width(),
              size().height() - pixel_size(kPadding.vsum()));

    // draw model item
    if (model_ && current_index_.valid()) {

      const AbstractForm* icon = 0;
      const AbstractForm* text = 0;

      switch (display_mode_) {

        case IconMode: {

          icon = current_index_.GetRawData();
          if (icon) {
            icon->DrawInRect(rect, AlignCenter,
                             AbstractWindow::theme()->menu().text_sel.data());
          }

          break;
        }

        case TextMode: {

          ModelIndex next = current_index_.GetRightIndex();
          if (next.valid()) {
            text = next.GetRawData();
            if(text) {
              text->DrawInRect(
                  rect,
                  AlignCenter | AlignJustify | AlignBaseline,
                  AbstractWindow::theme()->menu().text_sel.data());
            }
          }

          break;
        }

        case IconTextMode: {

          rect.set_width(std::min(rect.height(), rect.width()));

          icon = current_index_.GetRawData();
          if (icon) {
            icon->DrawInRect(
                rect, AlignCenter,
                AbstractWindow::theme()->menu().text_sel.data());
          }

          rect.set_x(rect.x() + rect.width());
          rect.set_width(
              size().width() - pixel_size(kPadding.hsum())
                  - context->icons()->menu()->size().width() - rect.width());

          ModelIndex next = current_index_.GetRightIndex();
          if (next.valid()) {
            text = next.GetRawData();
            if (text) {
              text->DrawInRect(
                  rect,
                  AlignLeft | AlignJustify | AlignVerticalCenter | AlignBaseline,
                  AbstractWindow::theme()->menu().text_sel.data());
            }
          }

          break;
        }

        case TextIconMode: {

          ModelIndex next = current_index_.GetRightIndex();
          if (next.valid()) {
            text = next.GetRawData();
            if (text) {
              text->DrawInRect(
                  rect,
                  AlignLeft | AlignJustify | AlignVerticalCenter | AlignBaseline,
                  AbstractWindow::theme()->menu().text_sel.data());
              rect.cut_left(text->size().width());
            }

          }

          icon = current_index_.GetRawData();
          if (icon) {
            icon->DrawInRect(
                rect, AlignCenter,
                AbstractWindow::theme()->menu().text_sel.data());
          }

          break;
        }

        default: {

          rect.set_width(std::min(rect.height(), rect.width()));

          icon = current_index_.GetRawData();
          if (icon) {
            icon->DrawInRect(
                rect, AlignCenter,
                AbstractWindow::theme()->menu().text_sel.data());
          }

          rect.set_x(rect.x() + rect.width());
          rect.set_width(
              size().width() - pixel_size(kPadding.hsum())
                  - context->icons()->menu()->size().width() - rect.width());

          ModelIndex next = current_index_.GetRightIndex();
          if (next.valid()) {
            text = next.GetRawData();
            if (text) {
              text->DrawInRect(
                  rect,
                  AlignLeft | AlignJustify | AlignVerticalCenter | AlignBaseline,
                  AbstractWindow::theme()->menu().text_sel.data());
            }
          }

          break;
        }

      }
    }

    rect.set_x(pixel_size(kPadding.left()));
    rect.set_width(size().width() - pixel_size(kPadding.hsum()));

    AbstractWindow::icons()->menu()->DrawInRect(rect,
        AlignRight | AlignVerticalCenter, Color(0xEFEFEFFF).data());

    return Finish;
  }

  Response ComboBox::PerformMousePress (AbstractWindow* context)
  {
    status_down_ = true;

    if (popup_) {

      delete popup_;
      popup_ = 0;
      SetRoundType(last_round_status_);
      RequestRedraw();
      return Finish;

    } else {

      last_round_status_ = round_type();
      Menu* menu = new Menu;
      menu->AddAction(context->icons()->icon_16x16(Icons::IMAGEFILE), "Menu Item 1");
      menu->AddAction(context->icons()->icon_16x16(Icons::IMAGE_RGB), "Menu Item 2");
      menu->AddAction(context->icons()->icon_16x16(Icons::IMAGE_ZDEPTH), "Menu Item 3");

      popup_ = menu;
      popup_->Resize(popup_->GetPreferredSize());

      events()->connect(popup_->destroyed(), this,
          &ComboBox::OnPopupListDestroyed);

      Point pos = context->GetAbsolutePosition(this);

      int top = pos.y() + size().height() + popup_->size().height();
      int bottom = pos.y() - popup_->size().height();

      if (top <= context->size().height()) {  // pop up above this widget

        popup_->MoveTo(pos.x(), pos.y() + size().height());

        int round = round_type();
        CLRBIT(round, RoundTopLeft);
        CLRBIT(round, RoundTopRight);

        SetRoundType(round);

      } else {

        if (bottom >= 0) {  // pop up below this widget

          popup_->MoveTo(pos.x(), pos.y() - popup_->size().height());

          int round = round_type();
          CLRBIT(round, RoundBottomLeft);
          CLRBIT(round, RoundBottomRight);

          SetRoundType(round);

        } else {

          int diff = top - context->size().height() + bottom;
          if (diff <= 0) {
            popup_->MoveTo(pos.x(), pos.y() + size().height());
            SetRoundType(RoundBottomLeft | RoundBottomRight);
          } else {
            popup_->MoveTo(pos.x(), pos.y() - popup_->size().height());
            SetRoundType(RoundTopLeft | RoundTopRight);
          }

        }

      }

      context->AddFrame(popup_);
      return Ignore;

    }
  }

  Response ComboBox::PerformMouseRelease (AbstractWindow* context)
  {
    status_down_ = false;

    RequestRedraw();
    return Finish;
  }

  void ComboBox::InitializeComboBox ()
  {
    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->menu().shaded) {
      GenerateRoundedVertices(Vertical, AbstractWindow::theme()->menu().shadetop,
          AbstractWindow::theme()->menu().shadedown, &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.generate();

    glGenVertexArrays(2, vaos_);

    glBindVertexArray(vaos_[0]);
    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(vaos_[1]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    vbo_.reset();
  }

  void ComboBox::OnPopupListDestroyed (AbstractFrame* frame)
  {
    //DBG_ASSERT(frame == popup_);
    popup_->destroyed().disconnectOne(this, &ComboBox::OnPopupListDestroyed);
    popup_ = 0;
    SetRoundType(last_round_status_);
    status_down_ = false;
    RequestRedraw();
  }

}
