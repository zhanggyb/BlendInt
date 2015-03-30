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

ComboListModel::ComboListModel ()
    : AbstractListModel(), rows_(0), columns_(0)
{
}

ComboListModel::~ComboListModel ()
{
}

bool ComboListModel::InsertColumns (int column,
                                    int count,
                                    const ModelIndex& parent)
{
  if (AbstractListModel::InsertColumns(column, count, parent)) {
    if (rows_ == 0) rows_++;

    columns_ += count;
    return true;
  }

  return false;
}

bool ComboListModel::RemoveColumns (int column,
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

bool ComboListModel::InsertRows (int row, int count, const ModelIndex& parent)
{
  if (AbstractListModel::InsertRows(row, count, parent)) {
    if (columns_ == 0) columns_++;
    rows_ += count;
    return true;
  }

  return false;
}

bool ComboListModel::RemoveRows (int row, int count, const ModelIndex& parent)
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

int ComboListModel::GetRowCount (const ModelIndex& parent) const
{
  return rows_;
}

int ComboListModel::GetColumnCount (const ModelIndex& parent) const
{
  return columns_;
}

int ComboListModel::GetPreferredColumnWidth (int index,
                                             const ModelIndex& parent) const
{
  if (index == 0) {
    return max_icon_size_.width();
  }

  if (index == 1) {
    return max_text_size_.width();
  }

  return 80;
}

int ComboListModel::GetPreferredRowHeight (int index,
                                           const ModelIndex& parent) const
{
  return std::max(max_icon_size_.height(), Font::default_height());
}

void ComboListModel::SetIcon (const ModelIndex& index,
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

void ComboListModel::SetText (const ModelIndex& index, const RefPtr<Text>& text)
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

ComboListView::ComboListView ()
    : AbstractItemView(), highlight_index_(-1)
{
  set_size(240, 320);

  GLfloat h = (GLfloat) Font::default_height();
  GLfloat verts[] = { 0.f, 0.f, (GLfloat) size().width(), 0.f, 0.f, h,
      (GLfloat) size().width(), h };

  std::vector<GLfloat> inner_verts;

  GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);
  vbo_.generate();

  glGenVertexArrays(2, vao_);

  glBindVertexArray(vao_[0]);

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(vao_[1]);

  vbo_.bind(1);
  vbo_.set_data(sizeof(verts), verts);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

ComboListView::~ComboListView ()
{
  glDeleteVertexArrays(2, vao_);
}

bool ComboListView::IsExpandX () const
{
  return true;
}

bool ComboListView::IsExpandY () const
{
  return true;
}

Size ComboListView::GetPreferredSize () const
{
  if (model_) {
    ModelIndex root = model_->GetRootIndex();
    int columns = model_->GetColumnCount(root);
    int width = 0;
    for (int i = 0; i < columns; i++) {
      width += model_->GetPreferredColumnWidth(i, root);
    }

    int rows = model_->GetRowCount(root);
    int height = 0;
    for (int i = 0; i < rows; i++) {
      height += model_->GetPreferredRowHeight(i, root);
    }

    return Size(width, height);
  } else {
    return Size(240, 320);
  }
}

const RefPtr<AbstractItemModel> ComboListView::GetModel () const
{
  return model_;
}

void ComboListView::SetModel (const RefPtr<AbstractItemModel>& model)
{
  RefPtr<ComboListModel> combo_model = RefPtr<ComboListModel>::cast_dynamic(
      model);

  if (model_) {
    model_ = combo_model;
    RequestRedraw();
  } else if (combo_model) {
    model_ = combo_model;
    RequestRedraw();
  }

//    if (model_) {
//      int h = font_.height();
//      h = model_->GetRowCount() * h;  // total height
//    }
}

ModelIndex ComboListView::GetIndexAt (const Point& point) const
{
  return ModelIndex();
}

Response ComboListView::Draw (AbstractWindow* context)
{
  int y = size().height();
  const int h = Font::default_height();

  AbstractWindow::shaders()->widget_inner_program()->use();

  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA),
              0);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
              0);

  glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
               1, AbstractWindow::theme()->regular().inner.data());

  glBindVertexArray(vao_[0]);
  // glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  context->BeginPushStencil();  // inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);
  context->EndPushStencil();

  AbstractWindow::shaders()->widget_triangle_program()->use();

  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS),
      0);
  glVertexAttrib4f(AttributeColor, 0.475f, 0.475f, 0.475f, 0.75f);

  glBindVertexArray(vao_[1]);

  int i = 0;
  while (y > 0) {
    y -= h;

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_POSITION),
        0, y);

    if (i == highlight_index_) { // TODO: use different functions for performance
      glUniform1i(
          AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA),
          -35);
    } else {
      if (i % 2 == 0) {
        glUniform1i(
            AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA),
            0);
      } else {
        glUniform1i(
            AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA),
            15);
      }
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    i++;
  }

  RefPtr<AbstractItemModel> model = GetModel();
  if (model) {

    ModelIndex index = model->GetRootIndex();
    index = index.GetChildIndex(0, 0);

    Rect rect(0, size().height() - h, size().width(), h);

    while (index.valid()) {
      index.GetData()->DrawInRect(
          rect, AlignLeft | AlignVerticalCenter | AlignJustify | AlignBaseline,
          AbstractWindow::theme()->regular().text.data());
      index = index.GetDownIndex();
      rect.set_y(rect.y() - h);
    }

  }

  AbstractWindow::shaders()->widget_inner_program()->use();

  context->BeginPopStencil(); // pop inner stencil
  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);
  glBindVertexArray(0);
  context->EndPopStencil();

  return Finish;
}

