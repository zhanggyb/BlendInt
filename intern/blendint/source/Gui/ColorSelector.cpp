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

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/LinearLayout.hpp>
#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/BrightnessSlider.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/RadioButton.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/Label.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	ColorSelector::ColorSelector()
	: PopupFrame(), stack_(0)
	{
		InitializeColorSelector();
	}

	ColorSelector::~ColorSelector()
	{
	}

	void ColorSelector::InitializeColorSelector()
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

		SetLayout(main_layout);
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
