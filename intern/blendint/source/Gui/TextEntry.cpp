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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	Margin TextEntry::kPadding = Margin(2, 2, 2, 2);

	TextEntry::TextEntry ()
	: Widget(),
	  start_(0),
	  length_(0),
	  index_(0)
	{
		int h = font_.height();
		int initial_width = h + round_radius() * 2 * AbstractWindow::theme->pixel()
				+ 120;
		int initial_height = h + vertical_space * 2 * AbstractWindow::theme->pixel();

		set_size(initial_width, initial_height);

		InitializeTextEntry();
	}

	TextEntry::~TextEntry ()
	{
		glDeleteVertexArrays(3, vao_);
	}

	void TextEntry::SetText (const String& text)
	{
		bool cal_width = true;

		text_ = text;

		Rect text_outline;	// = font_.GetTextOutline(text_);

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

//		font_.set_pen(round_radius(),
//				(size().height() - font_.height()) / 2 + std::abs(font_.descender()));

		index_ = text_.length();

		if(!text_ext_) {
			text_ext_.reset(new Text(text));
		} else {
	        text_ext_->SetText(text);
		}
        
		RequestRedraw();
	}

	void TextEntry::SetFont (const Font& font)
	{
		font_ = font;

		//Rect text_outline = m_font.GetTextOutline(m_text);

		//m_length = GetVisibleTextLengthInCursorMove(m_text, m_start);

//		font_.set_pen(round_radius(),
//				(size().height() - font_.height()) / 2 + std::abs(font_.descender()));
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

		int max_font_height = font_.height();

		preferred_size.set_height(
				max_font_height
						+ kPadding.vsum() * AbstractWindow::theme->pixel());// top padding: 2, bottom padding: 2

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

	ResponseType TextEntry::PerformKeyPress (AbstractWindow* context)
	{
		if(!context->GetTextInput().empty()) {

			text_.insert(index_, context->GetTextInput());
			index_ += context->GetTextInput().length();
			length_ += context->GetTextInput().length();

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

			switch (context->GetKeyInput()) {

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

	ResponseType TextEntry::PerformMousePress(AbstractWindow* context)
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

			if (AbstractWindow::theme->text().shaded) {
				GenerateRoundedVertices(Vertical,
						AbstractWindow::theme->text().shadetop,
						AbstractWindow::theme->text().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			vbo_.bind(0);
			vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			vbo_.bind(1);
			vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

            vbo_.bind(2);
			GLfloat* buf_p = (GLfloat*) vbo_.map(GL_READ_WRITE);
			*(buf_p + 5) = (GLfloat) (request.size()->height()
					- vertical_space * 2 * AbstractWindow::theme->pixel());
			*(buf_p + 7) = (GLfloat) (request.size()->height()
					- vertical_space * 2 * AbstractWindow::theme->pixel());
            vbo_.unmap();
			vbo_.reset();

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

		if (AbstractWindow::theme->text().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->text().shadetop,
					AbstractWindow::theme->text().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

        vbo_.bind(0);
        vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		RequestRedraw();
	}

	void TextEntry::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->text().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->text().shadetop,
					AbstractWindow::theme->text().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

        vbo_.bind(0);
        vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

//		font_.set_pen(radius,
//				font_.pen().y());

		RequestRedraw();
	}

	ResponseType TextEntry::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders->widget_inner_program()->use();

		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				AbstractWindow::theme->text().inner.data());
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1,
				AbstractWindow::theme->text().outline.data());
		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION), 0.f, 0.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
			        0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		if(focus()) {			// draw a cursor

			AbstractWindow::shaders->widget_triangle_program()->use();
			unsigned int cursor_pos = font_.GetTextWidth(text_,
						        index_ - start_, start_);
			cursor_pos += round_radius();

			glm::vec2 pos(0.f + cursor_pos, 0.f + 1);

			glUniform2fv(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_POSITION), 1,
					glm::value_ptr(pos));
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);
			glVertexAttrib4f(AttributeColor, 0.f,
					0.215f, 1.f, 0.75f);

			glBindVertexArray(vao_[2]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		//font_.Print(0.f, 0.f, text_, length_, start_);

        if(text_ext_) {
            
            int w = size().width() - pixel_size(kPadding.hsum());
            int h = size().height() - pixel_size(kPadding.vsum());
            int x = pixel_size(kPadding.left());
            int y = (size().height() - text_ext_->font().height()) / 2 - text_ext_->font().descender();
            
            // A workaround for Adobe Source Han Sans
            int diff = text_ext_->font().ascender() - text_ext_->font().descender();
            if(diff < text_ext_->font().height()) {
                y += (text_ext_->font().height() - diff - 1) / 2;
            }
            
//            if((alignment_ == AlignHorizontalCenter) || (alignment_ == AlignCenter)) {
//                x += (w - text_ext_->size().width()) / 2;
//            } else if (alignment_ == AlignRight) {
//                x = w - text_ext_->size().width();
//            }
            
            if(text_ext_->size().height() <= h) {
                text_ext_->DrawWithin(x, y, w, AbstractWindow::theme->text().text);
            }
            
        }

        
		return Finish;
	}

	void TextEntry::PerformFocusOn (AbstractWindow* context)
	{
		RequestRedraw();
	}

	void TextEntry::PerformFocusOff (AbstractWindow* context)
	{
		RequestRedraw();
	}

	void TextEntry::InitializeTextEntry ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->text().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->text().shadetop,
					AbstractWindow::theme->text().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(3, vao_);
        vbo_.generate();

		glBindVertexArray(vao_[0]);
		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord,
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord,
				2, GL_FLOAT, GL_FALSE, 0, 0);

		std::vector<GLfloat> cursor_vertices(8, 0.f);

		cursor_vertices[0] = 1.f;
		cursor_vertices[1] = (GLfloat) vertical_space * AbstractWindow::theme->pixel();

		cursor_vertices[2] = 3.f;
		cursor_vertices[3] = (GLfloat) vertical_space * AbstractWindow::theme->pixel();

		cursor_vertices[4] = 1.f;
		cursor_vertices[5] = (GLfloat) (size().height()
				- vertical_space * 2 * AbstractWindow::theme->pixel());

		cursor_vertices[6] = 3.f;
		cursor_vertices[7] = (GLfloat) (size().height()
				- vertical_space * 2 * AbstractWindow::theme->pixel());

		glBindVertexArray(vao_[2]);
		vbo_.bind(2);
		vbo_.set_data(sizeof(GLfloat) * cursor_vertices.size(), &cursor_vertices[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord,
				2, GL_FLOAT, GL_FALSE, 0, 0);

		vbo_.reset();
		glBindVertexArray(0);

		// TODO: count Theme::pixel for retina
//		font_.set_pen(round_radius(),
//				(size().height() - font_.height()) / 2
//		                + std::abs(font_.descender()));
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
				// text_width = font_.GetReversedTextWidth(text_, len, 0);
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
	
	int TextEntry::GetCursorPosition (AbstractWindow* context)
	{
		Point global_pos = context->active_frame()->GetAbsolutePosition(this);

		int click_position = context->GetCursorPosition().x() - global_pos.x()
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
