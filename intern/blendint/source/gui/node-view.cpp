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

#include <glm/gtx/matrix_transform_2d.hpp>

#include <gui/node-view.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  NodeView::NodeView ()
  : AbstractScrollable(),
    vao_(0),
    pressed_(false),
    focused_(false),
    hover_(false),
    mouse_tracking_record_(false)
  {
    set_size(400, 300);

    InitializeNodeView();
//		curve_ = new CubicBezierCurve;
//		curve_->Unpack();
  }

  NodeView::NodeView (int width, int height)
      : AbstractScrollable(width, height), vao_(0), pressed_(false)
  {

    InitializeNodeView();
//		curve_ = new CubicBezierCurve;
//		curve_->Unpack();
  }

  NodeView::~NodeView ()
  {
//		delete curve_;
    glDeleteVertexArrays(1, &vao_);
  }

  bool NodeView::AddNode (AbstractNode* node)
  {
    AbstractWindow* win = AbstractWindow::GetWindow(this);

    if (PushBackSubView(node)) {

      if (win) {
        if (node->previous_view()) {
          dynamic_cast<AbstractNode*>(node->previous_view())->PerformFocusOff(
              win);
        }
        node->PerformFocusOn(win);
      }

      RequestRedraw();
      return true;
    }

    return false;
  }

  bool NodeView::InsertNode (int index, AbstractNode* node)
  {
    AbstractWindow* win = AbstractWindow::GetWindow(this);

    if (InsertSubView(index, node)) {

      if (win) {

        if (node->next_view() == 0) { // push back
          if (node->previous_view()) {
            dynamic_cast<AbstractNode*>(node->previous_view())->PerformFocusOff(
                win);
          }
          node->PerformFocusOn(win);
        }

      }

      RequestRedraw();
      return true;
    }

    return false;
  }

  bool NodeView::IsExpandX () const
  {
    return true;
  }

  bool NodeView::IsExpandY () const
  {
    return true;
  }

  Size NodeView::GetPreferredSize () const
  {
    return Size(500, 400);
  }

  void NodeView::SetFocusedNode (AbstractNode* node)
  {
    if (node == 0) return;

    if (last_subview() == node) return;

    if (node->superview() == 0) {
      DBG_PRINT_MSG("%s", "the node is not in this view");
      return;
    }

    AbstractWindow* win = AbstractWindow::GetWindow(this);

    // if node is not the root node in this view, find and switch to it
    if (node->superview() != this) {

      AbstractView* tmp = node;
      NodeView* node_view = 0;

      while (tmp->superview()) {
        node_view = dynamic_cast<NodeView*>(tmp->superview());
        if (node_view) break;
        tmp = tmp->superview();
      }

      if (node_view == 0) {
        DBG_PRINT_MSG("%s", "the node is not in this view");
        return;
      }

      if (node_view != this) {
        DBG_PRINT_MSG("%s", "the node is not in this view");
        return;
      }

      node = dynamic_cast<AbstractNode*>(tmp);
      if (node == 0) return;
    }

    if (last_subview()) {
      dynamic_cast<AbstractNode*>(last_subview())->PerformFocusOff(win);
    }

    MoveToLast(node);
    node->PerformFocusOn(win);

    RequestRedraw();
  }

  NodeView* NodeView::GetNodeView (AbstractNode* node)
  {
    AbstractView* parent = node->superview();
    while (parent && is_node(parent)) {
      parent = parent->superview();
    }

    return dynamic_cast<NodeView*>(parent);
  }

  bool NodeView::SizeUpdateTest (const SizeUpdateRequest& request)
  {
    return true;
  }

  bool NodeView::PositionUpdateTest (const PositionUpdateRequest& request)
  {
    return true;
  }

  void NodeView::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {
      set_size(*request.size());

      std::vector<GLfloat> inner_verts;
      GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                       0);

      vbo_.bind();
      vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                        &inner_verts[0]);
      vbo_.reset();

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  void NodeView::PerformRoundTypeUpdate (int type)
  {
    set_round_type(type);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                     0);

    vbo_.bind();
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  void NodeView::PerformRoundRadiusUpdate (float radius)
  {
    set_round_radius(radius);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                     0);

    vbo_.bind();
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  void NodeView::PerformFocusOn (AbstractWindow* context)
  {
    DBG_PRINT_MSG("%s", "Focused");
    focused_ = true;

    if(hover_) {
      mouse_tracking_record_ = context->mouse_tracking();
      context->set_mouse_tracking(true);
    }
  }

  void NodeView::PerformFocusOff (AbstractWindow* context)
  {
    DBG_PRINT_MSG("%s", "UnFocused");
    focused_ = false;

    if(hover_) {
      context->set_mouse_tracking(mouse_tracking_record_);
    }
  }

  void NodeView::PerformHoverIn (AbstractWindow* context)
  {
    hover_ = true;

    if(focused_) {
      mouse_tracking_record_ = context->mouse_tracking();
      context->set_mouse_tracking(true);
    }
  }

  void NodeView::PerformHoverOut (AbstractWindow* context)
  {
    hover_ = false;

    if(focused_ && context)
      context->set_mouse_tracking(mouse_tracking_record_);
  }

  Response NodeView::PerformKeyPress (AbstractWindow* context)
  {
    Response response = Ignore;

    for (AbstractView* p = last_subview(); p; p = p->previous_view()) {
      response = dynamic_cast<AbstractNode*>(p)->PerformKeyPress(context);
       if (response == Finish) break;
    }

    return response;
  }

  Response NodeView::PerformMousePress (AbstractWindow* context)
  {
    pressed_ = true;

    PerformMouseHover(context);

    Response response = Ignore;
    Point local_position = context->local_cursor_position();
    for (AbstractView* p = last_subview(); p; p = p->previous_view()) {
      response = dynamic_cast<AbstractNode*>(p)->PerformMousePress(context);
      context->set_local_cursor_position(local_position);
      if (response == Finish) break;
    }

    return Finish;
  }

  Response NodeView::PerformMouseRelease (AbstractWindow* context)
  {
    pressed_ = false;

    PerformMouseHover(context);

    Response response = Ignore;
    Point local_position = context->local_cursor_position();
    for (AbstractView* p = last_subview(); p; p = p->previous_view()) {
      response = dynamic_cast<AbstractNode*>(p)->PerformMouseRelease(context);
      context->set_local_cursor_position(local_position);
      if (response == Finish) {
        break;
      }
    }

    return Finish;
  }

  Response NodeView::PerformMouseMove (AbstractWindow* context)
  {
    PerformMouseHover(context);

    if (pressed_) {

      Response response = Ignore;
      AbstractNode* node = 0;
      Point local_position = context->local_cursor_position();

      for (AbstractView* p = last_subview(); p; p = p->previous_view()) {
        node = dynamic_cast<AbstractNode*>(p);
        response = node->PerformMouseMove(context);
        context->set_local_cursor_position(local_position);
        if (response == Finish) {
          break;
        }
      }

    }

    return Finish;
  }

  bool NodeView::PreDraw (AbstractWindow* context)
  {
    if (!visiable()) return false;

    glm::mat3 matrix = glm::translate(
        AbstractWindow::shaders()->widget_model_matrix(),
        glm::vec2(position().x(), position().y()));

    AbstractWindow::shaders()->PushWidgetModelMatrix();
    AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform4f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
        0.208f, 0.208f, 0.208f, 1.f);

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    context->BeginPushStencil();	// inner stencil
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    context->EndPushStencil();

    return true;
  }

  Response NodeView::Draw (AbstractWindow* context)
  {
//		curve_->Draw();

    if (subs_count()) {

      Point offset = GetOffset();

      glm::mat3 matrix = glm::translate(
          AbstractWindow::shaders()->widget_model_matrix(),
          glm::vec2(offset.x(), offset.y()));

      AbstractWindow::shaders()->PushWidgetModelMatrix();
      AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

      return Ignore;

    } else {
      return Finish;
    }
  }

  void NodeView::PostDraw (AbstractWindow* context)
  {
    if (subs_count()) {
      AbstractWindow::shaders()->PopWidgetModelMatrix();
    }

    // draw mask
    AbstractWindow::shaders()->widget_inner_program()->use();

    glBindVertexArray(vao_);

    context->BeginPopStencil();	// pop inner stencil
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    context->EndPopStencil();

    AbstractWindow::shaders()->PopWidgetModelMatrix();
  }

  void NodeView::PerformMouseHover (AbstractWindow* context)
  {
    try {

      Response response = Ignore;
      Point local_position = context->local_cursor_position();
      for (AbstractView* p = last_subview(); p; p = p->previous_view()) {
        response = dynamic_cast<AbstractNode*>(p)->PerformMouseHover(context);
        context->set_local_cursor_position(local_position);
        if (response == Finish) break;
      }

    } catch (std::bad_cast& e) {

      DBG_PRINT_MSG("Error: %s", "Only AbstractNode should be added in NodeView");
      exit(EXIT_FAILURE);

    }
  }

  void NodeView::InitializeNodeView ()
  {
    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                     0);

    vbo_.generate();
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);

    vbo_.bind();
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 3,
    GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindVertexArray(0);
    vbo_.reset();

  }

}
