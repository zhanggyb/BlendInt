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
	: decoration_(0),
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

		if(InsertSubWidget(DecorationIndex, widget)) {
			decoration_ = widget;
			FillSubWidgets(position(), size(), margin());
		}
	}

	void AbstractPanel::SetContent (AbstractWidget* widget)
	{
		if(widget == 0) return;

		if(widget == content_) return;	// already in this panel

		int sum = widget_count();

		if (sum > 1) {
			DBG_PRINT_MSG("TODO: %s", "delete tail widgets");

			AbstractWidget* tmp = 0;
			for(AbstractWidget* p = first()->next(); p; p = tmp)
			{
				tmp = p->next();
				if(p->managed() && (p->reference_count() == 0))
				{
					delete p;
				} else {
					DBG_PRINT_MSG("Warning: %s is not set managed and will not be deleted", p->name().c_str());
				}
			}
		}

		if(InsertSubWidget(ContentIndex, widget)) {
			content_ = widget;
			FillSubWidgets(position(), size(), margin());
		}
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
		AbstractWidget* dec = GetWidgetAt(DecorationIndex);
		AbstractWidget* content = GetWidgetAt(ContentIndex);

		Size dec_prefer(0, 0);

		if (dec) {
			dec_prefer = dec->GetPreferredSize();
		}

		y = y + h;
		int space = dec ? space_ : 0;

		if (content) {

			if (h > dec_prefer.height()) {
				ResizeSubWidget(dec, w, dec_prefer.height());
				ResizeSubWidget(content, w, h - dec_prefer.height() - space);
			} else {
				ResizeSubWidget(dec, w, h);
				ResizeSubWidget(content, w, 0);
			}

			y = y - dec->size().height();
			SetSubWidgetPosition(dec, x, y);
			y -= space;
			y = y - content->size().height();
			SetSubWidgetPosition(content, x, y);

		} else {

			if (h > dec_prefer.height()) {
				ResizeSubWidget(dec, w, dec_prefer.height());
			} else {
				ResizeSubWidget(dec, w, h);
			}

			y = y - dec->size().height();
			SetSubWidgetPosition(dec, x, y);
		}
	}

}
