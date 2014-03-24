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
 * License along with BlendInt.	 If not, see
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

#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {
	
	MenuButton::MenuButton (const String& text)
	: AbstractButton(), m_vao(0)
	{
		set_text(text);

		InitOnce();
	}
	
	MenuButton::~MenuButton ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	bool MenuButton::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {
			switch (request.type()) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(*size_p,
								   round_type(),
								   radius(),
								   m_inner.get(),
								   0,
								   0);
				glBindVertexArray(0);
				Refresh();
				return true;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   *type_p,
								   radius(),
								   m_inner.get(),
								   0,
								   0);
				glBindVertexArray(0);
				Refresh();
				return true;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   round_type(),
								   *radius_p,
								   m_inner.get(),
								   0,
								   0);
				glBindVertexArray(0);
				Refresh();
				return true;
			}

			default:
				return AbstractButton::Update(request);
			}

		} else {
			return false;
		}
	}
	
	void MenuButton::Draw (RedrawEvent* event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event->projection_matrix() * event->view_matrix(), pos);

		if (hover()) {
			glBindVertexArray(m_vao);

			RefPtr<GLSLProgram> program =
							ShaderManager::instance->default_widget_program();
			program->Use();

			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
							glm::value_ptr(mvp));
			program->SetVertexAttrib1f("z", (float) z());

			ThemeManager* tm = ThemeManager::instance();

			glm::vec4 color(tm->themes()->menu_item.inner_sel.r() / 255.f,
							tm->themes()->menu_item.inner_sel.g() / 255.f,
							tm->themes()->menu_item.inner_sel.b() / 255.f,
							tm->themes()->menu_item.inner_sel.a()
											/ WIDGET_AA_JITTER / 255.f);

			program->SetVertexAttrib4fv("color", glm::value_ptr(color));

			glEnableVertexAttribArray(0);

			DrawTriangleFan(program, mvp, 0, m_inner.get());

			glDisableVertexAttribArray(0);
			program->Reset();

			glBindVertexArray(0);
		}

		if (text().size()) {
			font().Print(mvp, origin().x(), origin().y(), text(),
							text_length(), 0);
		}

		event->accept(this);
	}
	
	void MenuButton::InitOnce ()
	{
		set_round_type(RoundAll);
		set_expand_x(true);
		set_size(40, 20);
		set_preferred_size(90, 20);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner.reset(new GLArrayBuffer);

		GenerateFormBuffer(
						size(),
						round_type(),
						radius(),
						m_inner.get(),
						0,
						0);

		glBindVertexArray(0);
	}

} /* namespace BlendInt */
