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

#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Core/Image.hpp>

namespace BlendInt {

	ToolButton::ToolButton ()
	: AbstractButton()
	{
		set_round_type(RoundAll);
		set_size(24, 24);

		InitializeToolButton();
	}

	ToolButton::~ToolButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void ToolButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

            std::vector<GLfloat> inner_verts;
            std::vector<GLfloat> outer_verts;

            if (Context::theme->regular().shaded) {
            	GenerateRoundedVertices(Vertical,
                        Context::theme->regular().shadetop,
                        Context::theme->regular().shadedown,
                        &inner_verts,
                        &outer_verts);
            } else {
            	GenerateRoundedVertices(&inner_verts, &outer_verts);
            }

            vbo_.bind(0);
            vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
            vbo_.bind(1);
            vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
            GLArrayBuffer::reset();

            RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ToolButton::PerformRoundTypeUpdate(int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->regular().shadetop,
					Context::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}

	void ToolButton::PerformRoundRadiusUpdate(float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->regular().shadetop,
					Context::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}

	void ToolButton::PerformHoverIn(const Context* context)
	{
		RequestRedraw();
	}

	void ToolButton::PerformHoverOut(const Context* context)
	{
		RequestRedraw();
	}

	ResponseType ToolButton::Draw (const Context* context)
	{
		Context::shaders->widget_inner_program()->use();

		if (is_down()) {
			glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
			glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
			        Context::theme->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 15);
			} else {
				glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
			}

            glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
					Context::theme->regular().inner.data());
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

        Context::shaders->widget_outer_program()->use();

        glUniform2f(Context::shaders->location(Shaders::WIDGET_OUTER_POSITION), 0.f, 0.f);
        glUniform4fv(Context::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1,
                Context::theme->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
            glUniform4f(Context::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
                    1.0f, 1.0f, 0.16f);
            glUniform2f(Context::shaders->location(Shaders::WIDGET_OUTER_POSITION),
                    0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		DrawIconText();

		return Finish;
	}

	void ToolButton::SetAction (const String& text)
	{
		RefPtr<Action> action(new Action(text));

		action_ = action;
	}

	void ToolButton::SetAction (const String& text, const String& shortcut)
	{
		RefPtr<Action> action(new Action(text, shortcut));

		action_ = action;
	}

	void ToolButton::SetAction (const RefPtr<AbstractIcon>& icon, const String& text)
	{
		RefPtr<Action> action(new Action(icon, text));

		action_ = action;

		SetIcon(icon);
		SetText(text);
	}

	void ToolButton::SetAction (const RefPtr<AbstractIcon>& icon, const String& text,
	        const String& shortcut)
	{
		RefPtr<Action> action(new Action(icon, text, shortcut));

		action_ = action;

		SetIcon(icon);
		SetText(text);
	}

	void ToolButton::SetAction (const RefPtr<Action>& item)
	{
		action_ = item;
	}

	Size ToolButton::GetPreferredSize() const
	{
		if(!text()) {
			return Size(24, 24);
		} else {
			return Size(24, 24);
		}
	}

	void ToolButton::InitializeToolButton ()
	{
        std::vector<GLfloat> inner_verts;
        std::vector<GLfloat> outer_verts;

        if (Context::theme->regular().shaded) {
        	GenerateRoundedVertices(Vertical,
                    Context::theme->regular().shadetop,
                    Context::theme->regular().shadedown,
                    &inner_verts,
                    &outer_verts);
        } else {
        	GenerateRoundedVertices(&inner_verts, &outer_verts);
        }

        glGenVertexArrays(2, vao_);
        vbo_.generate ();

        glBindVertexArray(vao_[0]);

        vbo_.bind(0);
        vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
        glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_INNER_COORD));
        glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_INNER_COORD), 3,
                GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(vao_[1]);
        vbo_.bind(1);
        vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
        glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_OUTER_COORD));
        glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_OUTER_COORD), 2,
                GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(0);
        vbo_.reset();
	}

}
