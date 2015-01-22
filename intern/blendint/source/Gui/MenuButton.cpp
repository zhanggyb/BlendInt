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

#include <BlendInt/Gui/MenuButton.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	MenuButton::MenuButton (const String& text)
	: AbstractButton(text),
	  vao_(0)
	{
		set_round_type(RoundAll);

		int w = this->text()->size().width();
		int h = this->text()->font().height();
		if(w < 80) w = 80;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);

		InitializeMenuButton();
	}
	
	MenuButton::~MenuButton ()
	{
		glDeleteVertexArrays(1, &vao_);
	}
	
	Size MenuButton::GetPreferredSize() const
	{
		Size s = AbstractButton::GetPreferredSize();

		if(text()) {
			if(s.width() < 80) {
				s.set_width(80);
			}
		}

		return s;
	}

	void MenuButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

            set_size(*request.size());

            std::vector<GLfloat> inner_verts;
            AbstractView::GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts, 0);

			vbo_.bind();
			vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
            vbo_.reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void MenuButton::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, round_type, round_radius(), &inner_verts, 0);

		vbo_.bind();
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	void MenuButton::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts, 0);

		vbo_.bind();
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	ResponseType MenuButton::Draw (const Context* context)
	{
		if (hover()) {

			Context::shaders->widget_inner_program()->use();

			glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
			glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1, Context::theme->menu_item().inner_sel.data());

			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
			glBindVertexArray(0);

            GLSLProgram::reset();

		}

		DrawIconText();

		return Finish;
	}

	void MenuButton::SetMenu (const RefPtr<Menu>& menu)
	{
		m_menu = menu;
	}

	void MenuButton::InitializeMenuButton ()
	{
		glGenVertexArrays(1, &vao_);

        std::vector<GLfloat> inner_verts;
        GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts, 0);

		glBindVertexArray(vao_);
		vbo_.generate();
		vbo_.bind();
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		vbo_.reset();
	}

} /* namespace BlendInt */
