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

#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Icons.hpp>

namespace BlendInt {

	using Stock::Shaders;

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

		Size prefer(h + round_radius() * 2 + DefaultButtonPadding().hsum() + 100,
						h + DefaultButtonPadding().vsum());

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

		ReportSizeUpdate(request);
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

		ReportRoundTypeUpdate(request);
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

		ReportRoundRadiusUpdate(request);
	}

	ResponseType ExpandButton::Draw (const RedrawEvent& event)
	{
		if(text().size()) {
			font().Print(position(), text(), text_length(), 0);
		}

		RefPtr<VertexIcon> icon = Stock::Icons::instance->icon_num();

		float rotate = 0.f;
		if(checked()) {
			rotate = 0.f;
		} else {
			rotate = -90.f;
		}

		glm::vec3 pos((float)position().x(), (float)position().y(), 0.f);
		pos.x += icon->size().width()/2.f;
		pos.y += size().height()/2.f;

		icon->Draw(pos, rotate, 1.5f, Color(0x0F0F0FFF));

		return Accept;
	}

	void ExpandButton::InitializeExpandButton ()
	{
		set_checkable(true);
		int h = font().GetHeight();
		set_size(h + round_radius() * 2 + DefaultButtonPadding().hsum(),
						h + DefaultButtonPadding().vsum());
	}

	void ExpandButton::InitializeExpandButton (const String& text)
	{
		set_checkable(true);
		set_text(text);

		int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right(),
							h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom());
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			int width = text_outline.width() + round_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right();
			int height = h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom();

			set_size(width, height);

			set_pen((width - text_outline.width()) / 2,
							(height - font().GetHeight()) / 2
											+ std::abs(font().GetDescender()));
		}
	}

	// ----------------------

	Expander::Expander ()
	: AbstractContainer(2), vao_(0), frame_height_(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton);
		Frame* frame = Manage(new Frame);

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

		set_margin(2, 2, 2, 2);

		width = width + margin().hsum();
		height = height + margin().vsum();
		set_size(width, height);

		FillInExpander(position(), size(), margin());
		frame_height_ = frame->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);

		InitializeExpander();
	}

	Expander::Expander (const String& title)
	: AbstractContainer(2), vao_(0), frame_height_(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton(title));
		Frame* frame = Manage(new Frame);

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

		width = width + margin().left() + margin().right();
		height = height + margin().top() + margin().bottom();

		set_size(width, height);
		set_margin(2, 2, 2, 2);

		FillInExpander(position(), size(), margin());
		frame_height_ = frame->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);

		InitializeExpander();
	}

	Expander::~Expander ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	bool Expander::Setup (AbstractWidget* widget)
	{
		Frame* frame = dynamic_cast<Frame*>(GetWidgetAt(1));
		if(frame->Setup(widget)) {
			return true;
		}

		return false;
	}

	Size Expander::GetPreferredSize() const
	{
		Size prefer;

		Size tmp;
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			tmp = p->GetPreferredSize();
			prefer.set_width(std::max(prefer.width(), tmp.width()));
			prefer.add_height(tmp.height());
		}

		prefer.add_width(margin().hsum());
		prefer.add_height(margin().vsum());

		return prefer;
	}

	bool Expander::IsExpandX () const
	{
		bool expand = false;

		for(AbstractWidget* p = first(); p; p = p->next())
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

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	void Expander::PerformMarginUpdate(const Margin& request)
	{
		FillInExpander(position(), size(), request);
	}

	void Expander::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			FillInExpander(position(), *request.size(), margin());

			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void Expander::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		ReportPositionUpdate(request);
	}

	ResponseType Expander::Draw (const RedrawEvent& event)
	{
		RefPtr<GLSLProgram> program =
				Shaders::instance->triangle_program();
		program->Use();

		program->SetUniform3f(Shaders::instance->triangle_uniform_position(),
				(float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		program->SetUniform1i(Shaders::instance->triangle_uniform_antialias(),
				0);

		program->SetVertexAttrib4f(Shaders::instance->triangle_attrib_color(),
				0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);

		program->Reset();

		return Ignore;
	}

	ResponseType Expander::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Expander::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}
	
	void Expander::FillInExpander (const Point& out_pos,
					const Size& out_size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		if(w >= 0 && h >= 0)
			FillInExpander(x, y, w, h);
	}
	
	void Expander::FillInExpander (int x, int y, int width,
					int height)
	{
		int button_preferred_height = 0;
		ExpandButton* button = dynamic_cast<ExpandButton*>(GetWidgetAt(0));
		Frame* frame = dynamic_cast<Frame*>(GetWidgetAt(1));

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
	
	void Expander::InitializeExpander ()
	{
		glGenVertexArrays(1, &vao_);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		glBindVertexArray(vao_);
		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		tool.SetInnerBufferData(inner_.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

	void Expander::OnToggled (bool toggle)
	{
		ExpandButton* button = dynamic_cast<ExpandButton*>(GetWidgetAt(0));
		Frame* frame = dynamic_cast<Frame*>(GetWidgetAt(1));

		if(toggle) {
			int x = position().x();
			int y = position().y() + size().height();
			frame->SetVisible(false);
			frame_height_ = frame->size().height();
			Resize(size().width(), button->size().height() + margin().vsum());
			y = y - size().height();
			SetPosition(x, y);
		} else {
			int x = position().x();
			int y = position().y() + size().height();

			frame->SetVisible(true);

			Resize(size().width(),
							button->size().height() + frame_height_ + margin().vsum());
			y = y - size().height();
			SetPosition(x, y);
		}

		Refresh();
	}

}
