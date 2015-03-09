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

#include <gui/flow-layout.hpp>
#include <gui/abstract-window.hpp>
#include <gui/node.hpp>

namespace BlendInt {

  Node::Node (AbstractLayout* layout)
  : AbstractNode(), layout_(0)
  {
    set_round_type(RoundAll);
    set_round_radius(10.f);

    if (layout == nullptr) {
      layout_ = Manage(new FlowLayout);
    } else {
      layout_ = layout;
    }

    PushBackSubView(layout_);
    set_size(layout_->size());

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(&inner_verts, &outer_verts);

    glGenVertexArrays(2, vao_);
    vbo_.generate();

    glBindVertexArray(vao_[0]);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(vao_[1]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    vbo_.reset();

    shadow_.reset(new WidgetShadow(size(), round_type(), round_radius()));
  }

  Node::~Node ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  bool Node::AddWidget (AbstractWidget* widget)
  {
    return layout_->AddWidget(widget);
  }

  bool Node::InsertWidget (int index, AbstractWidget* widget)
  {
    return layout_->InsertWidget(index, widget);
  }

  bool Node::IsExpandX () const
  {
    return layout_->IsExpandX();
  }

  bool Node::IsExpandY () const
  {
    return layout_->IsExpandY();
  }

  Size Node::GetPreferredSize () const
  {
    return layout_->GetPreferredSize();
  }

  void Node::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {

      set_size(*request.size());

      std::vector<GLfloat> inner_verts;
      std::vector<GLfloat> outer_verts;

      GenerateRoundedVertices(&inner_verts, &outer_verts);

      vbo_.bind(0);
      vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                        &inner_verts[0]);
      vbo_.bind(1);
      vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
                        &outer_verts[0]);
      vbo_.reset();

      ResizeSubView(layout_, size());

      shadow_->Resize(size());

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  void Node::PerformRoundTypeUpdate (int round)
  {
    set_round_type(round);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->regular().shaded) {
      GenerateRoundedVertices(Vertical,
                              AbstractWindow::theme()->regular().shadetop,
                              AbstractWindow::theme()->regular().shadedown,
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

  void Node::PerformRoundRadiusUpdate (float radius)
  {
    set_round_radius(radius);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->regular().shaded) {
      GenerateRoundedVertices(Vertical,
                              AbstractWindow::theme()->regular().shadetop,
                              AbstractWindow::theme()->regular().shadedown,
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

  Response Node::Draw (AbstractWindow* context)
  {
    shadow_->Draw(0.f, 0.f);

    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        AbstractWindow::theme()->regular().inner.data());

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION),
        0.f, 0.f);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        AbstractWindow::theme()->regular().outline.data());

    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
                 GetOutlineVertices(round_type()) * 2 + 2);

    glBindVertexArray(0);
    GLSLProgram::reset();

    return subs_count() ? Ignore : Finish;
  }

}
