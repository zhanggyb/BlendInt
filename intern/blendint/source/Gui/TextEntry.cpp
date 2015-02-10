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
	  text_start_(0),
	  cursor_index_(0)
	{
		text_.reset(new Text(String("")));

		int w = text_->size().width();
		int h = text_->font().height();
		if(w < 160) w = 160;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);

		InitializeTextEntry();
	}

	TextEntry::TextEntry (const String& text)
	: Widget(),
	  text_start_(0),
	  cursor_index_(0)
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

    void TextEntry::ClearText ()
    {
        if(text_) {
            text_.destroy();
            cursor_index_ = 0;
            text_start_ = 0;
            
            RequestRedraw();
        }
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
    
    void TextEntry::PerformFocusOn (AbstractWindow* context)
    {
        RequestRedraw();
    }
    
    void TextEntry::PerformFocusOff (AbstractWindow* context)
    {
        RequestRedraw();
    }
    
    void TextEntry::PerformHoverIn(AbstractWindow* context)
    {
        context->PushCursor();
        context->SetCursor(IBeamCursor);
    }
    
    void TextEntry::PerformHoverOut(AbstractWindow* context)
    {
        context->PopCursor();
    }
    
	Response TextEntry::PerformKeyPress (AbstractWindow* context)
	{
		if(!context->GetTextInput().empty()) {

            int valid_width = size().width() - pixel_size(kPadding.hsum());

            if(text_->empty()) {

                text_->Add(context->GetTextInput());
    			cursor_index_ += context->GetTextInput().length();

            } else {

            	int prev_width = text_->size().width();
                text_->Insert(cursor_index_, context->GetTextInput());
                int new_width = text_->size().width();

    			cursor_index_ += context->GetTextInput().length();

                if(new_width <= valid_width) {

                	text_start_ = 0;

                } else {

                	int diff = new_width - prev_width;
                	assert(diff > 0);

                	size_t visible_width = text_->GetTextWidth(cursor_index_ - text_start_ + 1, text_start_, false);

                	if(visible_width > (size_t)valid_width) {

                    	size_t num = 0;
                		size_t char_width = 0;
                		while(char_width < (size_t)diff) {
                			num++;
                			char_width = text_->GetTextWidth(num, text_start_, true);
                		}
                		text_start_ += num;
                	}

                }

            }

			RequestRedraw();

			return Finish;

		} else {

            if(!text_) return Finish;
            
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

	Response TextEntry::PerformMousePress(AbstractWindow* context)
	{
		if(text_) {
			size_t index = GetTextCursorIndex(context);

			if(index != cursor_index_) {
				cursor_index_ = index;
				RequestRedraw();
			}
		}

		return Finish;
	}

    Response TextEntry::Draw (AbstractWindow* context)
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
        
        int x = pixel_size(kPadding.left());
        int y = (size().height() - text_->font().height()) / 2 - text_->font().descender();

        if(text_) {

            int w = size().width() - pixel_size(kPadding.hsum());
            int h = size().height() - pixel_size(kPadding.vsum());
            
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
            	cursor_pos = text_->DrawWithCursor(x,
            			y,
            			cursor_index_,
            			text_start_,
            			w,
            			AbstractWindow::theme->text().text);
            }
            
        }

		if(focus()) {			// draw a cursor

			x += cursor_pos;
			y = 0 + 1;

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

	void TextEntry::DisposeBackspacePress ()
	{
		if (text_->length() && cursor_index_ > 0) {

			int prev_width = text_->size().width();
            text_->Erase(cursor_index_ - 1, 1);
            int new_width = text_->size().width();

			cursor_index_--;

			if(text_start_ > 0) {

				int diff = prev_width - new_width;

            	size_t num = 0;
        		size_t char_width = 0;
        		while((char_width < (size_t)diff) && ((text_start_ - num) > 0)) {
        			num++;
        			char_width = text_->GetTextWidth(num, text_start_ - num, true);
        		}
        		text_start_ -= num;

			}

			RequestRedraw();
		}
	}
	
	void TextEntry::DisposeDeletePress ()
	{
		if (text_->length() && (cursor_index_ < text_->length())) {

            text_->Erase(cursor_index_, 1);

			RequestRedraw();
		}
	}
	
	void TextEntry::DisposeLeftPress ()
	{
		if (text_->length() && cursor_index_ > 0) {

			if(cursor_index_ == text_start_) {
				text_start_--;
			}

			cursor_index_--;
            RequestRedraw();
		}

	}
	
	void TextEntry::DisposeRightPress ()
	{
		if ((text_->length() > 0) && (cursor_index_ < text_->length())) {

			int valid_width = size().width() - pixel_size(kPadding.hsum());
        	size_t visible_width = text_->GetTextWidth(cursor_index_ - text_start_ + 1, text_start_, false);

			cursor_index_++;
        	if(visible_width > (size_t)valid_width) {
        		text_start_ += 1;
        	}

			RequestRedraw();
		}
	}

	size_t TextEntry::GetTextCursorIndex (AbstractWindow* context)
	{
		if (text_->empty())
			return 0;

		Point local_position = context->GetCursorPosition()
		        - context->active_frame()->GetAbsolutePosition(this);

		int x = local_position.x() - pixel_size(kPadding.left());
		if (x < 0)
			return 0;
		if (x >= text_->size().width())
			return text_->length();

		size_t n = 0;

		size_t dist = 0;
		while ((dist < (size_t) x) && (n < text_->length())) {
			n++;
			dist = text_->GetTextWidth(n, text_start_, true);
		}

		return text_start_ + n - 1;
	}

}
