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
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/StockItems.hpp>

namespace BlendInt {

	ComboBox::ComboBox ()
	: RoundWidget()
	{
		InitOnce();
	}

	ComboBox::~ComboBox ()
	{
	}

	bool ComboBox::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateFormBuffer(size_p,
								   round_type(),
								   radius(),
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);

				m_shadow.Resize(*size_p);
				Refresh();
				return true;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				GenerateFormBuffer(&(size()),
								   *type_p,
								   radius(),
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);

				Refresh();
				return true;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				GenerateFormBuffer(&(size()),
								   round_type(),
								   *radius_p,
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);

				Refresh();
				return true;
			}

			default:
				return RoundWidget::Update(type, data);
		}
	}

	void ComboBox::Draw(RedrawEvent* event)
	{
		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event->pv_matrix(), pos);

		GLint xy_attrib = program->GetAttributeLocation("xy");

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib1f("z", (float)z());

		ThemeManager* tm = ThemeManager::instance();

		float r, g, b, a;

		r = tm->themes()->regular.inner.r() / 255.f;
		g = tm->themes()->regular.inner.g() / 255.f;
		b = tm->themes()->regular.inner.b() / 255.f;
		a = tm->themes()->regular.inner.a() / 255.f;

		program->SetVertexAttrib4f("color", r, g, b, a);

		glEnableVertexAttribArray(xy_attrib);

		// Describe our vertices array to OpenGL (it can't guess its format automatically)
		m_inner_buffer->Bind();

		glVertexAttribPointer(xy_attrib, // attribute
							  2,			// number of elements per vertex, here (x,y,z)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		// Push each element in buffer_vertices to the vertex shader
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

		glVertexAttribPointer(xy_attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			jitter.x = jit[j][0]; jitter.y = jit[j][1]; jitter.z = 0.0f;
			jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp * jitter_matrix));
			glDrawArrays(GL_QUAD_STRIP, 0, m_outer_buffer->GetBufferSize() / (2 * sizeof(GLfloat)));
		}

		m_outer_buffer->Reset();

		glDisableVertexAttribArray(xy_attrib);

		program->Reset();

		RefPtr<VertexIcon> icon = StockItems::instance->icon_num();

		icon->Draw(mvp);

		event->accept(this);
		return;
	}

	void ComboBox::InitOnce()
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		set_round_type(RoundAll);
		set_expand_x(true);
		set_size(90, 20);
		set_preferred_size(90, 20);

		GenerateFormBuffer(&size(), round_type(), radius(), m_inner_buffer.get(),
				m_outer_buffer.get(), 0);
	}

}
