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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/BrightnessSlider.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/Label.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Icons.hpp>

#include <BlendInt/Gui/SingleStack.hpp>

namespace BlendInt {

	using Stock::Icons;

	ColorSelector::ColorSelector()
	: VLayout (), stack_(0)
	{
		set_size(200, 320);
		set_round_type(RoundAll);
		set_drop_shadow(true);
		set_margin(4, 4, 4, 4);

		InitializeColorSelector();
	}

	ColorSelector::~ColorSelector()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	void ColorSelector::InitializeColorSelector()
	{
		VertexTool tool;
		tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(), round_radius());

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);

		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		HLayout* hbox1 = Manage(new HLayout);
		ColorWheel* colorwheel = Manage(new ColorWheel);
		BrightnessSlider* br_slider = Manage(new BrightnessSlider(Vertical));

		hbox1->PushBack(colorwheel);
		hbox1->PushBack(br_slider);

		ToggleButton* btn1 = Manage(new ToggleButton("RGB"));
		ToggleButton* btn2 = Manage(new ToggleButton("HSV"));
		ToggleButton* btn3 = Manage(new ToggleButton("Hex"));

		radio_group_.PushBack(btn1);
		radio_group_.PushBack(btn2);
		radio_group_.PushBack(btn3);

		btn1->SetChecked(true);

		HBlockLayout* btn_block = Manage(new HBlockLayout);
		btn_block->SetMargin(4, 4, 4, 4);
		btn_block->PushBack(btn1);
		btn_block->PushBack(btn2);
		btn_block->PushBack(btn3);

		stack_ = CreateBlockStack();
		NumericalSlider* alpha_slider = Manage(new NumericalSlider);
		alpha_slider->SetEmboss(true);

		VLayout* color_box = Manage(new VLayout);
		color_box->SetMargin(0, 0, 0, 0);
		color_box->PushBack(stack_);
		color_box->PushBack(alpha_slider);

		Button* pick_btn = Manage(new Button(Icons::instance->icon_16x16(Stock::EYEDROPPER)));
		pick_btn->SetEmboss(true);

		HLayout* hbox2 = Manage(new HLayout(AlignTop));
		hbox2->PushBack(color_box);
		hbox2->PushBack(pick_btn);

		PushBack(hbox1);
		PushBack(btn_block);
		PushBack(hbox2);

		events()->connect(radio_group_.button_index_toggled(), this, &ColorSelector::OnButtonToggled);
	}

	void ColorSelector::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(), round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			set_size(*request.size());

			VLayout::PerformSizeUpdate(request);
			return;	// return to avoid double report of size update
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ColorSelector::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), *request.round_type(), round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			set_round_type(*request.round_type());
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void ColorSelector::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(), *request.round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			set_round_radius(*request.round_radius());
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType ColorSelector::Draw (Profile& profile)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();

		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4fv("a_color", Theme::instance->menu().inner.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		program->SetVertexAttrib4fv("a_color", Theme::instance->menu().outline.data());
		program->SetUniform1i("u_AA", 1);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		program->reset();

		return Ignore;
	}
	
	VBlockLayout* ColorSelector::CreateRGBBlock ()
	{
		VBlockLayout* block = Manage(new VBlockLayout);

		NumericalSlider* red_slider = Manage(new NumericalSlider);
		NumericalSlider* green_slider = Manage(new NumericalSlider);
		NumericalSlider* blue_slider = Manage(new NumericalSlider);

		block->PushBack(red_slider);
		block->PushBack(green_slider);
		block->PushBack(blue_slider);

		return block;
	}
	
	VBlockLayout* ColorSelector::CreateHSVBlock ()
	{
		VBlockLayout* block = Manage(new VBlockLayout);

		NumericalSlider* h_slider = Manage(new NumericalSlider);
		NumericalSlider* s_slider = Manage(new NumericalSlider);
		NumericalSlider* v_slider = Manage(new NumericalSlider);

		block->PushBack(h_slider);
		block->PushBack(s_slider);
		block->PushBack(v_slider);

		return block;
	}
	
	VLayout* ColorSelector::CreateHexBlock ()
	{
		VLayout* box = Manage(new VLayout(AlignLeft, 0));
		box->SetMargin(2, 2, 2, 2);

		TextEntry* hex_edit = Manage(new TextEntry);
		Label* label = Manage(new Label("Gamma Corrected"));

		box->PushBack(hex_edit);
		box->PushBack(label);

		return box;
	}
	
	void ColorSelector::OnButtonToggled (int index, bool toggled)
	{
		stack_->SetIndex(index);
		Refresh();
	}

	SingleStackLayout* ColorSelector::CreateBlockStack()
	{
		SingleStackLayout* stack = Manage(new SingleStackLayout);
		stack->SetMargin(0, 0, 0, 0);

		VBlockLayout* rgb_block = CreateRGBBlock();
		VBlockLayout* hsv_block = CreateHSVBlock();
		VLayout* hex_box = CreateHexBlock();

		stack->PushBack(rgb_block);
		stack->PushBack(hsv_block);
		stack->PushBack(hex_box);

		return stack;
	}

}
