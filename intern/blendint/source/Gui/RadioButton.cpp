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

#include <BlendInt/Gui/RadioButton.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	RadioButton::RadioButton ()
	: AbstractButton()
	{
		set_round_type(RoundAll);
		set_checkable(true);
		Font font;	// default font
		set_size(font.height() + kPadding.hsum(),
		        font.height() + kPadding.vsum());

		InitializeRadioButtonOnce();
	}

	RadioButton::RadioButton (const String& text)
	: AbstractButton()
	{
		set_round_type(RoundAll);
		set_checkable(true);

		RefPtr<Text> t(new Text(text));
		set_text(t);

		InitializeRadioButtonOnce(text);
	}

	RadioButton::RadioButton (const RefPtr<AbstractIcon>& icon)
	: AbstractButton()
	{
		set_round_type(RoundAll);
		set_checkable(true);
		icon_ = icon;

		InitializeRadioButtonOnce(icon, String());
	}

	RadioButton::RadioButton (const RefPtr<AbstractIcon>& icon,
	        const String& text)
	: AbstractButton()
	{
		set_round_type(RoundAll);
		set_checkable(true);

		RefPtr<Text> t(new Text(text));
		set_text(t);

		icon_ = icon;

		InitializeRadioButtonOnce(icon, text);
	}

	RadioButton::~RadioButton ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void RadioButton::SetIcon (const RefPtr<AbstractIcon>& icon)
	{
		icon_ = icon;

		// TODO: reset icon_offset

		RequestRedraw();
	}

	Size RadioButton::GetPreferredSize () const
	{
		Size prefer;

		Font font;	// default font
		if(text()) {
			font = text()->font();
		}

		int font_height = font.height();
		int h = font_height;

		if(icon_) {
			h = std::max(icon_->size().height(), font_height);
		}

		prefer.set_height(h + kPadding.vsum());
		// top padding: 2, bottom padding: 2

		int w = 0;
		if (text()) {

			if(icon_) {
				w = icon_->size().width();
			} else {
				w = font_height;
			}

			w = w + kPadding.hsum();

		} else {

			if(icon_) {
				w = w + icon_->size().width() + kIconTextSpace;
			}

			int text_width; // = font().GetTextWidth(text());
			w = w + text_width + kPadding.hsum(); // left padding: 2, right padding: 2

		}

		prefer.set_width(w);

		return prefer;
	}

	bool RadioButton::IsExpandX () const
	{
		return true;
	}

	void RadioButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Context::theme->radio_button().shaded) {
				GenerateRoundedVertices(Vertical,
						Context::theme->radio_button().shadetop,
						Context::theme->radio_button().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			inner_->bind();
			inner_->set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			outer_->bind();
			outer_->set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			GLArrayBuffer::reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void RadioButton::PerformRoundTypeUpdate (int round_type)
	{
			set_round_type(round_type);

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Context::theme->radio_button().shaded) {
				GenerateRoundedVertices(Vertical,
						Context::theme->radio_button().shadetop,
						Context::theme->radio_button().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			inner_->bind();
			inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			outer_->bind();
			outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			GLArrayBuffer::reset();

			RequestRedraw();
	}

	void RadioButton::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->radio_button().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->radio_button().shadetop,
					Context::theme->radio_button().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		outer_->bind();
		outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}

	ResponseType RadioButton::Draw (const Context* context)
	{
		Context::shaders->widget_inner_program()->use();

		if (hover()) {

			glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 15);
			if (is_checked()) {
				glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        Context::theme->radio_button().inner_sel.data());
			} else {
				glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        Context::theme->radio_button().inner.data());
			}

		} else {
			glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
			if (is_checked()) {
				glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        Context::theme->radio_button().inner_sel.data());
			} else {
				glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        Context::theme->radio_button().inner.data());
			}
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		Context::shaders->widget_outer_program()->use();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_OUTER_POSITION), 0.f, 0.f);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1,
		        Context::theme->radio_button().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(Context::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(Context::shaders->location(Shaders::WIDGET_OUTER_POSITION),
			        0.f, 0.f - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		if(icon_) {

		}

		if (text()) {
			//font().Print(0.f, 0.f, text(), text_length(), 0);
		}

		return Finish;
	}

	void RadioButton::InitializeRadioButtonOnce ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Context::theme->radio_button().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->radio_button().shadetop,
					Context::theme->radio_button().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_OUTER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_OUTER_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void RadioButton::InitializeRadioButtonOnce (const String& text)
	{
		/*
		int left = kPadding.left() * Context::theme->pixel();
		int right = kPadding.right() * Context::theme->pixel();
		int top = kPadding.top() * Context::theme->pixel();
		int bottom = kPadding.bottom() * Context::theme->pixel();
		int h = font().height();

		if(text.empty()) {
			set_size(h + round_radius() * 2 * Context::theme->pixel() + left + right,
							h + top + bottom);
		} else {
			set_text_length(text.length());
			Rect text_outline; // = font().GetTextOutline(text);

			int width = text_outline.width()
							+ round_radius() * 2 * Context::theme->pixel()
							+ left + right;
			int height = h + top + bottom;

			set_size(width, height);

//			set_pen((width - text_outline.width()) / 2,
//							(height - font().GetHeight()) / 2
//											+ std::abs(font().GetDescender()));
		}

		InitializeRadioButtonOnce();
		*/
	}

	void RadioButton::InitializeRadioButtonOnce (const RefPtr<AbstractIcon>& icon,
	        const String& text)
	{
		/*
		int left = kPadding.left() * Context::theme->pixel();
		int right = kPadding.right() * Context::theme->pixel();
		int top = kPadding.top() * Context::theme->pixel();
		int bottom = kPadding.bottom() * Context::theme->pixel();
		int font_height = font().height();
		int h = 0;

		if(text.empty()) {

			if(icon) {
				h = std::max(icon->size().height(), font_height);
				set_size(icon->size().width() + round_radius() * 2 * Context::theme->pixel() + left + right,
						h + top + bottom);
			} else {
				set_size(font_height + round_radius() * 2 * Context::theme->pixel() + left + right,
						font_height + top + bottom);
			}

		} else {

			set_text_length(text.length());
			Rect text_outline; // = font().GetTextOutline(text);

			if(icon) {
				h = std::max(icon->size().height(), font_height);

				int width = icon->size().width() + text_outline.width()
								+ round_radius() * 2 * Context::theme->pixel()
								+ left + right;
				int height = h + top + bottom;

				set_size(width, height);

//				set_pen(((width - icon_->size().width()) - text_outline.width()) / 2 + icon_->size().width(),
//								(height - font().GetHeight()) / 2
//												+ std::abs(font().GetDescender()));

			} else {

				int width = text_outline.width()
								+ round_radius() * 2 * Context::theme->pixel()
								+ left + right;
				int height = font_height + top + bottom;

				set_size(width, height);

//				set_pen((width - text_outline.width()) / 2,
//								(height - font().GetHeight()) / 2
//												+ std::abs(font().GetDescender()));
			}

		}

		if(icon) {
			icon_offset_x_ = round_radius() + left + icon_->size().width() / 2.f;
			icon_offset_y_ = size().height() / 2.f;
		}

		InitializeRadioButtonOnce();
		*/
	}

}
