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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Expander.hpp>

#include <BlendInt/Gui/ToggleButton.hpp>

#include <BlendInt/Gui/BinLayout.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Icons.hpp>

namespace BlendInt {

	using Stock::Shaders;
	using Stock::Icons;

	ExpandButton::ExpandButton()
	: AbstractButton()
	{
		InitializeExpandButton();
	}

	ExpandButton::ExpandButton (const String& text)
	: AbstractButton()
	{
		InitializeExpandButton(text);
	}

	ExpandButton::~ExpandButton ()
	{
	}

	bool ExpandButton::IsExpandX () const
	{
		return true;
	}

	Size ExpandButton::GetPreferredSize () const
	{
		int h = font().GetHeight();

		Size prefer(h + round_radius() * 2 + default_padding.hsum() + 100,
						h + default_padding.vsum());

		return prefer;
	}

	void ExpandButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(*request.size(), round_type(),
			        round_radius(), text());

			set_size(*request.size());
			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ExpandButton::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(size(), *request.round_type(), round_radius(),
			        text());

			set_round_type(*request.round_type());

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void ExpandButton::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(size(), round_type(), *request.round_radius(),
			        text());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType ExpandButton::Draw (Profile& profile)
	{
		if(text().size()) {
			font().Print(0.f, 0.f, text(), text_length(), 0);
		}

		float rotate = 0.f;
		if(is_checked()) {
			rotate = 0.f;
		} else {
			rotate = -90.f;
		}

		glm::vec2 pos(0.f, 0.f);
		pos.x += Icons::instance->num()->size().width()/2.f;
		pos.y += size().height()/2.f;

		Icons::instance->num()->Draw(pos, rotate, 1.5f, Color(0x0F0F0FFF));

		return Accept;
	}

	void ExpandButton::InitializeExpandButton ()
	{
		set_checkable(true);
		int h = font().GetHeight();
		set_size(h + round_radius() * 2 + default_padding.hsum(),
						h + default_padding.vsum());
	}

	void ExpandButton::InitializeExpandButton (const String& text)
	{
		set_checkable(true);
		set_text(text);

		int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_radius() * 2 + default_padding.hsum(),
							h + default_padding.vsum());
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			int width = text_outline.width() + round_radius() * 2 + default_padding.hsum();
			int height = h + default_padding.vsum();

			set_size(width, height);

			set_pen((width - text_outline.width()) / 2,
							(height - font().GetHeight()) / 2
											+ std::abs(font().GetDescender()));
		}
	}

	// ----------------------

	Expander::Expander ()
	: Widget(), frame_height_(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton);
		BinLayout* frame = Manage(new BinLayout);

		PushBackSubWidget(title_button);	// 0
		PushBackSubWidget(frame);	// 1

		int width = 0;
		int height = 0;

		Size tmp = title_button->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		tmp = frame->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		set_size(width, height);

		FillInExpander(size());
		frame_height_ = frame->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);
	}

	Expander::Expander (const String& title)
	: Widget(), frame_height_(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton(title));
		BinLayout* frame = Manage(new BinLayout);

		PushBackSubWidget(title_button);	// 0
		PushBackSubWidget(frame);	// 1

		int width = 0;
		int height = 0;

		Size tmp = title_button->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		tmp = frame->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		set_size(width, height);

		FillInExpander(size());
		frame_height_ = frame->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);
	}

	Expander::~Expander ()
	{
	}

	bool Expander::Setup (Widget* widget)
	{
		BinLayout* frame = dynamic_cast<BinLayout*>(GetWidgetAt(1));
		if(frame->Setup(widget)) {
			return true;
		}

		return false;
	}

	Size Expander::GetPreferredSize() const
	{
		Size prefer;

		Size tmp;
		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			tmp = p->GetPreferredSize();
			prefer.set_width(std::max(prefer.width(), tmp.width()));
			prefer.add_height(tmp.height());
		}

		return prefer;
	}

	bool Expander::IsExpandX () const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next())
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

		for(AbstractWidget* p = first_child(); p; p = p->next())
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
			FillInExpander(*request.size());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	ResponseType Expander::Draw (Profile& profile)
	{
		return subs_count() ? Ignore : Accept;
	}

	void Expander::FillInExpander (const Size& out_size)
	{
		int x = 0;
		int y = 0;
		int w = out_size.width();
		int h = out_size.height();

		if(w >= 0 && h >= 0)
			FillInExpander(x, y, w, h);
	}
	
	void Expander::FillInExpander (int x, int y, int width,
					int height)
	{
		int button_preferred_height = 0;
		ExpandButton* button = dynamic_cast<ExpandButton*>(GetWidgetAt(0));
		BinLayout* frame = dynamic_cast<BinLayout*>(GetWidgetAt(1));

		button_preferred_height = button->GetPreferredSize().height();

		if(frame->visiable()) {

			if(button_preferred_height < height) {

				y = y + height;
				ResizeSubWidget(button, width, button_preferred_height);
				y -= button_preferred_height;
				SetSubWidgetPosition(button, x, y);

				ResizeSubWidget(frame, width, height - button_preferred_height);
				y -= frame->size().height();

				SetSubWidgetPosition(frame, x, y);

			} else {

				ResizeSubWidget(button, width, height);
				SetSubWidgetPosition(button, x, y);
				ResizeSubWidget(frame, width, 0);
				SetSubWidgetPosition(frame, x, y);
			}

		} else {
			ResizeSubWidget(button, width, height);
			SetSubWidgetPosition(button, x, y);
		}
	}
	
	void Expander::SetTitle (const String& text)
	{
		dynamic_cast<ExpandButton*>(GetWidgetAt(0))->SetText(text);
	}

	const String& Expander::GetTitle () const
	{
		ExpandButton* button = dynamic_cast<ExpandButton*>(GetWidgetAt(0));

		return button->text();
	}
	
	void Expander::OnToggled (bool toggle)
	{
		ExpandButton* button = dynamic_cast<ExpandButton*>(GetWidgetAt(0));
		BinLayout* frame = dynamic_cast<BinLayout*>(GetWidgetAt(1));

		if(toggle) {
			int x = position().x();
			int y = position().y() + size().height();
			frame->SetVisible(false);
			frame_height_ = frame->size().height();
			Resize(size().width(), button->size().height());
			y = y - size().height();
			MoveTo(x, y);
		} else {
			int x = position().x();
			int y = position().y() + size().height();

			frame->SetVisible(true);

			Resize(size().width(),
							button->size().height() + frame_height_);
			y = y - size().height();
			MoveTo(x, y);
		}

		Refresh();
	}

}
