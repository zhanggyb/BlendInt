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

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

#include <BlendInt/Gui/AbstractFrame.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Margin TextEntry::default_padding = Margin(2, 2, 2, 2);

	TextEntry::TextEntry ()
	: Widget(),
	  start_(0),
	  length_(0),
	  index_(0)
	{
		int h = font_.GetHeight();
		int initial_width = h + round_radius() * 2 * Theme::instance->pixel()
				+ 120;
		int initial_height = h + vertical_space * 2 * Theme::instance->pixel();

		set_size(initial_width, initial_height);

		InitializeTextEntry();
	}

	TextEntry::~TextEntry ()
	{
		glDeleteVertexArrays(3, vaos_);
	}

	void TextEntry::SetText (const String& text)
	{
		bool cal_width = true;

		text_ = text;

		Rect text_outline = font_.GetTextOutline(text_);

		length_ = text_.length();

		if(size().height() < text_outline.height()) {
			length_ = 0;
			cal_width = false;
		}

		if(cal_width) {
			if(size().width() < text_outline.width()) {
				length_ = GetValidTextSize();
			}
		}

		font_.set_pen(round_radius(),
				(size().height() - font_.GetHeight()) / 2 + std::abs(font_.GetDescender()));

		index_ = text_.length();

		RequestRedraw();
	}

	void TextEntry::SetFont (const Font& font)
	{
		font_ = font;

		//Rect text_outline = m_font.GetTextOutline(m_text);

		//m_length = GetVisibleTextLengthInCursorMove(m_text, m_start);

		font_.set_pen(round_radius(),
				(size().height() - font_.GetHeight()) / 2 + std::abs(font_.GetDescender()));
	}

	void TextEntry::Clear ()
	{
		text_.clear();
		index_ = 0;
		start_ = 0;
		length_ = 0;
	}

	Size TextEntry::GetPreferredSize () const
	{
		Size preferred_size;

		float radius_plus = 0.f;

		if ((round_type() & RoundTopLeft) || (round_type() & RoundBottomLeft)) {
			radius_plus += round_radius();
		}

		if ((round_type() & RoundTopRight) || (round_type() & RoundBottomRight)) {
			radius_plus += round_radius();
		}

		int max_font_height = font_.GetHeight();

		preferred_size.set_height(
				max_font_height
						+ default_padding.vsum() * Theme::instance->pixel());// top padding: 2, bottom padding: 2

		if (text().empty()) {
			preferred_size.set_width(max_font_height + (int)radius_plus + 120);
		} else {
			int width = font_.GetTextWidth(text());
			preferred_size.set_width(width + (int)radius_plus);
		}

		return preferred_size;
	}

	bool TextEntry::IsExpandX () const
	{
		return true;
	}

	ResponseType TextEntry::KeyPressEvent (const Context* context)
	{
		if(!context->text().empty()) {

			text_.insert(index_, context->text());
			index_ += context->text().length();
			length_ += context->text().length();

			int text_width = font_.GetTextWidth(text_, length_,
							start_);
			int valid_width = size().width() - round_radius() * 2;

			if(text_width > valid_width) {
				start_++;
				length_--;

				text_width = font_.GetTextWidth(text_, length_, start_);
				while (text_width > valid_width) {
					start_++;
					length_--;
					text_width = font_.GetTextWidth(text_, length_, start_);
				}
			}

			RequestRedraw();
			return Finish;

		} else {

			switch (context->key()) {

				case Key_Backspace: {
					DisposeBackspacePress();
					break;
				}

				case Key_Delete: {
					DisposeDeletePress();
					break;
				}

				case Key_Left: {
					DisposeLeftPress();
					break;
				}

				case Key_Right: {
					DisposeRightPress();
					break;
				}

				case Key_Up: {
					break;
				}

				case Key_Down: {
					break;
				}

				default:
					break;
			}

			return Finish;
		}
	}

	ResponseType TextEntry::MousePressEvent(const Context* context)
	{
		if(text_.size()) {
			index_ = GetCursorPosition(context);

			DBG_PRINT_MSG("index: %d", index_);

			RequestRedraw();
		}

		return Finish;
	}

	void TextEntry::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Theme::instance->text().shaded) {
				GenerateRoundedVertices(Vertical,
						Theme::instance->text().shadetop,
						Theme::instance->text().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			inner_->bind();
			inner_->set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			outer_->bind();
			outer_->set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			cursor_buffer_->bind();
			GLfloat* buf_p = (GLfloat*) cursor_buffer_->map(GL_READ_WRITE);
			*(buf_p + 5) = (GLfloat) (request.size()->height()
					- vertical_space * 2 * Theme::instance->pixel());
			*(buf_p + 7) = (GLfloat) (request.size()->height()
					- vertical_space * 2 * Theme::instance->pixel());
			cursor_buffer_->unmap();
			cursor_buffer_->reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void TextEntry::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->text().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->text().shadetop,
					Theme::instance->text().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		outer_->bind();
		outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		RequestRedraw();
	}

	void TextEntry::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->text().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->text().shadetop,
					Theme::instance->text().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		outer_->bind();
		outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		font_.set_pen(radius,
				font_.pen().y());

		RequestRedraw();
	}

	ResponseType TextEntry::Draw (const Context* context)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				Theme::instance->text().inner.data());
		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		Shaders::instance->widget_outer_program()->use();

		glUniform4fv(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1,
				Theme::instance->text().outline.data());
		glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION), 0.f, 0.f);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
			        0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		if(focus()) {			// draw a cursor

			Shaders::instance->widget_triangle_program()->use();
			unsigned int cursor_pos = font_.GetTextWidth(text_,
						        index_ - start_, start_);
			cursor_pos += round_radius();

			glm::vec2 pos(0.f + cursor_pos, 0.f + 1);

			glUniform2fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), 1,
					glm::value_ptr(pos));
			glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
			glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);
			glVertexAttrib4f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 0.f,
					0.215f, 1.f, 0.75f);

			glBindVertexArray(vaos_[2]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		font_.Print(0.f, 0.f, text_, length_, start_);

		return Finish;
	}

	void TextEntry::FocusEvent (bool focus)
	{
		RequestRedraw();
	}

	void TextEntry::InitializeTextEntry ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->text().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->text().shadetop,
					Theme::instance->text().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(3, vaos_);

		glBindVertexArray(vaos_[0]);
		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_OUTER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_OUTER_COORD),
				2, GL_FLOAT, GL_FALSE, 0, 0);

		std::vector<GLfloat> cursor_vertices(8, 0.f);

		cursor_vertices[0] = 1.f;
		cursor_vertices[1] = (GLfloat) vertical_space * Theme::instance->pixel();

		cursor_vertices[2] = 3.f;
		cursor_vertices[3] = (GLfloat) vertical_space * Theme::instance->pixel();

		cursor_vertices[4] = 1.f;
		cursor_vertices[5] = (GLfloat) (size().height()
				- vertical_space * 2 * Theme::instance->pixel());

		cursor_vertices[6] = 3.f;
		cursor_vertices[7] = (GLfloat) (size().height()
				- vertical_space * 2 * Theme::instance->pixel());

		glBindVertexArray(vaos_[2]);
		cursor_buffer_.reset(new GLArrayBuffer);

		cursor_buffer_->generate();
		cursor_buffer_->bind();
		cursor_buffer_->set_data(sizeof(GLfloat) * cursor_vertices.size(), &cursor_vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD),
				2, GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::reset();
		glBindVertexArray(0);

		// TODO: count Theme::pixel for retina
		font_.set_pen(round_radius(),
				(size().height() - font_.GetHeight()) / 2
		                + std::abs(font_.GetDescender()));
	}

	int TextEntry::GetValidTextSize ()
	{
		int width = 0;
		int str_len = text_.length();

		width = font_.GetTextWidth(text_, str_len, 0);

		if(width > size().width()) {
			while(str_len > 0) {
				width = font_.GetTextWidth(text_, str_len, 0);
				if(width < size().width()) break;
				str_len--;
			}
		}

		return str_len;
	}
	
	void TextEntry::DisposeBackspacePress ()
	{
		if (text_.size() && index_ > 0) {

			size_t valid_width = size().width() - round_radius() * 2;

			text_.erase(index_ - 1, 1);
			index_--;

			size_t text_width = 0;
			size_t len = text_.length();
			while(len > 0)
			{
				text_width = font_.GetReversedTextWidth(text_, len, 0);
				if(text_width < valid_width) {
					break;
				}
				len--;
			}

			length_ = len;
			start_ = text_.length() - length_;

			RequestRedraw();
		}
	}
	
	void TextEntry::DisposeDeletePress ()
	{
		if (text_.size() && (index_ < static_cast<int>(text_.length()))) {

			size_t valid_width = size().width() - round_radius() * 2;

			text_.erase(index_, 1);

			size_t text_width = 0;

			size_t len = 0;
			while(len < (text_.length() - start_))
			{
				text_width = font_.GetTextWidth(text_, len, start_);
				if(text_width > valid_width) {
					len--;
					break;
				}
				len++;
			}

			length_ = len;

			RequestRedraw();
		}
	}
	
	void TextEntry::DisposeLeftPress ()
	{
		if (text_.size() && index_ > 0) {

			int valid_width = size().width() - round_radius() * 2;

			index_--;

			if (index_ < start_) {
				start_ = index_;

				int text_width = font_.GetTextWidth(text_, length_,
								start_);

				if((text_width < valid_width) && (length_ < (static_cast<int>(text_.length()) - start_))) {
					length_++;
					text_width = font_.GetTextWidth(text_, length_, start_);
					while(text_width < valid_width && length_ < (static_cast<int>(text_.length()) - start_)) {
						length_++;
						text_width = font_.GetTextWidth(text_, length_, start_);
					}
				}

				if(text_width > valid_width && length_ > 0) {
					length_--;
					text_width = font_.GetTextWidth(text_, length_, start_);
					while ((text_width > valid_width) && (length_ > 0)) {
						length_--;
						text_width = font_.GetTextWidth(text_, length_, start_);
					}
				}

			}

			RequestRedraw();
		}

	}
	
	void TextEntry::DisposeRightPress ()
	{
		if ((text_.size() > 0) && (index_ < static_cast<int>(text_.length()))) {
			index_++;

			if (index_ > (start_ + length_))
				start_++;

			//m_length = GetVisibleTextLength(m_text, m_start);

			//DBG_PRINT_MSG("length: %lu, start: %lu, cursor: %lu",
			//				m_length, m_start, m_cursor_position);

			RequestRedraw();
		}
	}

	void TextEntry::GetVisibleTextRange (size_t* start, size_t* length)
	{
		int str_len = text_.length();
		int width = font_.GetTextWidth(text_, str_len, 0);

		if(width < (size().width() - 4)) {
			*start = 0;
			*length = str_len;
		} else {
			while(str_len > 0) {
				str_len--;
				width = font_.GetTextWidth(text_, str_len, 0);
				if(width < (size().width() - 4)) break;
			}

			*start = text_.length() - str_len;
			*length = str_len;
		}
	}
	
	int TextEntry::GetCursorPosition (const Context* context)
	{
		Point global_pos = context->active_frame()->GetAbsolutePosition(this);

		int click_position = context->cursor_position().x() - global_pos.x()
						- round_radius();

		if((click_position < 0) ||
				(click_position > (size().width() - round_radius()))) {
			return index_;
		}

		int text_width = 0;
		int offset = 1;
		while (offset < length_) {
			text_width = font_.GetTextWidth(text_, offset, start_);
			if(text_width > click_position) {
				// FIXME: some character: e.g. 'f' always return small text width
				offset--;
				break;
			}
			offset++;
		}

		return start_ + offset;
	}

}
