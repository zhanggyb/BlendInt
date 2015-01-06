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

#include <BlendInt/Gui/ToolButton.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Core/Image.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ToolButton::ToolButton ()
	: AbstractButton()
	{
		set_size(24, 24);
		set_round_type(RoundAll);

		InitializeToolButton();
	}

	ToolButton::~ToolButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void ToolButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(*request.size(), round_type(),
					round_radius(), text());

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

            buffer_.bind(0);
            buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
            buffer_.bind(1);
            buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
            GLArrayBuffer::reset();

            RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ToolButton::PerformRoundTypeUpdate(int round_type)
	{
		UpdateTextPosition(size(), round_type, round_radius(),
				text());

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

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}

	void ToolButton::PerformRoundRadiusUpdate(float radius)
	{
		UpdateTextPosition(size(), round_type(), radius,
				text());

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

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
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
		Shaders::instance->widget_inner_program()->use();

		if (is_down()) {
			glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);
			glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
			        Context::theme->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 15);
			} else {
				glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);
			}

            glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
					Context::theme->regular().inner.data());
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

        Shaders::instance->widget_outer_program()->use();

        glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION), 0.f, 0.f);
        glUniform4fv(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1,
                Context::theme->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
            glUniform4f(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1.0f,
                    1.0f, 1.0f, 0.16f);
            glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
                    0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		if(icon_) {
			float x = (size().width() - icon_->size().width()) / 2;
			float y = (size().height() - icon_->size().height()) / 2;

			if(hover()) {
				icon_->Draw(x, y, 15);
			} else {
				icon_->Draw(x, y, 0);
			}
		}
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

		icon_ = icon;	// for temporary use
	}

	void ToolButton::SetAction (const RefPtr<AbstractIcon>& icon, const String& text,
	        const String& shortcut)
	{
		RefPtr<Action> action(new Action(icon, text, shortcut));

		action_ = action;
	}

	void ToolButton::SetAction (const RefPtr<Action>& item)
	{
		action_ = item;
	}

	Size ToolButton::GetPreferredSize() const
	{
		return Size(24, 24);
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
        buffer_.generate ();

        glBindVertexArray(vao_[0]);

        buffer_.bind(0);
        buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
        glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
        glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3,
                GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(vao_[1]);
        buffer_.bind(1);
        buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
        glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_OUTER_COORD));
        glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_OUTER_COORD), 2,
                GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(0);
        buffer_.reset();

		// demo
		icon_ = Context::icons->icon_16x16(Icons::IMAGE_ALPHA);
	}

}
