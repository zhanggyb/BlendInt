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

#include <BlendInt/OpenGL/GLHeader.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Margin Label::kPadding (2, 2, 2, 2);

	Label::Label (const String& text, Alignment alignment)
    : Widget(),
	  alignment_(alignment)
	{
		text_.reset(new Text(text));

		DBG_PRINT_MSG("descender: %d", text_->font().descender());

		int w = text_->size().width();
		int h = text_->font().height();
		if(w < 80) w = 80;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);
	}

	Label::~Label ()
	{
	}

	void Label::SetText (const String& text)
	{
		text_->SetText(text);
		RequestRedraw();
	}

	void Label::SetFont (const Font& font)
	{
		text_->SetFont(font);
		RequestRedraw();
	}

	void Label::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size (*request.size());
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType Label::Draw (const Context* context)
	{
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

			if((alignment_ == AlignHorizontalCenter) || (alignment_ == AlignCenter)) {
				x += (w - text_->size().width()) / 2;
			} else if (alignment_ == AlignRight) {
				x = w - text_->size().width();
			}

			if(text_->size().height() <= h) {
				text_->DrawWithin(x, y, w);
			}

		}

		return Finish;
	}

	Size Label::GetPreferredSize () const
	{
		int h = text_->font().height();
		int w = text_->size().width();

		if(w < 80) w = 80;

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		return Size(w, h);
	}

	bool Label::IsExpandX() const
	{
		return true;
	}

} /* namespace BlendInt */

