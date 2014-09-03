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

#include <BlendInt/Gui/VertexTool.hpp>
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
		set_drop_shadow(true);
		int h = font().GetHeight();
		set_size(h + default_padding.hsum(),
		        h + default_padding.vsum());

		InitializeButtonOnce();
	}

	Button::Button (const String& text)
	: AbstractButton(),
	  icon_offset_x_(0.f),
	  icon_offset_y_(0.f),
	  show_icon_(true)
	{
		set_round_type(RoundAll);
		set_drop_shadow(true);
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
		set_drop_shadow(true);
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
		set_drop_shadow(true);
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

		icon_offset_x_ = default_padding.left();
		icon_offset_y_ = (size().height() - icon_->size().height()) / 2.f;

		Refresh();
	}

	Size Button::GetPreferredSize() const
	{
		Size prefer;

		int font_height = font().GetHeight();
		int h = font_height;

		if(icon_) {
			h = std::max(icon_->size().height(), font_height);
		}

		prefer.set_height(h + default_padding.vsum());
		// top padding: 2, bottom padding: 2

		int w = 0;
		if (text().empty()) {

			if(icon_) {
				w = icon_->size().width();
			} else {
				w = font_height;
			}

			w = w + default_padding.hsum();

		} else {

			if(icon_) {
				w = w + icon_->size().width() + icon_text_space;
			}

			int text_width = font().GetTextWidth(text());
			w = w + text_width + default_padding.hsum(); // left padding: 2, right padding: 2

		}

		prefer.set_width(w);

		return prefer;
	}

	void Button::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			if (Theme::instance->regular().shaded) {
				tool.GenerateVertices(*request.size(), DefaultBorderWidth(),
						round_type(), round_radius(), Vertical,
						Theme::instance->regular().shadetop,
						Theme::instance->regular().shadedown);
			} else {
				tool.GenerateVerticesExt(*request.size(), DefaultBorderWidth(),
						round_type(), round_radius());
			}
			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_size(*request.size());

			CalculateIconTextPosition(size(), round_type(), round_radius());

			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Button::PerformRoundTypeUpdate(const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			if (Theme::instance->regular().shaded) {
				tool.GenerateVertices(size(), DefaultBorderWidth(),
						*request.round_type(), round_radius(), Vertical,
						Theme::instance->regular().shadetop,
						Theme::instance->regular().shadedown);
			} else {
				tool.GenerateVerticesExt(size(), DefaultBorderWidth(),
						*request.round_type(), round_radius());
			}
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_round_type(*request.round_type());

			CalculateIconTextPosition(size(), round_type(), round_radius());

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void Button::PerformRoundRadiusUpdate(const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			if (Theme::instance->regular().shaded) {
				tool.GenerateVertices(size(), DefaultBorderWidth(),
						round_type(), *request.round_radius(), Vertical,
						Theme::instance->regular().shadetop,
						Theme::instance->regular().shadedown);
			} else {
				tool.GenerateVerticesExt(size(), DefaultBorderWidth(),
						round_type(), *request.round_radius());
			}
			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_round_radius(*request.round_radius());

			CalculateIconTextPosition(size(), round_type(), round_radius());

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType Button::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->widget_program();
		program->Use();

		glm::vec3 pos((GLfloat)position().x(), (GLfloat)position().y(), 0.f);

		glUniform3fv(Shaders::instance->location(Stock::WIDGET_POSITION), 1, glm::value_ptr(pos));
		glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 0);

		if (is_down()) {
			glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
			        Theme::instance->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 15);
			}

			glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
					Theme::instance->regular().inner.data());
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 1);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
		        Theme::instance->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);

			glUniform3f(Shaders::instance->location(Stock::WIDGET_POSITION),
			        (float) position().x(), (float) position().y() - 1.f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->reset();

		if(show_icon_ && icon_) {
			if(hover()) {
				icon_->Draw(pos.x + icon_offset_x_, pos.y + icon_offset_y_, 15);
			} else {
				icon_->Draw(pos.x + icon_offset_x_, pos.y + icon_offset_y_, 0);
			}
		}

		if (text().size()) {
			font().Print(position(), text(), text_length(), 0);
		}

		return Accept;
	}

	void Button::CalculateIconTextPosition(const Size& size, int round_type, float radius)
	{
		int x = default_padding.left() * Theme::instance->pixel();
		int y = default_padding.bottom() * Theme::instance->pixel();

		icon_offset_x_ = 0.f;
		icon_offset_y_ = 0.f;

		int valid_width = size.width() - default_padding.hsum() * Theme::instance->pixel();
		int valid_height = size.height() - default_padding.vsum() * Theme::instance->pixel();

		if(valid_width <= 0 || valid_height <= 0) {
			show_icon_ = false;
			set_text_length(0);
			return;
		}

		icon_offset_x_ += default_padding.left() * Theme::instance->pixel();

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
		VertexTool tool;

		if(Theme::instance->regular().shaded) {
			tool.GenerateVertices(size(),
					DefaultBorderWidth(),
					round_type(),
					round_radius(),
					Vertical,
					Theme::instance->regular().shadetop,
					Theme::instance->regular().shadedown);
		} else {
			tool.GenerateVerticesExt(size(), DefaultBorderWidth(), round_type(), round_radius());
		}

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void Button::InitializeButtonOnce (const String& text)
	{
		int left = default_padding.left() * Theme::instance->pixel();
		int right = default_padding.right() * Theme::instance->pixel();
		int top = default_padding.top() * Theme::instance->pixel();
		int bottom = default_padding.bottom() * Theme::instance->pixel();
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
		int left = default_padding.left() * Theme::instance->pixel();
		int right = default_padding.right() * Theme::instance->pixel();
		int top = default_padding.top() * Theme::instance->pixel();
		int bottom = default_padding.bottom() * Theme::instance->pixel();
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

