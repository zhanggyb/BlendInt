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

#include <gui/expander.hpp>
#include <gui/linear-layout.hpp>
#include <gui/abstract-window.hpp>
#include <stock/icons.hpp>

namespace BlendInt {

	ExpandButton::ExpandButton()
	: AbstractButton(AbstractWindow::icons->num())
	{
		set_checkable(true);

		int w = this->icon()->size().width();
		int h = this->icon()->size().height();

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);
	}

	ExpandButton::ExpandButton (const String& text)
	: AbstractButton(AbstractWindow::icons->num(), text)
	{
		set_checkable(true);

		int w = this->icon()->size().width();
		int h = this->icon()->size().height();

		w += kIconTextSpace;

		w += this->text()->size().width();
		h = std::max(h, this->text()->font().height());

		if(w < 80) w = 80;
		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		set_size(w, h);
	}

	ExpandButton::~ExpandButton ()
	{
	}

	bool ExpandButton::IsExpandX () const
	{
		return true;
	}

	void ExpandButton::SetText(const String& text)
	{
		set_text(text);
		RequestRedraw();
	}

	void ExpandButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ExpandButton::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		RequestRedraw();
	}

	void ExpandButton::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);
		RequestRedraw();
	}

	Response ExpandButton::Draw (AbstractWindow* context)
	{
		float rotate = is_checked() ? 0.f : -90.f;
		short gamma = hover() ? 25 : 0;

		Rect rect(pixel_size(kPadding.left()),
		          pixel_size(kPadding.bottom()),
		          size().height() - pixel_size(kPadding.vsum()),
		          size().height() - pixel_size(kPadding.vsum()));

		if(icon()) {
		  icon()->DrawInRect(rect, AlignCenter, Color(0x0F0F0FFF).data(), gamma, rotate, true);
		  rect.set_x(rect.width() + kIconTextSpace);
		  rect.set_width(size().width() - pixel_size(kPadding.hsum()) - kIconTextSpace - icon()->size().width());
		} else {
      rect.set_width(size().width() - pixel_size(kPadding.hsum()));
		}

		if(text()) {
		  text()->DrawInRect(rect, AlignHorizontalCenter | AlignJustify | AlignBaseline, Color(Color::Black).data(), gamma);
		}

		return Finish;
	}

	// ----------------------

	Expander::Expander (const String& title,
	                    AbstractLayout* layout,
	                    Orientation orient,
	                    int align,
	                    int space)
	: AbstractWidget(),
	  title_(0),
	  layout_(0),
	  last_size_(0),
	  orientation_(orient),
	  alignment_(align),
	  space_(space)
	{
	  title_ = Manage(new ExpandButton(title));

		if(layout) {
		  layout_ = layout;
		} else {
		  layout_ = Manage(new LinearLayout(orient));
		}

		PushBackSubView(title_);	// 0
		PushBackSubView(layout_);	// 1

		int width = 0;
		int height = 0;

		Size tmp = title_->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		height += space_;

    tmp = layout_->GetPreferredSize();
    width = std::max(width, tmp.width());
    height += tmp.height();

		set_size(width, height);

		FillWidgets(size());

    if (orientation_ == Horizontal) {
      last_size_ = layout_->size().width();
    } else {
      last_size_ = layout_->size().height();
    }

		events()->connect(title_->toggled(), this, &Expander::OnToggled);
	}

	Expander::~Expander ()
	{
	}

	bool Expander::AddWidget (AbstractWidget* widget)
	{
	  if(layout_->AddWidget(widget)) {

	    // TODO: change size

	    layout_->Adjust();

	    return true;
	  }

	  return false;
	}

  bool Expander::InsertWidget (int index, AbstractWidget* widget)
  {
    if(layout_->InsertWidget(index, widget)) {

      // TODO: change size
      layout_->Adjust();

      return true;
    }

    return false;
  }

  bool Expander::InsertWidget (int row, int column, AbstractWidget* widget)
  {
    if(layout_->InsertWidget(row, column, widget)) {

      // TODO: change size
      layout_->Adjust();

      return true;
    }

    return false;
  }

	Size Expander::GetPreferredSize() const
	{
		int w = 0;
		int h = 0;
		Size tmp;

		int count = 0;

		if(orientation_ == Horizontal) {

      for(AbstractView* p = first_subview(); p; p = p->next_view())
      {
        if(p->visiable()) {
          tmp = p->GetPreferredSize();
          w += tmp.width();
          h = std::max(h, tmp.height());
          count++;
        }
      }

      h += (count * space_);

		} else {

	    for(AbstractView* p = first_subview(); p; p = p->next_view())
	    {
	      if(p->visiable()) {
	        tmp = p->GetPreferredSize();
	        w = std::max(w, tmp.width());
	        h += tmp.height();
	        count++;
	      }
	    }

	    h += (count * space_);

		}

		return Size(w, h);
	}

	bool Expander::IsExpandX () const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool Expander::IsExpandY () const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	void Expander::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());

			FillWidgets();

			RequestRedraw();
		}

		ReportSizeUpdate(request);
	}

	Response Expander::Draw (AbstractWindow* context)
	{
		return Ignore;
	}

	void Expander::FillWidgets()
	{
	  if(size().width() <= 0 || size().height() <= 0) {
	    SetSubViewVisibility(title_, false);
	    SetSubViewVisibility(layout_, false);
	    return;
	  }

    int title_size_hint = 0;
    int x = 0;
    int y = 0;
    int width = size().width();
    int height = size().height();

    if(orientation_ == Horizontal) {

      title_size_hint = title_->GetPreferredSize().width();

      if (layout_->visiable()) {

        if (title_size_hint < width) {

          ResizeSubView(title_, title_size_hint, height);
          MoveSubViewTo(title_, x, y);

          x += title_size_hint;
          x += space_;

          ResizeSubView(layout_, width - title_size_hint, height);
          MoveSubViewTo(layout_, x, y);

        } else {

          ResizeSubView(title_, width, height);
          MoveSubViewTo(title_, x, y);
          ResizeSubView(layout_, 0, height);
          MoveSubViewTo(layout_, x, y);
        }

      } else {
        ResizeSubView(title_, width, height);
        MoveSubViewTo(title_, x, y);
      }

    } else {

      title_size_hint = title_->GetPreferredSize().height();

      if (layout_->visiable()) {

        if (title_size_hint < height) {

          y = y + height;
          ResizeSubView(title_, width, title_size_hint);
          y -= title_size_hint;
          MoveSubViewTo(title_, x, y);

          ResizeSubView(layout_, width, height - title_size_hint);
          y -= layout_->size().height();
          y -= space_;

          MoveSubViewTo(layout_, x, y);

        } else {

          ResizeSubView(title_, width, height);
          MoveSubViewTo(title_, x, y);
          ResizeSubView(layout_, width, 0);
          MoveSubViewTo(layout_, x, y);
        }

      } else {
        ResizeSubView(title_, width, height);
        MoveSubViewTo(title_, x, y);
      }

    }
	}

	void Expander::FillWidgets (const Size& out_size)
	{
		int x = 0;
		int y = 0;
		int w = out_size.width();
		int h = out_size.height();

		if(w >= 0 && h >= 0)
			FillWidgets(x, y, w, h);
	}

  void Expander::FillWidgets (int x, int y, int width, int height)
  {
    int button_preferred_height = 0;

    button_preferred_height = title_->GetPreferredSize().height();

    if (layout_->visiable()) {

      if (button_preferred_height < height) {

        y = y + height;
        ResizeSubView(title_, width, button_preferred_height);
        y -= button_preferred_height;
        MoveSubViewTo(title_, x, y);

        ResizeSubView(layout_, width, height - button_preferred_height);
        y -= layout_->size().height();
        y -= space_;

        MoveSubViewTo(layout_, x, y);

      } else {

        ResizeSubView(title_, width, height);
        MoveSubViewTo(title_, x, y);
        ResizeSubView(layout_, width, 0);
        MoveSubViewTo(layout_, x, y);
      }

    } else {
      ResizeSubView(title_, width, height);
      MoveSubViewTo(title_, x, y);
    }
  }
	
	void Expander::SetTitle (const String& text)
	{
	  title_->SetText(text);
	}

	const String& Expander::GetTitle () const
	{
		return title_->text()->text();
	}

	void Expander::OnToggled (AbstractButton* sender, bool toggle)
  {
    if (toggle) {

      last_size_ = layout_->size().height();
      SetSubViewVisibility(layout_, false);
      Resize(size().width(), title_->size().height());

    } else {

      SetSubViewVisibility(layout_, true);
      Resize(size().width(), title_->size().height() + space_ + last_size_);

    }

    RequestRedraw();
  }

}
