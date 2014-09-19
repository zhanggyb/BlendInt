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

#include <BlendInt/Gui/AbstractPanel.hpp>

namespace BlendInt {

	AbstractPanel::AbstractPanel ()
	: Container(),
	  decoration_(0),
	  content_(0),
	  space_(2)
	{
		set_size(360, 360);
	}

	AbstractPanel::~AbstractPanel ()
	{
	}

	void AbstractPanel::SetDecoration (AbstractWidget* widget)
	{
		if(widget == 0) return;
		if(widget == decoration_) return;	// already in this panel

		if(decoration_) {
			DBG_PRINT_MSG("TODO: %s", "delete original decoration");

			if(decoration_->managed() && (decoration_->reference_count() == 0))
			{
				delete decoration_;
			} else {
				DBG_PRINT_MSG("Warning: %s is not set managed and will not be deleted", decoration_->name().c_str());
			}
			decoration_ = 0;
		}

		if(InsertSubWidget(DecorationIndex, widget)) {
			decoration_ = widget;
			FillSubWidgets(position(), size(), margin());
		}
	}

	void AbstractPanel::SetContent (AbstractWidget* widget)
	{
		if(widget == 0) return;

		if(widget == content_) return;	// already in this panel

		if (content_) {
			DBG_PRINT_MSG("TODO: %s", "delete tail widgets");

			if(content_->managed() && (content_->reference_count() == 0))
			{
				delete content_;
			} else {
				DBG_PRINT_MSG("Warning: %s is not set managed and will not be deleted", content_->name().c_str());
			}

			content_ = 0;
		}

		if(InsertSubWidget(ContentIndex, widget)) {
			content_ = widget;
			FillSubWidgets(position(), size(), margin());
		}
	}

	void AbstractPanel::SetSpace (int space)
	{
		space_ = space;

		FillSubWidgets(position(), size(), margin());
	}

	Size AbstractPanel::GetPreferredSize () const
	{
		if(widget_count() == 0) {
			return Size(360, 360);
		}

		Size prefer;

		Size tmp;
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			tmp = p->GetPreferredSize();

			prefer.set_width(std::max(prefer.width(), tmp.width()));
			prefer.add_height(tmp.height());
		}

		prefer.add_height((widget_count() - 1) * space_);

		prefer.add_width(margin().hsum());
		prefer.add_height(margin().vsum());

		return prefer;
	}

	void AbstractPanel::PerformMarginUpdate (const Margin& margin)
	{
		FillSubWidgets(position(), size(), margin);
	}

	ResponseType AbstractPanel::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType AbstractPanel::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractPanel::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractPanel::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	void AbstractPanel::FillSubWidgets (const Point& out_pos,
	        const Size& size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = size.width() - margin.hsum();
		int h = size.height() - margin.vsum();

		FillSubWidgets(x, y, w, h);
	}

	void AbstractPanel::FillSubWidgets (int x, int y, int w, int h)
	{
		if (decoration_) {

			Size dec_prefer = decoration_->GetPreferredSize();

			y = y + h;

			if (content_) {

				if (h > dec_prefer.height()) {
					ResizeSubWidget(decoration_, w, dec_prefer.height());
					ResizeSubWidget(content_, w, h - dec_prefer.height() - space_);
				} else {
					ResizeSubWidget(decoration_, w, h);
					ResizeSubWidget(content_, w, 0);
				}

				y = y - decoration_->size().height();
				SetSubWidgetPosition(decoration_, x, y);
				y -= space_;
				y = y - content_->size().height();
				SetSubWidgetPosition(content_, x, y);

			} else {

				if (h > dec_prefer.height()) {
					ResizeSubWidget(decoration_, w, dec_prefer.height());
				} else {
					ResizeSubWidget(decoration_, w, h);
				}

				y = y - decoration_->size().height();
				SetSubWidgetPosition(decoration_, x, y);
			}

		} else {

			if (content_) {

				ResizeSubWidget(content_, w, h);
				SetSubWidgetPosition(content_, x, y);
				DBG_PRINT_MSG("content size: %d %d", content_->size().width(), content_->size().height());

			}

		}

	}

}