void ComboListView::PerformSizeUpdate (const AbstractView* source,
                                       const AbstractView* target,
                                       int width,
                                       int height)
{
  if (target == this) {

    set_size(width, height);

    GLfloat h = Font::default_height();
    GLfloat verts[] = { 0.f, 0.f, (GLfloat) width, 0.f, 0.f, h, (GLfloat) width,
        h };

    vbo_.bind(1);
    vbo_.set_data(sizeof(verts), verts);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.reset();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

Response ComboListView::PerformMousePress (AbstractWindow* context)
{
  if (model_) {

    ModelIndex index;

    int rows = model_->GetRowCount();

    if (rows > 0) {
      int h = Font::default_height(); // the row height
      int total = rows * h;

      int i = 0;
      if (total > size().height()) {
        i = position().y() - context->GetGlobalCursorPosition().y();
      } else {  // no vbar
        i = position().y() + size().height()
            - context->GetGlobalCursorPosition().y();
      }

      i = i / h;
      highlight_index_ = i;

      index = model_->GetRootIndex().GetChildIndex();
      while ((i > 0) && index.valid()) {
        index = index.GetDownIndex();
        i--;
      }

      if (!index.valid()) {
        highlight_index_ = -1;
      }
    }

  } else {
    highlight_index_ = -1;
  }

  return Finish;
}

// ----------------

Margin ComboBox::kPadding = Margin(2, 2, 2, 2);

ComboBox::ComboBox (DisplayMode display_mode)
    :
      AbstractRoundWidget(),
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

        w = std::max(w, model_->max_icon_size().width())
            + model_->max_text_size().width();
        h = std::max(model_->max_icon_size().height(),
                     model_->max_text_size().height());
        h = std::max(h, Font::default_height());

        break;
      }

      default: {

        w = std::max(w, model_->max_icon_size().width())
            + model_->max_text_size().width();
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

void ComboBox::SetModel (const RefPtr<ComboListModel>& model)
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

void ComboBox::SetCurrentIndex (int index)
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

void ComboBox::PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->menu().shaded) {
      GenerateRoundedVertices(Vertical,
                              AbstractWindow::theme()->menu().shadetop,
                              AbstractWindow::theme()->menu().shadedown,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void ComboBox::PerformRoundTypeUpdate (int round_type)
{
  set_round_type(round_type);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->menu().shaded) {
    GenerateRoundedVertices(Vertical, AbstractWindow::theme()->menu().shadetop,
                            AbstractWindow::theme()->menu().shadedown,
                            &inner_verts, &outer_verts);
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
                            AbstractWindow::theme()->menu().shadedown,
                            &inner_verts, &outer_verts);
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
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
              context->theme()->menu().shaded);

  glBindVertexArray(vaos_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  // draw outer:

  AbstractWindow::shaders()->widget_outer_program()->use();

  glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, AbstractWindow::theme()->menu().outline.data());
  glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              0.f, 0.f);

  glBindVertexArray(vaos_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

//		if (emboss()) {
//			glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
//			        1.0f, 1.0f, 0.16f);
//			glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
//			        0.f, - 1.f);
//			glDrawArrays(GL_TRIANGLE_STRIP, 0,
//			        emboss_vertex_count(round_type()) * 2);
//		}

  Rect rect(
      pixel_size(kPadding.left()),
      pixel_size(kPadding.bottom()),
      size().width() - pixel_size(kPadding.hsum())
          - context->icons()->menu()->size().width(),
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
          if (text) {
            text->DrawInRect(rect, AlignCenter | AlignJustify | AlignBaseline,
                             AbstractWindow::theme()->menu().text_sel.data());
          }
        }

        break;
      }

      case IconTextMode: {

        rect.set_width(std::min(rect.height(), rect.width()));

        icon = current_index_.GetRawData();
        if (icon) {
          icon->DrawInRect(rect, AlignCenter,
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
          icon->DrawInRect(rect, AlignCenter,
                           AbstractWindow::theme()->menu().text_sel.data());
        }

        break;
      }

      default: {

        rect.set_width(std::min(rect.height(), rect.width()));

        icon = current_index_.GetRawData();
        if (icon) {
          icon->DrawInRect(rect, AlignCenter,
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
                                              AlignRight | AlignVerticalCenter,
                                              Color(0xEFEFEFFF).data());

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

    ComboListView* list = new ComboListView;
    list->SetModel(model_);
    menu->AddWidget(list);

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
                            AbstractWindow::theme()->menu().shadedown,
                            &inner_verts, &outer_verts);
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

} // namespace BlendInt
