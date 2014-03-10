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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	Button::Button ()
			: AbstractButton()
	{
		InitOnce();
	}

	Button::Button (const String& text)
			: AbstractButton()
	{
		InitOnce(text);
	}

	Button::~Button ()
	{
	}

	bool Button::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateFormBuffer(size_p, round_type(), radius(), m_inner_buffer.get(), m_outer_buffer.get(), m_emboss_buffer.get());

				Refresh();
				return true;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				GenerateFormBuffer(&(size()), *type_p, radius(), m_inner_buffer.get(), m_outer_buffer.get(), m_emboss_buffer.get());

				Refresh();
				return true;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				GenerateFormBuffer(&(size()), round_type(), *radius_p, m_inner_buffer.get(), m_outer_buffer.get(), m_emboss_buffer.get());

				Refresh();
				return true;
			}

			default:
				return AbstractButton::Update(type, data);
		}
	}

	void Button::Draw (RedrawEvent* event)
	{
		RefPtr<GLSLProgram> program = ShaderManager::instance->widget_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 model = glm::translate(glm::mat4(1.0), pos);

		GLint pos_location = program->GetAttributeLocation("xy");

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(event->pv_matrix() * model));
		program->SetVertexAttrib1f("z", (float)z());

		ThemeManager* tm = ThemeManager::instance();

		float r, g, b, a;

		// draw inner, simple fill
		if(down()) {
			r = tm->themes()->regular.inner_sel.r() / 255.0;
			g = tm->themes()->regular.inner_sel.g() / 255.0;
			b = tm->themes()->regular.inner_sel.b() / 255.0;
			a = tm->themes()->regular.inner_sel.a() / 255.0;
		} else {
			if(hover()) {
				r = tm->themes()->regular.inner.highlight_red() / 255.0;
				g = tm->themes()->regular.inner.highlight_green() / 255.0;
				b = tm->themes()->regular.inner.highlight_blue() / 255.0;
				a = tm->themes()->regular.inner.a() / 255.0;
			} else {
				r = tm->themes()->regular.inner.r() / 255.0;
				g = tm->themes()->regular.inner.g() / 255.0;
				b = tm->themes()->regular.inner.b() / 255.0;
				a = tm->themes()->regular.inner.a() / 255.0;
			}
		}

		program->SetVertexAttrib4f("color", r, g, b, a);

		glEnableVertexAttribArray(pos_location);

		// Describe our vertices array to OpenGL (it can't guess its format automatically)
		m_inner_buffer->Bind();

		glVertexAttribPointer(pos_location, // attribute
				2,            // number of elements per vertex, here (x,y,z)
				GL_FLOAT,          // the type of each element
				GL_FALSE,          // take our values as-is
				0,                 // no extra data between each position
				0                  // offset of first element
		);

		/*
		glEnableVertexAttribArray(m_attribute_v_color);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cube_colors);
		glVertexAttribPointer(m_attribute_v_color, // attribute
				3,            // number of elements per vertex, here (R,G,B)
				GL_FLOAT,          // the type of each element
				GL_FALSE,          // take our values as-is
				0,                 // no extra data between each position
				0                  // offset of first element
		);
		*/

		// Push each element in buffer_vertices to the vertex shader
		glVertexPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glDrawArrays(GL_POLYGON, 0, m_inner_buffer->GetBufferSize()/(2 * sizeof(GLfloat)));

		m_inner_buffer->Reset();

		GLfloat outline_color[4] = {themes()->regular.outline.r() / 255.f,
		        themes()->regular.outline.g() / 255.f,
		        themes()->regular.outline.b() / 255.f,
		        (themes()->regular.outline.a() / WIDGET_AA_JITTER) / 255.f
		};

		program->SetVertexAttrib4fv("color", outline_color);

		glm::vec3 jitter;
		glm::mat4 jitter_matrix;

		m_outer_buffer->Bind();

		glVertexAttribPointer(pos_location, // attribute
				2,            // number of elements per vertex, here (x,y)
				GL_FLOAT,          // the type of each element
				GL_FALSE,          // take our values as-is
				0,                 // no extra data between each position
				0                  // offset of first element
		);

		glVertexPointer(2, GL_FLOAT, 0, 0);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			jitter.x = jit[j][0]; jitter.y = jit[j][1]; jitter.z = 0.0f;
			jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(event->pv_matrix() * model * jitter_matrix));

			glDrawArrays(GL_QUAD_STRIP, 0, m_outer_buffer->GetBufferSize() / (2 * sizeof(GLfloat)));

			//jitter.x = -jit[j][0]; jitter.y = -jit[j][1]; jitter.z = 0.0f;
			//jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			//program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(event->pv_matrix() * model * jitter_matrix));
		}

		m_outer_buffer->Reset();

		program->SetVertexAttrib4f("color", 1.0f, 1.0f, 1.0f, 0.02f);

		m_emboss_buffer->Bind();

		glVertexAttribPointer(pos_location, // attribute
				2,            // number of elements per vertex, here (x,y)
				GL_FLOAT,          // the type of each element
				GL_FALSE,          // take our values as-is
				0,                 // no extra data between each position
				0                  // offset of first element
		);

		glVertexPointer(2, GL_FLOAT, 0, 0);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			jitter.x = jit[j][0]; jitter.y = jit[j][1]; jitter.z = 0.0f;
			jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(event->pv_matrix() * model * jitter_matrix));
			glDrawArrays(GL_QUAD_STRIP, 0, m_emboss_buffer->GetBufferSize() / (2 * sizeof(GLfloat)));
		}

		m_emboss_buffer->Reset();

		glDisableVertexAttribArray(pos_location);

		program->Reset();

		event->accept(this);
		return;

		//ThemeManager* tm = ThemeManager::instance();

		// draw inner, simple fill
		if(down()) {
			glColor4ub(tm->themes()->regular.inner_sel.r(),
					tm->themes()->regular.inner_sel.g(),
					tm->themes()->regular.inner_sel.b(),
					tm->themes()->regular.inner_sel.a());
		} else {
			if(hover()) {
				glColor4ub(tm->themes()->regular.inner.highlight_red(),
						tm->themes()->regular.inner.highlight_green(),
						tm->themes()->regular.inner.highlight_blue(),
						tm->themes()->regular.inner.a());
			} else {
				glColor4ub(tm->themes()->regular.inner.r(),
						tm->themes()->regular.inner.g(),
						tm->themes()->regular.inner.b(),
						tm->themes()->regular.inner.a());
			}
		}

		DrawInnerBuffer(m_inner_buffer.get());

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		DrawOutlineBuffer(m_outer_buffer.get());

		glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
		DrawOutlineBuffer(m_emboss_buffer.get());

		// Draw text
		if(text().size()) {
			FontCache::create(font())->print(origin().x(), origin().y(), text(), valid_text_length());
		}
	}

	void Button::InitOnce ()
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_emboss_buffer.reset(new GLArrayBuffer);

		set_round_type(RoundAll);
		set_expand_x(true);
		set_size(90, 20);
		set_preferred_size(90, 20);

		GenerateFormBuffer(&size(), round_type(), radius(), m_inner_buffer.get(),
				m_outer_buffer.get(), m_emboss_buffer.get());
	}

	void Button::InitOnce (const String& text)
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_emboss_buffer.reset(new GLArrayBuffer);

		set_round_type(RoundAll);
		SetExpandX(true);
		Resize(90, 20);
		SetText(text);
		SetPreferredSize(size());
	}

}
