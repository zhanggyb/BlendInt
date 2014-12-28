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

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Button::Button ()
	: AbstractButton(),
	  icon_offset_x_(0.f),
	  icon_offset_y_(0.f),
	  show_icon_(true)
	{
		set_round_type(RoundAll);
		int h = font().GetHeight();
		set_size(h + kDefaultPadding.hsum(),
		        h + kDefaultPadding.vsum());

		InitializeButtonOnce();
	}

	Button::Button (const String& text)
	: AbstractButton(),
	  icon_offset_x_(0.f),
	  icon_offset_y_(0.f),
	  show_icon_(true)
	{
		set_round_type(RoundAll);
		set_text(text);

		InitializeButtonOnce(text);
	}

	Button::Button (const RefPtr<AbstractIcon>& icon)
	: AbstractButton(),
	  icon_offset_x_(0.f),
	  icon_offset_y_(0.f),
	  show_icon_(true)
	{
		set_round_type(RoundAll);
		icon_ = icon;

		InitializeButtonOnce(icon, String());
	}

	Button::Button (const RefPtr<AbstractIcon>& icon, const String& text)
	: AbstractButton(),
	  icon_offset_x_(0.f),
	  icon_offset_y_(0.f),
	  show_icon_(true)
	{
		set_round_type(RoundAll);
		set_text(text);
		icon_ = icon;

		InitializeButtonOnce(icon, text);
	}

	Button::~Button ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void Button::SetIcon (const RefPtr<AbstractIcon>& icon)
	{
		icon_ = icon;

		icon_offset_x_ = kDefaultPadding.left();
		icon_offset_y_ = (size().height() - icon_->size().height()) / 2.f;

		RequestRedraw();
	}

	bool Button::IsExpandX() const
	{
		return false;
	}

	Size Button::GetPreferredSize() const
	{
		Size prefer;

		int font_height = font().GetHeight();
		int h = font_height;

		if(icon_) {
			h = std::max(icon_->size().height(), font_height);
		}

		prefer.set_height(h + kDefaultPadding.vsum());
		// top padding: 2, bottom padding: 2

		int w = 0;
		if (text().empty()) {

			if(icon_) {
				w = icon_->size().width();
			} else {
				w = font_height;
			}

			w = w + kDefaultPadding.hsum();

		} else {

			if(icon_) {
				w = w + icon_->size().width() + icon_text_space;
			}

			int text_width = font().GetTextWidth(text());
			w = w + text_width + kDefaultPadding.hsum(); // left padding: 2, right padding: 2

		}

		if((!icon_) && (w < 80)) {
			w = 80;
		}

		prefer.set_width(w);

		return prefer;
	}

	void Button::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Theme::instance->regular().shaded) {
				GenerateRoundedVertices(Vertical,
						Theme::instance->regular().shadetop,
						Theme::instance->regular().shadedown,
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

			CalculateIconTextPosition(size(), round_type(), round_radius());

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

		if (Theme::instance->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->regular().shadetop,
					Theme::instance->regular().shadedown,
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

		CalculateIconTextPosition(size(), round_type(), round_radius());

		RequestRedraw();
	}

	void Button::PerformRoundRadiusUpdate(float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->regular().shadetop,
					Theme::instance->regular().shadedown,
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

		CalculateIconTextPosition(size(), round_type(), round_radius());

		RequestRedraw();
	}

	ResponseType Button::Draw (Profile& profile)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);

		if (is_down()) {
			glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
			        Theme::instance->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 15);
			}

			glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
					Theme::instance->regular().inner.data());
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		Shaders::instance->widget_outer_program()->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION), 0.f, 0.f);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1,
		        Theme::instance->regular().outline.data());

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

		if(show_icon_ && icon_) {
			if(hover()) {
				icon_->Draw(0.f + icon_offset_x_, 0.f + icon_offset_y_, 15);
			} else {
				icon_->Draw(0.f + icon_offset_x_, 0.f + icon_offset_y_, 0);
			}
		}

		if (text().size()) {
			font().Print(0.f, 0.f, text(), text_length(), 0);
		}

		return Finish;
	}

	void Button::CalculateIconTextPosition(const Size& size, int round_type, float radius)
	{
		int x = kDefaultPadding.left() * Theme::instance->pixel();
		int y = kDefaultPadding.bottom() * Theme::instance->pixel();

		icon_offset_x_ = 0.f;
		icon_offset_y_ = 0.f;

		int valid_width = size.width() - kDefaultPadding.hsum() * Theme::instance->pixel();
		int valid_height = size.height() - kDefaultPadding.vsum() * Theme::instance->pixel();

		if(valid_width <= 0 || valid_height <= 0) {
			show_icon_ = false;
			set_text_length(0);
			return;
		}

		icon_offset_x_ += kDefaultPadding.left() * Theme::instance->pixel();

		if(text().empty()) {

			set_text_length(0);

			if(icon_) {

				if((icon_->size().width() > valid_width) ||
						(icon_->size().height() > valid_height)) {
					show_icon_ = false;
				} else {
					show_icon_ = true;
					icon_offset_x_ = (size.width() - icon_->size().width()) / 2.f;
					icon_offset_y_ = (size.height() - icon_->size().height()) / 2.f;
				}

			} else {

				// Nothing to configure

			}

		} else {

			int text_length = 0;

			if(icon_) {

				icon_offset_y_ = (size.height() - icon_->size().height()) / 2.f;

				int text_width = valid_width - icon_->size().width() - icon_text_space;

				if(text_width < 0) {

					show_icon_ = false;

				} else if (text_width == 0) {

					if(icon_->size().height() > valid_height) {
						show_icon_ = false;
					} else {
						show_icon_ = true;
					}

				} else {

					if(icon_->size().height() > valid_height) {
						show_icon_ = false;
					} else {
						show_icon_ = true;
					}

					x = x + icon_->size().width() + icon_text_space;

					bool cal_width = true;

					Rect text_outline = font().GetTextOutline(text());

					if(valid_height < text_outline.height()) {
						text_length = 0;
						cal_width = false;
					}

					if(cal_width) {
						if(text_width < text_outline.width()) {
							text_length = GetValidTextLength(text(), font(), text_width);
						} else if (text_width == text_outline.width()) {
							text_length = text().length();
						} else {
							text_length = text().length();
							x = x + (valid_width - icon_->size().width() - text_outline.width()) / 2;
						}
						y = (size.height() - font().GetHeight()) / 2 + std::abs(font().GetDescender());
					}

					set_pen(x, y);

				}

			} else {

				// If size changed, we need to update the text length for printing too.
				bool cal_width = true;

				Rect text_outline = font().GetTextOutline(text());

				if(valid_height < text_outline.height()) {
					text_length = 0;
					cal_width = false;
				}

				if(cal_width) {
					if(valid_width < text_outline.width()) {
						text_length = GetValidTextLength(text(), font(), valid_width);
					} else {
						text_length = text().length();
						x = (size.width() - text_outline.width()) / 2;
					}
					y = (size.height() - font().GetHeight()) / 2 + std::abs(font().GetDescender());
				}

				set_pen(x, y);

			}

			set_text_length(text_length);
		}

	}

	void Button::InitializeButtonOnce ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->regular().shadetop,
					Theme::instance->regular().shadedown,
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
	}

	void Button::InitializeButtonOnce (const String& text)
	{
		int left = kDefaultPadding.left() * Theme::instance->pixel();
		int right = kDefaultPadding.right() * Theme::instance->pixel();
		int top = kDefaultPadding.top() * Theme::instance->pixel();
		int bottom = kDefaultPadding.bottom() * Theme::instance->pixel();
		int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_radius() * 2 * Theme::instance->pixel() + left + right,
							h + top + bottom);
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			int width = text_outline.width()
							+ round_radius() * 2 * Theme::instance->pixel()
							+ left + right;
			int height = h + top + bottom;

			set_size(width, height);

			set_pen((width - text_outline.width()) / 2,
							(height - font().GetHeight()) / 2
											+ std::abs(font().GetDescender()));
		}

		InitializeButtonOnce();
	}

	void Button::InitializeButtonOnce (const RefPtr<AbstractIcon>& icon, const String& text)
	{
		int left = kDefaultPadding.left() * Theme::instance->pixel();
		int right = kDefaultPadding.right() * Theme::instance->pixel();
		int top = kDefaultPadding.top() * Theme::instance->pixel();
		int bottom = kDefaultPadding.bottom() * Theme::instance->pixel();
		int font_height = font().GetHeight();
		int h = 0;

		if(text.empty()) {

			if(icon) {
				h = std::max(icon->size().height(), font_height);
				set_size(icon->size().width() + round_radius() * 2 * Theme::instance->pixel() + left + right,
						h + top + bottom);
			} else {
				set_size(font_height + round_radius() * 2 * Theme::instance->pixel() + left + right,
						font_height + top + bottom);
			}

		} else {

			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			if(icon) {
				h = std::max(icon->size().height(), font_height);

				int width = icon->size().width() + text_outline.width()
								+ round_radius() * 2 * Theme::instance->pixel()
								+ left + right;
				int height = h + top + bottom;

				set_size(width, height);

				set_pen(((width - icon_->size().width()) - text_outline.width()) / 2 + icon_->size().width(),
								(height - font().GetHeight()) / 2
												+ std::abs(font().GetDescender()));

			} else {

				int width = text_outline.width()
								+ round_radius() * 2 * Theme::instance->pixel()
								+ left + right;
				int height = font_height + top + bottom;

				set_size(width, height);

				set_pen((width - text_outline.width()) / 2,
								(height - font().GetHeight()) / 2
												+ std::abs(font().GetDescender()));
			}

		}

		if(icon) {
			icon_offset_x_ = round_radius() + left;
			icon_offset_y_ = (size().height() - icon->size().height()) / 2.f;
		}

		InitializeButtonOnce();
	}

}
