/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <glm/gtx/transform.hpp>

#include <blendint/gui/color-selector.hpp>
#include <blendint/gui/linear-layout.hpp>
#include <blendint/gui/color-wheel.hpp>
#include <blendint/gui/numerical-slider.hpp>
#include <blendint/gui/brightness-slider.hpp>

#include <blendint/gui/push-button.hpp>
#include <blendint/gui/radio-button.hpp>
#include <blendint/gui/tool-button.hpp>
#include <blendint/gui/textentry.hpp>
#include <blendint/gui/label.hpp>

#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

ColorSelector::ColorSelector ()
    : AbstractDialog(), stack_(0)
{
  LinearLayout* main_layout = Manage(new LinearLayout(Vertical));

  LinearLayout* hbox1 = Manage(new LinearLayout);
  ColorWheel* colorwheel = Manage(new ColorWheel);
  BrightnessSlider* br_slider = Manage(new BrightnessSlider(Vertical));

  hbox1->AddWidget(colorwheel);
  hbox1->AddWidget(br_slider);

  ColorModeButton* btn1 = Manage(new ColorModeButton("RGB"));
  ColorModeButton* btn2 = Manage(new ColorModeButton("HSV"));
  ColorModeButton* btn3 = Manage(new ColorModeButton("Hex"));

  radio_group_.AddButton(btn1);
  radio_group_.AddButton(btn2);
  radio_group_.AddButton(btn3);

  btn1->SetChecked(true);

  Block* btn_block = Manage(new Block(Horizontal));
  btn_block->AddWidget(btn1);
  btn_block->AddWidget(btn2);
  btn_block->AddWidget(btn3);

  stack_ = CreateBlockStack();
  NumericalSlider* alpha_slider = Manage(new NumericalSlider("A:"));
  alpha_slider->SetMinimum(0.0);
  alpha_slider->SetMaximum(1.0);
  alpha_slider->SetValue(1.0);
  alpha_slider->SetEmboss(true);

  LinearLayout* color_box = Manage(new LinearLayout(Vertical));
  color_box->SetMargin(Margin(0, 0, 0, 0));
  color_box->AddWidget(stack_);
  color_box->AddWidget(alpha_slider);

  PushButton* pick_btn = Manage(
      new PushButton(AbstractWindow::icons()->icon_16x16(Icons::EYEDROPPER)));
  pick_btn->SetEmboss(true);

  LinearLayout* hbox2 = Manage(new LinearLayout(Horizontal, AlignTop));
  hbox2->AddWidget(color_box);
  hbox2->AddWidget(pick_btn);

  main_layout->AddWidget(hbox1);
  main_layout->AddWidget(btn_block);
  main_layout->AddWidget(hbox2);

  radio_group_.button_index_toggled().connect(this,
                    &ColorSelector::OnButtonToggled);

  main_layout->Resize(main_layout->GetPreferredSize());

  PushBackSubView(main_layout);

  set_round_type(RoundAll);
  set_round_radius(5.f);
  set_size(main_layout->size());
  set_refresh(true);
  EnableViewBuffer();

  projection_matrix_ = glm::ortho(0.f, (float) size().width(), 0.f,
                                  (float) size().height(), 100.f, -100.f);
  model_matrix_ = glm::mat3(1.f);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  if (AbstractWindow::theme()->menu_back().shaded) {
    GenerateRoundedVertices(Vertical,
                            AbstractWindow::theme()->menu_back().shadetop,
                            AbstractWindow::theme()->menu_back().shadedown,
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

ColorSelector::~ColorSelector ()
{
  glDeleteVertexArrays(2, vao_);
}

void ColorSelector::PerformSizeUpdate (const AbstractView* source,
                                       const AbstractView* target,
                                       int width,
                                       int height)
{
  if (target == this) {

    set_size(width, height);

    projection_matrix_ = glm::ortho(0.f, 0.f + (float) size().width(), 0.f,
                                    0.f + (float) size().height(), 100.f,
                                    -100.f);

    if (view_buffer()) {
      view_buffer()->Resize(size());
    }

    shadow_->Resize(size());

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->menu_back().shaded) {
      GenerateRoundedVertices(Vertical,
                              AbstractWindow::theme()->menu_back().shadetop,
                              AbstractWindow::theme()->menu_back().shadedown,
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

bool ColorSelector::PreDraw (AbstractWindow* context)
{
  DeclareActiveFrame(context, this);

  if (refresh() && view_buffer()) {
    RenderSubFramesToTexture(this, context, projection_matrix_, model_matrix_,
                             view_buffer()->texture());
  }

  return true;
}

Response ColorSelector::Draw (AbstractWindow* context)
{
  shadow_->Draw(position().x(), position().y());

  AbstractWindow::shaders()->frame_inner_program()->use();

  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::FRAME_INNER_POSITION),
      position().x(), position().y());
  glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_GAMMA),
              0);
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_COLOR),
               1, AbstractWindow::theme()->menu_back().inner.data());

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  if (view_buffer()) {

    AbstractWindow::shaders()->frame_image_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_POSITION),
        position().x(), position().y());
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
    glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_GAMMA),
                0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    view_buffer()->Draw(0, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  } else {

    glViewport(position().x(), position().y(), size().width(), size().height());

    AbstractWindow::shaders()->SetWidgetProjectionMatrix(projection_matrix_);
    AbstractWindow::shaders()->SetWidgetModelMatrix(model_matrix_);

    DrawSubViewsOnce(context);

    glViewport(0, 0, context->size().width(), context->size().height());

  }

  AbstractWindow::shaders()->frame_outer_program()->use();

  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_POSITION),
      position().x(), position().y());
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_COLOR),
               1, AbstractWindow::theme()->menu_back().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(round_type()) * 2 + 2);

  return Finish;
}

