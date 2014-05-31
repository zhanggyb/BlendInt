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

#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/HBox.hpp>
#include <BlendInt/Gui/VBox.hpp>
#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Gui/NumberSlider.hpp>
#include <BlendInt/Gui/Slider.hpp>

#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	ColorSelector::ColorSelector()
	: Frame ()
	{
		set_size(200, 320);
		set_round_corner_type(RoundAll);
		set_drop_shadow(true);

		InitializeColorSelector();
	}

	ColorSelector::~ColorSelector()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ColorSelector::InitializeColorSelector()
	{
		m_inner.reset(new GLArrayBuffer);
		m_outer.reset(new GLArrayBuffer);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		GenerateFormBuffer(size(), round_corner_type(), round_corner_radius(),
		        m_inner.get(), m_outer.get(), 0);

		glBindVertexArray(0);

		VBox* vbox = Manage(new VBox);

		HBox* hbox1 = Manage(new HBox);
		ColorWheel* colorwheel = Manage(new ColorWheel);
		Slider* alpha_slider = Manage(new Slider(Vertical));

		hbox1->PushBack(colorwheel);
		hbox1->PushBack(alpha_slider);

		NumberSlider* red_slider = Manage(new NumberSlider);
		NumberSlider* green_slider = Manage(new NumberSlider);
		NumberSlider* blue_slider = Manage(new NumberSlider);

		vbox->PushBack(hbox1);
		//vbox->PushBack(colorwheel);
		vbox->PushBack(red_slider);
		vbox->PushBack(green_slider);
		vbox->PushBack(blue_slider);

		Setup(vbox);
	}

	void ColorSelector::UpdateGeometry(const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				GenerateFormBuffer(*size_p, round_corner_type(), round_corner_radius(),
				        m_inner.get(), m_outer.get(), 0);
				break;
			}

			case WidgetRoundCornerType: {
				const int* type_p = static_cast<const int*>(request.data());
				GenerateFormBuffer(size(), *type_p, round_corner_radius(),
				        m_inner.get(), m_outer.get(), 0);
				break;
			}

			case WidgetRoundCornerRadius: {
				const int* radius_p = static_cast<const int*>(request.data());
				GenerateFormBuffer(size(), round_corner_type(), *radius_p,
				        m_inner.get(), m_outer.get(), 0);
				break;
			}

			default:
				break;
		}

		return Frame::UpdateGeometry(request);
	}

	ResponseType ColorSelector::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4fv("Color", Theme::instance->menu().inner.data());

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_inner.get());

		program->SetVertexAttrib4fv("Color", Theme::instance->menu().outline.data());
		program->SetUniform1i("AA", 1);

		DrawTriangleStrip(0, m_outer.get());

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		return Accept;
	}

}
