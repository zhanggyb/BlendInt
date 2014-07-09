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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Gui/ProgressBar.hpp>

namespace BlendInt {

	ProgressBar::ProgressBar(Orientation orientation)
	: AbstractWidget(),
	  m_orientation(orientation)
	{
		if(orientation == Horizontal) {
			set_size(200, 20);
		} else {
			set_size(20, 200);
		}

		glGenVertexArrays(2, m_vao);

		InitializeProgressBar();
	}

	ProgressBar::~ProgressBar ()
	{
		glDeleteVertexArrays(2, m_vao);
	}
	
	bool ProgressBar::IsExpandX () const
	{
		if(m_orientation == Horizontal) {
			return true;
		} else {
			return false;
		}
	}
	
	bool ProgressBar::IsExpandY () const
	{
		if(m_orientation == Vertical) {
			return true;
		} else {
			return false;
		}
	}
	
	Size ProgressBar::GetPreferredSize () const
	{
		Size prefer;

		return prefer;
	}

	bool ProgressBar::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		return true;
	}

	void ProgressBar::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if(request.target() == this) {
			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
					        static_cast<const Size*>(request.data());
					VertexTool tool;
					tool.Setup(*size_p, DefaultBorderWidth(),
					        round_corner_type(), round_corner_radius());
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_outer->Bind();
					tool.SetOuterBufferData(m_outer.get());

					set_size(*size_p);
					Refresh();
					break;
				}

				case WidgetRoundCornerType: {
					const int* type_p = static_cast<const int*>(request.data());
					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(), *type_p,
					        round_corner_radius());
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_outer->Bind();
					tool.SetOuterBufferData(m_outer.get());

					set_round_corner_type(*type_p);
					Refresh();
					break;
				}

				case WidgetRoundCornerRadius: {
					const float* radius_p =
					        static_cast<const float*>(request.data());
					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(),
					        round_corner_type(), *radius_p);
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_outer->Bind();
					tool.SetOuterBufferData(m_outer.get());

					set_round_corner_radius(*radius_p);
					Refresh();
					break;
				}

				default:
					break;
			}
		}

		ReportGeometryUpdate(request);
	}

	void ProgressBar::BroadcastUpdate (const GeometryUpdateRequest& request)
	{
	}

	ResponseType ProgressBar::Draw(const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(), 0.f);
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program =
						Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 0);

		//program->SetVertexAttrib4fv("Color",
		//		Theme::instance->regular().inner.data());

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

		program->SetUniform1i("AA", 1);
		program->SetVertexAttrib4fv("Color", Theme::instance->regular().outline.data());

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		glBindVertexArray(0);
		program->Reset();

		return Accept;
	}

	ResponseType ProgressBar::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ProgressBar::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ProgressBar::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void ProgressBar::InitializeProgressBar ()
	{
		float minxi = DefaultBorderWidth();
		float maxxi = size().width() - DefaultBorderWidth();
		float minyi = DefaultBorderWidth();
		float maxyi = size().height() - DefaultBorderWidth();

		float midx = (minxi + maxxi) / 2.f;

		GLfloat verts[] = {

				minxi, minyi,	0.25f, 0.25f, 0.25f, 0.75f,
				minxi, maxyi,	0.25f, 0.25f, 0.25f, 0.75f,

				midx, minyi,	0.25f, 0.25f, 0.25f, 0.25f,
				midx, maxyi, 	0.25f, 0.25f, 0.25f, 0.25f,

				maxxi, minyi,	0.25f, 0.25f, 0.25f, 0.025f,
				maxxi, maxyi,	0.25f, 0.25f, 0.25f, 0.025f

		};

		VertexTool tool;
		tool.Setup (size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());

		glGenVertexArrays(2, m_vao);
		glBindVertexArray(m_vao[0]);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();

		m_inner->SetData(36 * sizeof(GLfloat), verts);
		//tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[1]);
		m_outer.reset(new GLArrayBuffer);
		m_outer->Generate();
		m_outer->Bind();
		tool.SetOuterBufferData(m_outer.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

	}

}