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
#include <BlendInt/Gui/HBox.hpp>
#include <BlendInt/Gui/VBox.hpp>
#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/BrightnessSlider.hpp>

#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/Label.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	ColorSelector::ColorSelector()
	: VBox (), m_stack(0)
	{
		set_size(200, 320);
		set_round_corner_type(RoundAll);
		set_drop_shadow(true);
		set_margin(4, 4, 4, 4);

		InitializeColorSelector();
	}

	ColorSelector::~ColorSelector()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void ColorSelector::InitializeColorSelector()
	{
		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());

		glGenVertexArrays(2, m_vao);

		glBindVertexArray(m_vao[0]);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[1]);

		m_outer.reset(new GLArrayBuffer);
		m_outer->Generate();
		m_outer->Bind();
		tool.SetOuterBufferData(m_outer.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

		HBox* hbox1 = Manage(new HBox);
		ColorWheel* colorwheel = Manage(new ColorWheel);
		BrightnessSlider* br_slider = Manage(new BrightnessSlider(Vertical));

		hbox1->PushBack(colorwheel);
		hbox1->PushBack(br_slider);

		ToggleButton* btn1 = Manage(new ToggleButton("RGB"));
		ToggleButton* btn2 = Manage(new ToggleButton("HSV"));
		ToggleButton* btn3 = Manage(new ToggleButton("Hex"));
		btn1->SetChecked(true);

		m_radio_group.Add(btn1);
		m_radio_group.Add(btn2);
		m_radio_group.Add(btn3);

		HBlock* btn_block = Manage(new HBlock);
		btn_block->SetMargin(4, 4, 4, 4);
		btn_block->PushBack(btn1);
		btn_block->PushBack(btn2);
		btn_block->PushBack(btn3);

		m_stack = CreateBlockStack();
		NumericalSlider* alpha_slider = Manage(new NumericalSlider);
		alpha_slider->SetEmboss(true);

		VBox* color_box = Manage(new VBox);
		color_box->SetMargin(0, 0, 0, 0);
		color_box->PushBack(m_stack);
		color_box->PushBack(alpha_slider);

		ToolButton* pick_btn = Manage(new ToolButton);
		pick_btn->SetEmboss(true);

		HBox* hbox2 = Manage(new HBox(AlignTop));
		hbox2->PushBack(color_box);
		hbox2->PushBack(pick_btn);

		PushBack(hbox1);
		//vbox->PushBack(colorwheel);
		PushBack(btn_block);
		PushBack(hbox2);

		events()->connect(m_radio_group.button_index_toggled(), this, &ColorSelector::OnButtonToggled);
	}

	void ColorSelector::UpdateGeometry(const GeometryUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					VertexTool tool;
					tool.Setup(*size_p, DefaultBorderWidth(), round_corner_type(), round_corner_radius());
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_outer->Bind();
					tool.SetOuterBufferData(m_outer.get());
					set_size(*size_p);
					break;
				}

				case WidgetRoundCornerType: {
					const int* type_p = static_cast<const int*>(request.data());
					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(), *type_p, round_corner_radius());
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_outer->Bind();
					tool.SetOuterBufferData(m_outer.get());
					set_round_corner_type(*type_p);
					break;
				}

				case WidgetRoundCornerRadius: {
					const float* radius_p = static_cast<const float*>(request.data());
					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(), round_corner_type(), *radius_p);
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_outer->Bind();
					tool.SetOuterBufferData(m_outer.get());
					set_round_corner_radius(*radius_p);
					break;
				}

				default:
					break;
			}

			VBox::UpdateGeometry(request);
			return;
		}

		ReportGeometryUpdate(request);
	}

	ResponseType ColorSelector::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(), 0.f);
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4fv("Color", Theme::instance->menu().inner.data());

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);

		program->SetVertexAttrib4fv("Color", Theme::instance->menu().outline.data());
		program->SetUniform1i("AA", 1);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		glBindVertexArray(0);
		program->Reset();

		return Accept;
	}
	
	VBlock* ColorSelector::CreateRGBBlock ()
	{
		VBlock* block = Manage(new VBlock);

		NumericalSlider* red_slider = Manage(new NumericalSlider);
		NumericalSlider* green_slider = Manage(new NumericalSlider);
		NumericalSlider* blue_slider = Manage(new NumericalSlider);

		block->PushBack(red_slider);
		block->PushBack(green_slider);
		block->PushBack(blue_slider);

		return block;
	}
	
	VBlock* ColorSelector::CreateHSVBlock ()
	{
		VBlock* block = Manage(new VBlock);

		NumericalSlider* h_slider = Manage(new NumericalSlider);
		NumericalSlider* s_slider = Manage(new NumericalSlider);
		NumericalSlider* v_slider = Manage(new NumericalSlider);

		block->PushBack(h_slider);
		block->PushBack(s_slider);
		block->PushBack(v_slider);

		return block;
	}
	
	VBox* ColorSelector::CreateHexBlock ()
	{
		VBox* box = Manage(new VBox(AlignLeft, 0));
		box->SetMargin(2, 2, 2, 2);

		TextEntry* hex_edit = Manage(new TextEntry);
		Label* label = Manage(new Label("Gamma Corrected"));

		box->PushBack(hex_edit);
		box->PushBack(label);

		return box;
	}
	
	void ColorSelector::OnButtonToggled (int index, bool toggled)
	{
		m_stack->SetIndex(index);
		Refresh();
	}

	Stack* ColorSelector::CreateBlockStack()
	{
		Stack* stack = Manage(new Stack);
		stack->SetMargin(0, 0, 0, 0);

		VBlock* rgb_block = CreateRGBBlock();
		VBlock* hsv_block = CreateHSVBlock();
		VBox* hex_box = CreateHexBlock();

		stack->Add(rgb_block);
		stack->Add(hsv_block);
		stack->Add(hex_box);

		return stack;
	}

}
