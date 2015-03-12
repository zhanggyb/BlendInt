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
#include <gui/abstract-window.hpp>

#include <gui/combo-box.hpp>
#include <gui/menu.hpp>

namespace BlendInt {

  Margin ComboBox::kPadding = Margin(2, 2, 2, 2);

  ComboBox::ComboBox ()
  : AbstractRoundWidget(),
    status_down_(false),
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

    Font font;	// default font
    w = font.height();
    h = font.height();

    if (model_) {

      ModelIndex root = model_->GetRootIndex();
      ModelIndex index = model_->GetIndex(0, 0, root);

      if (index.valid()) {

        RefPtr<AbstractForm> data = index.GetData();
        w = std::max(w, data->size().width());
        h = std::max(h, data->size().height());

      }
    }

    w += AbstractWindow::icons()->menu()->size().width();

    w += pixel_size(kPadding.hsum());
    h += pixel_size(kPadding.vsum());

    return Size(w, h);
  }

  void ComboBox::SetModel (const RefPtr<AbstractItemModel>& model)
  {
    if (model_) {
      model_ = model;
      RequestRedraw();
    } else if (model) {
      model_ = model;
      RequestRedraw();
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

    glBindVertexArray(vaos_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

    // draw outer:

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
        1, AbstractWindow::theme()->menu().outline.data());
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION), 0.f,
        0.f);

    glBindVertexArray(vaos_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
        GetOutlineVertices(round_type()) * 2 + 2);

//		if (emboss()) {
//			glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
//			        1.0f, 1.0f, 0.16f);
//			glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION),
//			        0.f, - 1.f);
//			glDrawArrays(GL_TRIANGLE_STRIP, 0,
//			        GetHalfOutlineVertices(round_type()) * 2);
//		}

    Rect rect(pixel_size(kPadding.left()), pixel_size(kPadding.bottom()),
        size().width() - pixel_size(kPadding.hsum()),
        size().height() - pixel_size(kPadding.vsum()));

    // draw model item
    if (model_) {

      //int h = size().height() - pixel_size(kPadding.vsum());

      ModelIndex root = model_->GetRootIndex();
      ModelIndex index = model_->GetIndex(0, 0, root);

      if (index.valid()) {
        index.GetData()->DrawInRect(rect,
            AlignLeft | AlignJustify | AlignBaseline,
            AbstractWindow::theme()->menu().text_sel.data());
      }

    }

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
  }

}