Block* ColorSelector::CreateRGBBlock ()
{
  Block* block = Manage(new Block(Vertical));

  NumericalSlider* red_slider = Manage(new NumericalSlider("R:"));
  red_slider->SetMinimum(0.0);
  red_slider->SetMaximum(1.0);
  red_slider->SetValue(1.0);

  NumericalSlider* green_slider = Manage(new NumericalSlider("G:"));
  green_slider->SetMinimum(0.0);
  green_slider->SetMaximum(1.0);
  green_slider->SetValue(1.0);

  NumericalSlider* blue_slider = Manage(new NumericalSlider("B:"));
  blue_slider->SetMinimum(0.0);
  blue_slider->SetMaximum(1.0);
  blue_slider->SetValue(1.0);

  block->AddWidget(red_slider);
  block->AddWidget(green_slider);
  block->AddWidget(blue_slider);

  return block;
}

Block* ColorSelector::CreateHSVBlock ()
{
  Block* block = Manage(new Block(Vertical));

  NumericalSlider* h_slider = Manage(new NumericalSlider("H:"));
  h_slider->SetMinimum(0.0);
  h_slider->SetMaximum(1.0);
  h_slider->SetValue(1.0);

  NumericalSlider* s_slider = Manage(new NumericalSlider("S:"));
  s_slider->SetMinimum(0.0);
  s_slider->SetMaximum(1.0);
  s_slider->SetValue(1.0);

  NumericalSlider* v_slider = Manage(new NumericalSlider("V:"));
  v_slider->SetMinimum(0.0);
  v_slider->SetMaximum(1.0);
  v_slider->SetValue(1.0);

  block->AddWidget(h_slider);
  block->AddWidget(s_slider);
  block->AddWidget(v_slider);

  return block;
}

LinearLayout* ColorSelector::CreateHexBlock ()
{
  LinearLayout* box = Manage(new LinearLayout(Vertical, AlignLeft, 0));
  box->SetMargin(Margin(0, 0, 0, 0));

  TextEntry* hex_edit = Manage(new TextEntry);
  hex_edit->SetEmboss(true);
  Label* label = Manage(new Label("Gamma Corrected"));
  label->SetForeground(Color(Palette::Gray));

  box->AddWidget(hex_edit);
  box->AddWidget(label);

  return box;
}

void ColorSelector::OnButtonToggled (int index, bool toggled)
{
  stack_->SetIndex(index);
  RequestRedraw();
}

Stack* ColorSelector::CreateBlockStack ()
{
  Stack* stack = Manage(new Stack);

  Block* rgb_block = CreateRGBBlock();
  Block* hsv_block = CreateHSVBlock();
  LinearLayout* hex_box = CreateHexBlock();

  stack->AddWidget(rgb_block);
  stack->AddWidget(hsv_block);
  stack->AddWidget(hex_box);

  return stack;
}

}
