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

#include <math.h>

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/StockItems.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	ComboBox::ComboBox ()
	: RoundWidget(), m_vao(0), m_status_down(false)
	{
		set_round_type(RoundAll);
		set_expand_x(true);
		set_size(90, 20);

		InitOnce();
	}

	ComboBox::~ComboBox ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ComboBox::Update(const UpdateRequest& request)
	{
		if (request.source() == Predefined) {
			switch (request.type()) {

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					glBindVertexArray(m_vao);
					GenerateShadedFormBuffers(*size_p,
									round_type(),
									radius(),
									Theme::instance->menu().inner,
									Theme::instance->menu().shadetop,
									Theme::instance->menu().shadedown,
									Vertical,
									m_inner_buffer.get(),
									m_outer_buffer.get());
					glBindVertexArray(0);
					Refresh();
					break;
				}

				case FormRoundType: {
					const int* type_p = static_cast<const int*>(request.data());
					glBindVertexArray(m_vao);
					GenerateShadedFormBuffers(size(),
									*type_p,
									radius(),
									Theme::instance->menu().inner,
									Theme::instance->menu().shadetop,
									Theme::instance->menu().shadedown,
									Vertical,
									m_inner_buffer.get(),
									m_outer_buffer.get());
					glBindVertexArray(0);
					Refresh();
					break;
				}

				case FormRoundRadius: {
					const float* radius_p = static_cast<const float*>(request.data());
					glBindVertexArray(m_vao);
					GenerateShadedFormBuffers(size(),
									round_type(),
									*radius_p,
									Theme::instance->menu().inner,
									Theme::instance->menu().shadetop,
									Theme::instance->menu().shadedown,
									Vertical,
									m_inner_buffer.get(),
									m_outer_buffer.get());
					glBindVertexArray(0);
					Refresh();
					break;
				}

				default:
					RoundWidget::Update(request);
			}

		}
	}

	ResponseType ComboBox::Draw(const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(
						event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);

		if(m_status_down) {
			program->SetUniform1i("Gamma", 20);
		} else {
			if(hover()) {
				program->SetUniform1i("Gamma", 15);
			} else {
				program->SetUniform1i("Gamma", 0);
			}
		}

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		DrawShadedTriangleFan(0, 1, m_inner_buffer.get());

		glDisableVertexAttribArray(1);

		Theme* tm = Theme::instance;

		glm::vec4 color;
		color.r = tm->menu().outline.r() / 255.f;
		color.g = tm->menu().outline.g() / 255.f;
		color.b = tm->menu().outline.b() / 255.f;
		color.a = tm->menu().outline.a() / 255.f;

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
		program->SetUniform1i("AA", 1);
		program->SetUniform1i("Gamma", 0);

		DrawTriangleStrip(0, m_outer_buffer.get());

		glDisableVertexAttribArray(0);

		program->Reset();
		glBindVertexArray(0);

		RefPtr<VertexIcon> icon = StockItems::instance->icon_num();

		glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(1.15, 1.15, 1.15));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (glm::mediump_float)(M_PI * 1.5), glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(icon->size().width()/2.f, icon->size().height()/2.f, 0.0));

		icon->Draw(mvp * translate * rotate * scale);

		return Accept;
	}
	
	ResponseType ComboBox::MousePressEvent (const MouseEvent& event)
	{
		m_status_down = true;

		Context* context = GetContext();
		if(context) {
			if(m_menu->container()) {
				context->Remove(m_menu.get());
				SetRoundType(RoundAll);
			} else {
				int max_layer = context->GetMaxLayer();
				m_menu->SetLayer(max_layer + 1);
				m_menu->SetPosition(position().x(), position().y() + size().height());
				context->Add(m_menu.get());
				SetRoundType(RoundBottomLeft | RoundBottomRight);
				//context->SetFocusedWidget(m_menu.get());	// FIXME: no use, context will reset to this combobox.
			}

			Refresh();
		}

		return Accept;
	}
	
	ResponseType ComboBox::MouseReleaseEvent (const MouseEvent& event)
	{
		m_status_down = false;

		Refresh();
		return Accept;
	}
	
	ResponseType ComboBox::CursorEnterEvent (bool entered)
	{
		Refresh();
		return Accept;
	}

	void ComboBox::InitOnce()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		GenerateShadedFormBuffers(size(),
						round_type(),
						radius(),
						Theme::instance->menu().inner,
						Theme::instance->menu().shadetop,
						Theme::instance->menu().shadedown,
						Vertical,
						m_inner_buffer.get(),
						m_outer_buffer.get());

		glBindVertexArray(0);

		m_menu.reset(new Menu);

		m_menu->SetRoundType(RoundTopLeft | RoundTopRight);
		//m_menu->SetPosition(200, 200);
		//menu->Resize (200, 200);

		m_menu->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
		m_menu->AddActionItem("MenuItem2", "Ctrl + 1");
		m_menu->AddActionItem("MenuItem3", "Ctrl + 1");
		m_menu->AddActionItem("MenuItem4", "Ctrl + 1");
		m_menu->AddActionItem("MenuItem5");

		events()->connect(m_menu->triggered(), this, &ComboBox::OnMenuActionTriggered);
	}

	void ComboBox::OnMenuActionTriggered (ActionItem* item)
	{
		Context* context = GetContext();

		context->Remove(m_menu.get());
		SetRoundType(RoundAll);

		Refresh();
	}

}
