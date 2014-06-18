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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/VertexTool.hpp>

namespace BlendInt {

	static const unsigned char pixel_data[24 * 24 * 4 + 1] = {
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0g\313\25Cg\313\25Cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25Cg\313\25cg"
	  "\313\25cg\313\25cg\313\25cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0g\313\25Cg\313\25Cg\313\25cg\313\25\213g\313\25\246g\313\25\246"
	  "g\313\25\246g\313\25\246g\313\25\213g\313\25cg\313\25Cg\313\25C\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0g\313\25Cg\313\25cg\313\25\213g\313\25\246g\313\25\272g\313\25\311"
	  "g\313\25\325g\313\25\325g\313\25\311g\313\25\272g\313\25\246g\313\25\213"
	  "g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25\213g\313\25\246g\313\25\311g"
	  "\313\25\337g\313\25\355g\313\25\363g\313\25\363g\313\25\355g\313\25\337g"
	  "\313\25\311g\313\25\246g\313\25\213g\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\246"
	  "g\313\25\311g\313\25\347g\313\25\365g\313\25\366g\313\25\367g\313\25\367"
	  "g\313\25\366g\313\25\365g\313\25\347g\313\25\311g\313\25\246g\313\25cg\313"
	  "\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313"
	  "\25Cg\313\25\213g\313\25\272g\313\25\337g\313\25\365g\313\25\367g\313\25"
	  "\370g\313\25\371g\313\25\371g\313\25\370g\313\25\367g\313\25\365g\313\25"
	  "\337g\313\25\272g\313\25\213g\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\246g\313\25\311g\313\25\355"
	  "g\313\25\366g\313\25\370g\313\25\371g\313\25\372g\313\25\372g\313\25\371"
	  "g\313\25\370g\313\25\366g\313\25\355g\313\25\311g\313\25\246g\313\25cg\313"
	  "\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313"
	  "\25\246g\313\25\325g\313\25\363g\313\25\367g\313\25\371g\313\25\372g\313"
	  "\25\372g\313\25\372g\313\25\372g\313\25\371g\313\25\367g\313\25\363g\313"
	  "\25\325g\313\25\246g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\246g\313\25\325g\313\25\363g\313\25"
	  "\367g\313\25\371g\313\25\372g\313\25\372g\313\25\372g\313\25\372g\313\25"
	  "\371g\313\25\367g\313\25\363g\313\25\325g\313\25\246g\313\25cg\313\25C\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25"
	  "\246g\313\25\311g\313\25\355g\313\25\366g\313\25\370g\313\25\371g\313\25"
	  "\372g\313\25\372g\313\25\371g\313\25\370g\313\25\366g\313\25\355g\313\25"
	  "\311g\313\25\246g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25\213g\313\25\272g\313\25\337g\313\25\365"
	  "g\313\25\367g\313\25\370g\313\25\371g\313\25\371g\313\25\370g\313\25\367"
	  "g\313\25\365g\313\25\337g\313\25\272g\313\25\213g\313\25C\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25"
	  "\246g\313\25\311g\313\25\347g\313\25\365g\313\25\366g\313\25\367g\313\25"
	  "\367g\313\25\366g\313\25\365g\313\25\347g\313\25\311g\313\25\246g\313\25"
	  "cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0g\313\25Cg\313\25\213g\313\25\246g\313\25\311g\313\25\337g\313"
	  "\25\355g\313\25\363g\313\25\363g\313\25\355g\313\25\337g\313\25\311g\313"
	  "\25\246g\313\25\213g\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\213g\313\25"
	  "\246g\313\25\272g\313\25\311g\313\25\325g\313\25\325g\313\25\311g\313\25"
	  "\272g\313\25\246g\313\25\213g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg"
	  "\313\25Cg\313\25cg\313\25\213g\313\25\246g\313\25\246g\313\25\246g\313\25"
	  "\246g\313\25\213g\313\25cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0g\313\25Cg\313\25Cg\313\25cg\313\25cg\313\25cg\313\25cg\313\25"
	  "Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0g\313\25Cg\313\25Cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
	};


	ToolButton::ToolButton ()
	: AbstractButton()
	{
		set_size(24, 24);
		set_round_corner_type(RoundAll);

		InitializeToolButton();
	}

	ToolButton::~ToolButton ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void ToolButton::UpdateGeometry (const WidgetUpdateRequest& request)
	{

		switch (request.type()) {

			case WidgetSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				VertexTool tool;
				tool.Setup(*size_p, DefaultBorderWidth(), RoundAll, 5);
				m_inner->Bind();
				tool.SetInnerBufferData(m_inner.get());
				m_outer->Bind();
				tool.SetOuterBufferData(m_outer.get());

				break;
			}

			default:
				break;

		}

	}

	ResponseType ToolButton::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);

		if(down()) {
			program->SetVertexAttrib4f("Color", 0.2f, 0.2f, 0.2f, 1.f);
			program->SetUniform1i("Gamma", 0);
		} else {
			program->SetVertexAttrib4f("Color", 0.4f, 0.4f, 0.3f, 1.f);

			if(hover()) {
				program->SetUniform1i("Gamma", 20);
			} else {
				program->SetUniform1i("Gamma", 0);
			}
		}

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_corner_type()) + 2);

		program->SetUniform1i("AA", 1);
		program->SetUniform1i("Gamma", 0);
		program->SetVertexAttrib4f("Color", 0.f, 0.f, 0.f, 1.f);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		if (emboss()) {
			program->SetVertexAttrib4f("Color", 1.0f, 1.0f, 1.0f, 0.16f);

			glm::mat4 emboss_mvp = glm::translate(mvp,
							glm::vec3(0.f, -1.f, 0.f));
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
							glm::value_ptr(emboss_mvp));
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_corner_type()) * 2);
		}

		glBindVertexArray(0);
		program->Reset();

		if(hover()) {
			m_icon->Draw(mvp, 20);
		} else {
			m_icon->Draw(mvp, 0);
		}

		return Accept;
	}

	void ToolButton::SetAction (const String& text)
	{
		RefPtr<Action> action(new Action(text));

		m_action = action;
	}

	void ToolButton::SetAction (const String& text, const String& shortcut)
	{
		RefPtr<Action> action(new Action(text, shortcut));

		m_action = action;
	}

	void ToolButton::SetAction (const RefPtr<Icon>& icon, const String& text)
	{
		RefPtr<Action> action(new Action(icon, text));

		m_action = action;
	}

	void ToolButton::SetAction (const RefPtr<Icon>& icon, const String& text,
	        const String& shortcut)
	{
		RefPtr<Action> action(new Action(icon, text, shortcut));

		m_action = action;
	}

	void ToolButton::SetAction (const RefPtr<Action>& item)
	{
		m_action = item;
	}

	Size ToolButton::GetPreferredSize() const
	{
		return Size(24, 24);
	}

	void ToolButton::InitializeToolButton ()
	{
		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), RoundAll, 5.f);

		glGenVertexArrays(2, m_vao);

		glBindVertexArray(m_vao[0]);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[1]);
		m_outer.reset(new GLArrayBuffer);
		m_outer->Generate();
		m_outer->Bind();
		tool.SetOuterBufferData(m_outer.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		// demo
		m_icon.reset(new PixelIcon(24, 24, pixel_data));
	}

}
