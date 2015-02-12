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

#include <gui/decoration.hpp>
#include <gui/label.hpp>

#include <gui/abstract-window.hpp>

namespace BlendInt {

	Decoration::Decoration()
	: AbstractDecoration(),
	  close_button_(nullptr)
	{
		// create close button
		close_button_ = Manage(new CloseButton);

		PushBackSubView(close_button_);

		MoveSubViewTo(close_button_, pixel_size(kPadding.left()), pixel_size(kPadding.bottom()));
		set_size(200 + pixel_size(kPadding.hsum()), close_button_->size().height() + pixel_size(kPadding.vsum()));

		events()->connect(close_button_->clicked(), this, &Decoration::OnCloseButtonClicked);
	}

	Decoration::Decoration(const String& title)
	: AbstractDecoration(),
	  close_button_(nullptr)
	{
		// create close button
		close_button_ = Manage(new CloseButton);
		Label* label = Manage(new Label(title, AlignCenter));

		int x = pixel_size(kPadding.left());
		int y = pixel_size(kPadding.bottom());

		int w = close_button_->size().width() + label->size().width();
		int h = std::max(close_button_->size().height(), label->size().height());

		PushBackSubView(close_button_);
		PushBackSubView(label);

		set_size(w + pixel_size(kPadding.hsum()), h + pixel_size(kPadding.vsum()));

		MoveSubViewTo(close_button_, x, y + (h - close_button_->size().height()) / 2);
		x += close_button_->size().width();
		MoveSubViewTo(label, x, y + (h - label->size().height()) / 2);

		events()->connect(close_button_->clicked(), this, &Decoration::OnCloseButtonClicked);
	}

	Decoration::~Decoration ()
	{
	}

	bool Decoration::IsExpandX () const
	{
		return true;
	}

	bool Decoration::IsExpandY () const
	{
		return false;
	}

	Size Decoration::GetPreferredSize () const
	{
		int w = 0;
		int h = 0;

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			w += p->size().width();
			h = std::max(h, p->size().height());
		}

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		return Size(w, h);
	}

	void Decoration::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			int x = pixel_size(kPadding.left());
			//int y = pixel_size(kPadding.bottom());

			MoveSubViewTo(close_button_, x, (size().height() - close_button_->size().height()) / 2);
			x += close_button_->size().width();

			if(subs_count() > 1) {
				MoveSubViewTo(last_subview(), x, (size().height() - last_subview()->size().height()) / 2);
				ResizeSubView(last_subview(), size().width() - pixel_size(kPadding.right()) - x, size().height() - pixel_size(kPadding.vsum()));
			}

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	Response Decoration::Draw (AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	void Decoration::OnCloseButtonClicked(AbstractButton* button)
	{
		fire_close_triggered();
	}

}
