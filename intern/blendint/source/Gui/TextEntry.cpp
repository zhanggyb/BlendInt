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
		Font font;	// default font
		int w = 160;
		int h = font.height();

		set_size(w + pixel_size(kPadding.hsum()),
		        h + pixel_size(kPadding.vsum()));

		InitializeTextEntry();
	}

	TextEntry::TextEntry (const String& text)
	: Widget(),
	  start_(0),
	  length_(0),
	  index_(0)
	{
		text_.reset(new Text(text));

		int w = text_->size().width();
		int h = text_->font().height();
		if(w < 160) w = 160;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);

		InitializeTextEntry();
	}

	TextEntry::~TextEntry ()
	{
		glDeleteVertexArrays(3, vao_);
	}

	void TextEntry::SetText (const String& text)
	{
		if(!text_) {
			text_.reset(new Text(text));
		} else {
	        text_->SetText(text);
		}
        
        //index_ = 0;
        
		RequestRedraw();
	}

	void TextEntry::SetFont (const Font& font)
	{
		if(text_) {
			if(!(text_->font() == font)) {
				text_->SetFont(font);
				RequestRedraw();
			}
		}
	}

	void TextEntry::Clear ()
	{
		if(text_) {
			text_.destroy();
			index_ = 0;
			start_ = 0;
			length_ = 0;

			RequestRedraw();
		}
	}

	Size TextEntry::GetPreferredSize () const
	{
		int w = 0;
		int h = 0;

		Font font;	// default font
		if(text_) {
			font = text_->font();
			w += text_->size().width();
		}

		h = std::max(h, font.height());

		if (w == 0) w = h;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		if(!text_) {
			if(w < 160) w = 160;
		}

		return Size(w, h);
	}

	bool TextEntry::IsExpandX () const
	{
		return true;
	}

	ResponseType TextEntry::PerformKeyPress (AbstractWindow* context)
	{
		if(!context->GetTextInput().empty()) {

			text_->Insert(index_, context->GetTextInput());
			index_ += context->GetTextInput().length();

			RequestRedraw();

			/*
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
			*/
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
		if(text_) {

			index_ = 0;
			//index_ = GetCursorPosition(context);

			//DBG_PRINT_MSG("index: %d", index_);

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

        int cursor_pos = 0;
        
        if(text_) {

            int w = size().width() - pixel_size(kPadding.hsum());
            int h = size().height() - pixel_size(kPadding.vsum());
            int x = pixel_size(kPadding.left());
            int y = (size().height() - text_->font().height()) / 2 - text_->font().descender();
            
            // A workaround for Adobe Source Han Sans
            int diff = text_->font().ascender() - text_->font().descender();
            if(diff < text_->font().height()) {
                y += (text_->font().height() - diff - 1) / 2;
            }
            
//            if((alignment_ == AlignHorizontalCenter) || (alignment_ == AlignCenter)) {
//                x += (w - text_ext_->size().width()) / 2;
//            } else if (alignment_ == AlignRight) {
//                x = w - text_ext_->size().width();
//            }
            
            if(text_->size().height() <= h) {
            	cursor_pos = text_->DrawWithCursor(x, y, index_, text_->text().length(), 0, w, AbstractWindow::theme->text().text);
//                text_->Draw(x, y, 5, 1);
            }
            
        }

		if(focus()) {			// draw a cursor

			float x = 0.f;
			float y = 0.f;

			//unsigned int cursor_pos = text_->font().GetTextWidth(text_->text(), index_ - start_, start_);
			//cursor_pos += pixel_size(kPadding.left());
			cursor_pos += pixel_size(kPadding.left());

            //DBG_PRINT_MSG("cursor pos: %d", cursor_pos);
        
			x = 0.f + cursor_pos;
			y = 0.f + 1;

			AbstractWindow::shaders->widget_triangle_program()->use();
			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_POSITION), x, y);
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);
			glVertexAttrib4f(AttributeColor, 0.f, 0.f, 0.f, 1.f);
            // glVertexAttrib4f(AttributeColor, 0.f, 0.215f, 1.f, 0.75f);

			glBindVertexArray(vao_[2]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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

		cursor_vertices[0] = 0.f;
		cursor_vertices[1] = (GLfloat) vertical_space * AbstractWindow::theme->pixel();

		cursor_vertices[2] = 1.f;
		cursor_vertices[3] = (GLfloat) vertical_space * AbstractWindow::theme->pixel();

		cursor_vertices[4] = 0.f;
		cursor_vertices[5] = (GLfloat) (size().height()
				- vertical_space * 2 * AbstractWindow::theme->pixel());

		cursor_vertices[6] = 1.f;
		cursor_vertices[7] = (GLfloat) (size().height()
				- vertical_space * 2 * AbstractWindow::theme->pixel());

		glBindVertexArray(vao_[2]);
		vbo_.bind(2);
		vbo_.set_data(sizeof(GLfloat) * cursor_vertices.size(), &cursor_vertices[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord,
				2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		vbo_.reset();
	}

	int TextEntry::GetVisibleTextLength ()
	{
		int width = 0;
		int str_len = text_->text().length();

		width = text_->font().GetTextWidth(text_->text(), str_len, 0);

		if(width > size().width()) {
			while(str_len > 0) {
				width = text_->font().GetTextWidth(text_->text(), str_len, 0);
				if(width < size().width()) break;
				str_len--;
			}
		}

		return str_len;
	}
	
	void TextEntry::DisposeBackspacePress ()
	{
		if (text_->text().size() && index_ > 0) {

			size_t valid_width = size().width() - round_radius() * 2;

			//text_.erase(index_ - 1, 1);
			index_--;

			size_t text_width = 0;
			size_t len = text_->text().length();
			while(len > 0)
			{
				// text_width = font_.GetReversedTextWidth(text_, len, 0);
				if(text_width < valid_width) {
					break;
				}
				len--;
			}

			length_ = len;
			//start_ = text_.length() - length_;

			RequestRedraw();
		}
	}
	
	void TextEntry::DisposeDeletePress ()
	{
		if (text_->text().size() && (index_ < static_cast<int>(text_->text().length()))) {

			size_t valid_width = size().width() - round_radius() * 2;

			//text_.erase(index_, 1);

			size_t text_width = 0;

			size_t len = 0;
			while(len < (text_->text().length() - start_))
			{
				text_width = text_->font().GetTextWidth(text_->text(), len, start_);
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
		if (text_->text().size() && index_ > 0) {

			int valid_width = size().width() - round_radius() * 2;

			index_--;

			if (index_ < start_) {
				start_ = index_;

				int text_width = text_->font().GetTextWidth(text_->text(), length_,
								start_);

				if((text_width < valid_width) && (length_ < (static_cast<int>(text_->text().length()) - start_))) {
					length_++;
					text_width = text_->font().GetTextWidth(text_->text(), length_, start_);
					while(text_width < valid_width && length_ < (static_cast<int>(text_->text().length()) - start_)) {
						length_++;
						text_width = text_->font().GetTextWidth(text_->text(), length_, start_);
					}
				}

				if(text_width > valid_width && length_ > 0) {
					length_--;
					text_width = text_->font().GetTextWidth(text_->text(), length_, start_);
					while ((text_width > valid_width) && (length_ > 0)) {
						length_--;
						text_width = text_->font().GetTextWidth(text_->text(), length_, start_);
					}
				}

			}

			RequestRedraw();
		}

	}
	
	void TextEntry::DisposeRightPress ()
	{
		if ((text_->text().size() > 0) && (index_ < static_cast<int>(text_->text().length()))) {
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
		int str_len = text_->text().length();
		int width = text_->font().GetTextWidth(text_->text(), str_len, 0);

		if(width < (size().width() - 4)) {
			*start = 0;
			*length = str_len;
		} else {
			while(str_len > 0) {
				str_len--;
				width = text_->font().GetTextWidth(text_->text(), str_len, 0);
				if(width < (size().width() - 4)) break;
			}

			*start = text_->text().length() - str_len;
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
			text_width = text_->font().GetTextWidth(text_->text(), offset, start_);
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
