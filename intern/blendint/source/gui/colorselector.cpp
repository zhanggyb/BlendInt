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

#include <gui/colorselector.hpp>
#include <gui/linear-layout.hpp>
#include <gui/color-wheel.hpp>
#include <gui/numerical-slider.hpp>
#include <gui/brightness-slider.hpp>

#include <gui/button.hpp>
#include <gui/radio-button.hpp>
#include <gui/tool-button.hpp>
#include <gui/textentry.hpp>
#include <gui/label.hpp>

#include <gui/abstract-window.hpp>

namespace BlendInt {

	ColorSelector::ColorSelector()
	: AbstractDialog(),
	  stack_(0)
	{
		LinearLayout* main_layout = Manage(new LinearLayout(Vertical));

		LinearLayout* hbox1 = Manage(new LinearLayout);
		ColorWheel* colorwheel = Manage(new ColorWheel);
		BrightnessSlider* br_slider = Manage(new BrightnessSlider(Vertical));

		hbox1->AddWidget(colorwheel);
		hbox1->AddWidget(br_slider);

		RadioButton* btn1 = Manage(new RadioButton("RGB"));
		RadioButton* btn2 = Manage(new RadioButton("HSV"));
		RadioButton* btn3 = Manage(new RadioButton("Hex"));

		radio_group_.AddButton(btn1);
		radio_group_.AddButton(btn2);
		radio_group_.AddButton(btn3);

		btn1->SetChecked(true);

		Block* btn_block = Manage(new Block(Horizontal));
		btn_block->AddWidget(btn1);
		btn_block->AddWidget(btn2);
		btn_block->AddWidget(btn3);

		stack_ = CreateBlockStack();
		NumericalSlider* alpha_slider = Manage(new NumericalSlider);
		alpha_slider->SetEmboss(true);

		LinearLayout* color_box = Manage(new LinearLayout(Vertical));
		color_box->SetMargin(Margin(0, 0, 0, 0));
		color_box->AddWidget(stack_);
		color_box->AddWidget(alpha_slider);

		Button* pick_btn = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::EYEDROPPER)));
		pick_btn->SetEmboss(true);

		LinearLayout* hbox2 = Manage(new LinearLayout(Horizontal, AlignTop));
		hbox2->AddWidget(color_box);
		hbox2->AddWidget(pick_btn);

		main_layout->AddWidget(hbox1);
		main_layout->AddWidget(btn_block);
		main_layout->AddWidget(hbox2);

		events()->connect(radio_group_.button_index_toggled(), this, &ColorSelector::OnButtonToggled);

		main_layout->Resize(main_layout->GetPreferredSize());

		PushBackSubView(main_layout);

		set_round_type(RoundAll);
		set_round_radius(5.f);
		set_size(main_layout->size());
		EnableViewBuffer();
		set_refresh(true);

		projection_matrix_  = glm::ortho(
				0.f, (float)size().width(),
				0.f, (float)size().height(),
				100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->menu_back().shadetop,
					AbstractWindow::theme->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
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
		GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
	}

	ColorSelector::~ColorSelector()
	{
        glDeleteVertexArrays(2, vao_);
	}

	void ColorSelector::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
    	if(request.target() == this) {

    		set_size(*request.size());

    		projection_matrix_  = glm::ortho(
    				0.f,
    				0.f + (float)size().width(),
    				0.f,
    				0.f + (float)size().height(),
    				100.f, -100.f);

    		if(buffer()) {
    			buffer()->Resize(size());
    		}

    		shadow_->Resize(size());

    		std::vector<GLfloat> inner_verts;
    		std::vector<GLfloat> outer_verts;

    		if (AbstractWindow::theme->menu_back().shaded) {
    			GenerateRoundedVertices(Vertical,
    					AbstractWindow::theme->menu_back().shadetop,
    					AbstractWindow::theme->menu_back().shadedown,
    					&inner_verts,
    					&outer_verts);
    		} else {
    			GenerateRoundedVertices(&inner_verts, &outer_verts);
    		}

    		vbo_.bind(0);
    		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    		vbo_.bind(1);
    		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    		vbo_.reset();

    		ResizeSubView(first_subview(), size());

    		RequestRedraw();

    	}

    	if(request.source() == this) {
    		ReportSizeUpdate(request);
    	}
	}

	bool ColorSelector::PreDraw (AbstractWindow* context)
	{
		if(!visiable()) return false;

		context->register_active_frame(this);

		if(refresh() && buffer()) {
			RenderSubFramesToTexture(this,
					context,
					projection_matrix_,
					model_matrix_,
					buffer()->texture());
		}

		return true;
	}

	Response ColorSelector::Draw (AbstractWindow* context)
	{
    	shadow_->Draw(position().x(), position().y());

		AbstractWindow::shaders->frame_inner_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::FRAME_INNER_COLOR), 1, AbstractWindow::theme->menu_back().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		if(buffer()) {

			AbstractWindow::shaders->frame_image_program()->use();

			glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_POSITION), position().x(), position().y());
			glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
			glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_IMAGE_GAMMA), 0);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			buffer()->Draw();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		}

		AbstractWindow::shaders->frame_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		glUniform4fv(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COLOR), 1, AbstractWindow::theme->menu_back().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		return Finish;
	}

	Block* ColorSelector::CreateRGBBlock ()
	{
		Block* block = Manage(new Block(Vertical));

		NumericalSlider* red_slider = Manage(new NumericalSlider);
		NumericalSlider* green_slider = Manage(new NumericalSlider);
		NumericalSlider* blue_slider = Manage(new NumericalSlider);

		block->AddWidget(red_slider);
		block->AddWidget(green_slider);
		block->AddWidget(blue_slider);

		return block;
	}
	
	Block* ColorSelector::CreateHSVBlock ()
	{
		Block* block = Manage(new Block(Vertical));

		NumericalSlider* h_slider = Manage(new NumericalSlider);
		NumericalSlider* s_slider = Manage(new NumericalSlider);
		NumericalSlider* v_slider = Manage(new NumericalSlider);

		block->AddWidget(h_slider);
		block->AddWidget(s_slider);
		block->AddWidget(v_slider);

		return block;
	}
	
	LinearLayout* ColorSelector::CreateHexBlock ()
	{
		LinearLayout* box = Manage(new LinearLayout(Vertical, AlignLeft, 0));
		box->SetMargin(Margin(2, 2, 2, 2));

		TextEntry* hex_edit = Manage(new TextEntry);
		Label* label = Manage(new Label("Gamma Corrected"));

		box->AddWidget(hex_edit);
		box->AddWidget(label);

		return box;
	}
	
	void ColorSelector::OnButtonToggled (int index, bool toggled)
	{
		stack_->SetIndex(index);
		RequestRedraw();
	}

	Stack* ColorSelector::CreateBlockStack()
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