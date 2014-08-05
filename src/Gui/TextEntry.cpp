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

namespace BlendInt {

	using Stock::Shaders;

	TextEntry::TextEntry ()
	: AbstractWidget(),
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

		int radius_plus = 0;

		if ((round_type() & RoundTopLeft) || (round_type() & RoundBottomLeft)) {
			radius_plus += round_radius();
		}

		if ((round_type() & RoundTopRight)
				|| (round_type() & RoundBottomRight)) {
			radius_plus += round_radius();
		}

		int max_font_height = font_.GetHeight();

		preferred_size.set_height(
				max_font_height
						+ vertical_space * 2 * Theme::instance->pixel());// top padding: 2, bottom padding: 2

		if (text().empty()) {
			preferred_size.set_width(max_font_height + radius_plus + 120);
		} else {
			int width = font_.GetTextWidth(text());
			preferred_size.set_width(width + radius_plus);
		}

		return preferred_size;
	}

	bool TextEntry::IsExpandX () const
	{
		return true;
	}

	ResponseType TextEntry::KeyPressEvent (const KeyEvent& event)
	{
		if(!event.text().empty()) {

			text_.insert(index_, event.text());
			index_ += event.text().length();
			length_ += event.text().length();

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

			Refresh();
			return Accept;

		} else {

			switch (event.key()) {

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

			return Accept;
		}
	}

	ResponseType TextEntry::MousePressEvent(const MouseEvent& event)
	{
		if(text_.size()) {
			index_ = GetCursorPosition(event);

			DBG_PRINT_MSG("index: %d", index_);

			Refresh();
		}

		return Accept;
	}

	void TextEntry::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			const Color& color = Theme::instance->text().inner;
			short shadetop = Theme::instance->text().shadetop;
			short shadedown = Theme::instance->text().shadedown;

			VertexTool tool;
			tool.Setup(*request.size(), DefaultBorderWidth(), round_type(),
			        round_radius(), color, Vertical, shadetop, shadedown);
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			cursor_buffer_->Bind();
			GLfloat* buf_p = (GLfloat*) cursor_buffer_->Map(
			GL_READ_WRITE);
			*(buf_p + 5) = (GLfloat) (request.size()->height()
					- vertical_space * 2 * Theme::instance->pixel());
			*(buf_p + 7) = (GLfloat) (request.size()->height()
					- vertical_space * 2 * Theme::instance->pixel());
			cursor_buffer_->Unmap();
			cursor_buffer_->Reset();

			set_size(*request.size());
			Refresh();

		}

		ReportSizeUpdate(request);
	}

	void TextEntry::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if (request.target() == this) {

			const Color& color = Theme::instance->text().inner;
			short shadetop = Theme::instance->text().shadetop;
			short shadedown = Theme::instance->text().shadedown;

			VertexTool tool;
			tool.Setup(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius(), color, Vertical, shadetop, shadedown);
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			set_round_type(*request.round_type());
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void TextEntry::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if (request.target() == this) {
			const Color& color = Theme::instance->text().inner;
			short shadetop = Theme::instance->text().shadetop;
			short shadedown = Theme::instance->text().shadedown;

			VertexTool tool;
			tool.Setup(size(), DefaultBorderWidth(), round_type(),
			        *request.round_radius(), color, Vertical, shadetop,
			        shadedown);
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			font_.set_pen(
			        *request.round_radius(),
			        font_.pen().y());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType TextEntry::Draw (const RedrawEvent& event)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(),
				(float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(),
				0);

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
				Theme::instance->text().outline.data());
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		if(focused()) {			// draw a cursor
			unsigned int cursor_pos = font_.GetTextWidth(text_,
						        index_ - start_, start_);
			cursor_pos += round_radius();

			glm::vec3 pos(position().x() + cursor_pos, position().y() + 1, 0.f);

			glUniform3fv(Shaders::instance->triangle_uniform_position(), 1,
					glm::value_ptr(pos));
			glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
			glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

			glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.f,
					0.215f, 1.f, 0.75f);

			glBindVertexArray(vaos_[2]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		program->Reset();

		font_.Print(position(), text_, length_, start_);

		return Accept;
	}

	ResponseType TextEntry::FocusEvent (bool focus)
	{
		Refresh();
		return Accept;
	}

	void TextEntry::InitializeTextEntry ()
	{
		VertexTool tool;
		tool.Setup(size(),
				DefaultBorderWidth(),
				round_type(),
				round_radius(),
				Theme::instance->text().inner,
				Vertical,
				Theme::instance->text().shadetop,
				Theme::instance->text().shadedown);

		glGenVertexArrays(3, vaos_);

		glBindVertexArray(vaos_[0]);
		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		tool.SetInnerBufferData(inner_.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_color());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->triangle_attrib_color(),
				4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->Generate();
		outer_->Bind();
		tool.SetOuterBufferData(outer_.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(),
				2, GL_FLOAT, GL_FALSE, 0, 0);

		std::vector<GLfloat> cursor_vertices(8);

		cursor_vertices[0] = 1.f;
		cursor_vertices[1] = (GLfloat) vertical_space;

		cursor_vertices[2] = 3.f;
		cursor_vertices[3] = (GLfloat) vertical_space;

		cursor_vertices[4] = 1.f;
		cursor_vertices[5] = (GLfloat) (size().height()
				- vertical_space * 2 * Theme::instance->pixel());

		cursor_vertices[6] = 3.f;
		cursor_vertices[7] = (GLfloat) (size().height()
				- vertical_space * 2 * Theme::instance->pixel());

		glBindVertexArray(vaos_[2]);
		cursor_buffer_.reset(new GLArrayBuffer);

		cursor_buffer_->Generate();
		cursor_buffer_->Bind();
		cursor_buffer_->SetData(8 * sizeof(GLfloat), &cursor_vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLArrayBuffer::Reset();
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

			Refresh();
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

			Refresh();
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

			Refresh();
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

			Refresh();
		}
	}

	ResponseType TextEntry::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType TextEntry::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TextEntry::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TextEntry::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType TextEntry::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
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
	
	int TextEntry::GetCursorPosition (const MouseEvent& event)
	{
		int click_position = event.position().x() - position().x()
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
