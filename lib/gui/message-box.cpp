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

#include <glm/gtx/transform.hpp>

#include <blendint/gui/linear-layout.hpp>
#include <blendint/gui/close-button.hpp>
#include <blendint/gui/label.hpp>
#include <blendint/gui/font.hpp>

#include <blendint/gui/message-box.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

MessageBox::MessageBox (const String& title, const String& description)
    : AbstractDialog(), title_(0), text_(0), close_(0)
{
  title_ = new Label(title, AlignCenter);
  text_ = new Label(description, AlignCenter);
  close_ = new CloseButton;

  title_->SetForeground(theme()->menu_back().text_sel);
  text_->SetForeground(theme()->menu_back().text);

  close_->clicked().connect(this, &MessageBox::OnClose);

  LinearLayout* hlayout = new LinearLayout(Horizontal);
  hlayout->SetMargin(Margin(0, 0, 0, 0));
  hlayout->SetSpace(0);
  hlayout->AddWidget(close_);
  hlayout->AddWidget(title_);

  LinearLayout* vlayout = new LinearLayout(Vertical);
  vlayout->AddWidget(hlayout);
  vlayout->AddWidget(text_);

  vlayout->Resize(vlayout->GetPreferredSize());

  set_round_type(RoundAll);
  set_round_radius(5.f);
  set_size(vlayout->size());
  set_refresh(true);
  EnableViewBuffer();

  PushBackSubView(vlayout);

  projection_matrix_ = glm::ortho(0.f,
                                  pixel_size(size().width()),
                                  0.f,
                                  pixel_size(size().height()),
                                  100.f,
                                  -100.f);
  model_matrix_ = glm::mat3(1.f);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (theme()->menu_back().shaded) {
    GenerateRoundedVertices(Vertical,
                            theme()->menu_back().shadetop,
                            theme()->menu_back().shadedown,
                            &inner_verts, &outer_verts);
  } else {
    GenerateRoundedVertices(&inner_verts, &outer_verts);
  }

  glGenVertexArrays(2, vao_);
  vbo_.generate();

  glBindVertexArray(vao_[0]);

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3,
                        GL_FLOAT,
                        GL_FALSE, 0, 0);

  glBindVertexArray(vao_[1]);

  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
}

MessageBox::~MessageBox ()
{
  glDeleteVertexArrays(2, vao_);
}

void MessageBox::SetTitleFont (const BlendInt::Font& font)
{
  title_->SetFont(font);
}

void MessageBox::SetTextFont (const BlendInt::Font& font)
{
  text_->SetFont(font);
}

void MessageBox::PerformSizeUpdate (const AbstractView* source,
                                    const AbstractView* target,
                                    int width,
                                    int height)
{
  if (target == this) {

    set_size(width, height);

    projection_matrix_ = glm::ortho(0.f,
                                    pixel_size(size().width()),
                                    0.f,
                                    pixel_size(size().height()),
                                    100.f,
                                    -100.f);

    if (view_buffer()) {
      view_buffer()->Resize(size());
    }

    shadow_->Resize(size());

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (theme()->menu_back().shaded) {
      GenerateRoundedVertices(Vertical,
                              theme()->menu_back().shadetop,
                              theme()->menu_back().shadedown,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

    ResizeSubView(first(), size());

    RequestRedraw();

  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

bool MessageBox::PreDraw (AbstractWindow* context)
{
  DeclareActiveFrame(context, this);

  if (refresh() && view_buffer()) {
    RenderToTexture(this, context, projection_matrix_, model_matrix_,
                             view_buffer()->texture());
  }

  return true;
}

Response BlendInt::MessageBox::Draw (AbstractWindow* context)
{
  shadow_->Draw(position().x(), position().y());

  shaders()->frame_inner_program()->use();

  glUniform2f(
      shaders()->location(Shaders::FRAME_INNER_POSITION),
      pixel_size(position().x()),
      pixel_size(position().y()));
  glUniform1i(shaders()->location(Shaders::FRAME_INNER_GAMMA),
              0);
  glUniform4fv(
      shaders()->location(Shaders::FRAME_INNER_COLOR), 1,
      theme()->menu_back().inner.data());

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  if (view_buffer()) {

    shaders()->frame_image_program()->use();

    glUniform2f(
        shaders()->location(Shaders::FRAME_IMAGE_POSITION),
        pixel_size(position().x()),
        pixel_size(position().y()));
    glUniform1i(
        shaders()->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
    glUniform1i(
        shaders()->location(Shaders::FRAME_IMAGE_GAMMA), 0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    view_buffer()->Draw(0, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  } else {

    glViewport(pixel_size(position().x()),
               pixel_size(position().y()),
               pixel_size(size().width()),
               pixel_size(size().height()));

    shaders()->SetWidgetProjectionMatrix(projection_matrix_);
    shaders()->SetWidgetModelMatrix(model_matrix_);

    DrawSubViewsOnce(context);

    glViewport(0, 0, pixel_size(context->size().width()),
               pixel_size(context->size().height()));

  }

  shaders()->frame_outer_program()->use();

  glUniform2f(
      shaders()->location(Shaders::FRAME_OUTER_POSITION),
      pixel_size(position().x()),
      pixel_size(position().y()));
  glUniform4fv(
      shaders()->location(Shaders::FRAME_OUTER_COLOR), 1,
      theme()->menu_back().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

  return Finish;
}

void MessageBox::OnClose ()
{
  AbstractView* parent = super();
  Destroy(this);
  parent->RequestRedraw();
}

}
