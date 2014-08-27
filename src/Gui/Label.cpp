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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Label::Label (const String& text)
		: AbstractWidget(),
		  text_(text),
		  text_length_(0),
		  background_color_(0x00000000),
		  vao_(0)
	{
		InitializeLabel(text);
	}

	Label::~Label ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Label::SetText (const String& text)
	{
		text_ = text;
		text_length_ = UpdateTextPosition(size(), text, font_);

		Refresh();
	}

	void Label::SetFont (const Font& font)
	{
		font_ = font;
		text_length_ = UpdateTextPosition(size(), text_, font_);

		Refresh();
	}

	void Label::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			text_length_ = UpdateTextPosition(*request.size(), text_, font_);

			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0.f);

			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			GLArrayBuffer::reset();

			set_size (*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	ResponseType Label::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(), background_color_.data());

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->reset();

		if(text_.length()) {
			font_.Print(position(), text_, text_length_, 0);
		}

		return Accept;
	}

	size_t Label::UpdateTextPosition(const Size& size, const String& text, Font& font)
	{
		size_t str_len = 0;

		// If size changed, we need to update the text length for printing too.
		bool cal_width = true;

		int width = size.width() - 2 - 2;
		int height = size.height() - 2 - 2;

		if(width <= 0 || height <= 0) {
			return 0;
		}

		if(text.length() == 0) {
			return 0;
		}

		Rect text_outline = font.GetTextOutline(text);

		if(height < text_outline.height()) {
			str_len = 0;
			cal_width = false;
		}

		if(cal_width) {
			if(width < text_outline.width()) {
				str_len = GetValidTextSize(size, text, font);
			} else {
				str_len = text.length();
			}
		}

		font.set_pen((size.width() - text_outline.width()) / 2,
						(size.height() - font.GetHeight()) / 2
										+ std::abs(font.GetDescender()));
		return str_len;
	}

	size_t Label::GetValidTextSize(const Size& size, const String& text, const Font& font)
	{
		int width = 0;
		int str_len = text.length();

		width = font.GetTextWidth(text, str_len, 0);

		int text_width_space = size.width() - 2 - 2;

		if(width > text_width_space) {
			while(str_len > 0) {
				width = font.GetTextWidth(text, str_len, 0);
				if(width < text_width_space) break;
				str_len--;
			}
		}

		return str_len;
	}
	
	Size Label::GetPreferredSize () const
	{
		Size preferred_size;

		int max_font_height = font().GetHeight();

		preferred_size.set_height(max_font_height + 2 + 2);	// top padding: 2, bottom padding: 2

		if (text_.empty()) {
			preferred_size.set_width(
							max_font_height + 2 + 2);
		} else {
			size_t width = font().GetTextWidth(text_);
			preferred_size.set_width(width + 2 + 2);	// left padding: 2, right padding: 2
		}

		return preferred_size;
	}

	void Label::SetForegroundColor (const Color& fg)
	{
		font_.set_color(fg);
		Refresh();
	}

	void Label::SetBackgroundColor (const Color& color)
	{
		background_color_ = color;
		Refresh();
	}

	bool Label::IsExpandX() const
	{
		return true;
	}

	ResponseType Label::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Label::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Label::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Label::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Label::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Label::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Label::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Label::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Label::InitializeLabel (const String& text)
	{
		text_ = text;

		int h = font_.GetHeight();

		if(text.empty()) {
			set_size (h + 2 + 2,
							h + 2 + 2);
		} else {
			text_length_ = text.length();
			Rect text_outline = font_.GetTextOutline(text);

			int width = text_outline.width() + 2 + 2;
			int height = h + 2 + 2;
			set_size(width, height);
			font_.set_pen((width - text_outline.width()) / 2,
							(height - font_.GetHeight()) / 2 +
											std::abs(font_.GetDescender()));
		}

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0.f);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());	// 0 is the locaiton in shader
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLArrayBuffer::reset();
		glBindVertexArray(0);
	}

} /* namespace BlendInt */

