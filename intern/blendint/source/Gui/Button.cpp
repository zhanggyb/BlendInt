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

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Button::Button ()
	: AbstractButton()
	{
		set_round_type(RoundAll);

		Font font;	// default font
		int w = 80;
		int h = font.height();

		set_size(w + pixel_size(kPadding.hsum()),
		        h + pixel_size(kPadding.vsum()));

		InitializeButtonOnce();
	}

	Button::Button (const String& text)
	: AbstractButton()
	{
		set_round_type(RoundAll);

		RefPtr<Text> t(new Text(text));
		set_text(t);

		int w = t->size().width();
		int h = t->font().height();
		if(w < 80) w = 80;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);

		InitializeButtonOnce();
	}

	Button::Button (const RefPtr<AbstractIcon>& icon)
	: AbstractButton()
	{
		set_round_type(RoundAll);
		icon_ = icon;

		int w = icon_->size().width();
		int h = icon_->size().height();

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);

		InitializeButtonOnce();
	}

	Button::Button (const RefPtr<AbstractIcon>& icon, const String& text)
	: AbstractButton()
	{
		set_round_type(RoundAll);

		RefPtr<Text> t(new Text(text));
		set_text(t);

		icon_ = icon;

		int w = icon_->size().width();
		int h = icon_->size().height();

		w += kIconTextSpace;

		w += t->size().width();
		h = std::max(h, t->font().height());

		if(w < 80) w = 80;
		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);

		InitializeButtonOnce();
	}

	Button::~Button ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void Button::SetIcon (const RefPtr<AbstractIcon>& icon)
	{
		icon_ = icon;

		RequestRedraw();
	}

	bool Button::IsExpandX() const
	{
		return false;
	}

	Size Button::GetPreferredSize() const
	{
		Size prefer;

		int w = 0;
		int h = 0;

		if(icon_) {
			w = icon_->size().width();
			h = icon_->size().height();
		}

		w += kIconTextSpace;

		Font font;	// default font
		if(text()) {
			font = text()->font();
			w += text()->size().width();
		}

		h = std::max(h, font.height());

		if(w < 80) w = 80;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		return prefer;
	}

	void Button::PerformSizeUpdate (const SizeUpdateRequest& request)
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

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			buffer_.reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Button::PerformRoundTypeUpdate(int type)
	{
		set_round_type(type);

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
		buffer_.reset();

		RequestRedraw();
	}

	void Button::PerformRoundRadiusUpdate(float radius)
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

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		RequestRedraw();
	}

	ResponseType Button::Draw (const Context* context)
	{
		Context::shaders->widget_inner_program()->use();

		glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		if (is_down()) {
			glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
			        Context::theme->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 15);
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

		int w = size().width() - pixel_size(kPadding.hsum());
		int h = size().height() - pixel_size(kPadding.vsum());
		float x = pixel_size(kPadding.left());
		float y = size().height() / 2.f;

		if(icon_) {
			if(icon_->size().height() <= h) {

				if(icon_->size().width() <= w) {

					icon_->Draw(x + icon_->size().width() / 2.f, y);
					x += icon_->size().width();
					x += kIconTextSpace;
					w -= icon_->size().width();
					w -= kIconTextSpace;
				}

			}
		}

		if (text()) {

			if(text()->size().height() <= h) {

				y = y - text()->size().height() / 2.f;
				if(text()->size().width() < w) {
					x += (w - text()->size().width()) / 2.f;
					text()->Draw(x, y);
				} else {
					text()->Draw(x, y, (float)w);
				}

			}

		}

		return Finish;
	}

	void Button::InitializeButtonOnce ()
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
		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_OUTER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_OUTER_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

}
