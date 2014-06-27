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

		Size prefer(h + round_corner_radius() * 2 + DefaultButtonPadding().hsum() + 100,
						h + DefaultButtonPadding().vsum());

		return prefer;
	}

	void ExpandButton::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if (request.target() == this) {
			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
					        static_cast<const Size*>(request.data());
					UpdateTextPosition(*size_p, round_corner_type(),
					        round_corner_radius(), text());

					set_size(*size_p);
					Refresh();
					break;
				}

				case WidgetRoundCornerType: {
					const int* type_p = static_cast<const int*>(request.data());
					UpdateTextPosition(size(), *type_p, round_corner_radius(),
					        text());

					set_round_corner_type(*type_p);
					Refresh();
					break;
				}

				case WidgetRoundCornerRadius: {
					const float* radius_p =
					        static_cast<const float*>(request.data());
					UpdateTextPosition(size(), round_corner_type(), *radius_p,
					        text());

					set_round_corner_radius(*radius_p);
					Refresh();
					break;
				}

				default:
					break;
			}
		}

		ReportGeometryUpdate(request);
	}

	ResponseType ExpandButton::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		if(text().size()) {
			font().Print(mvp, text(), text_length(), 0);
		}

		RefPtr<VertexIcon> icon = Stock::Icons::instance->icon_num();

		glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(1.5f, 1.5f, 1.5f));
		glm::mat4 rotate;
		if(checked()) {
			rotate = glm::rotate(glm::mat4(1.f), (glm::mediump_float)(M_PI * 0.f), glm::vec3(0.0, 0.0, 1.0));
		} else {
			rotate = glm::rotate(glm::mat4(1.f), (glm::mediump_float)(M_PI * 1.5f), glm::vec3(0.0, 0.0, 1.0));
		}
		glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(icon->size().width()/2.f, size().height()/2.f, 0.0));

		icon->Draw(mvp * translate * rotate * scale, Color(0x0F0F0FFF));

		return Accept;
	}

	void ExpandButton::InitializeExpandButton ()
	{
		set_checkable(true);

		int h = font().GetHeight();

		set_size(h + round_corner_radius() * 2 + DefaultButtonPadding().hsum(),
						h + DefaultButtonPadding().vsum());
	}

	void ExpandButton::InitializeExpandButton (const String& text)
	{
		set_checkable(true);
		set_text(text);

		int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_corner_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right(),
							h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom());
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			int width = text_outline.width() + round_corner_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right();
			int height = h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom();

			set_size(width, height);

			set_pen((width - text_outline.width()) / 2,
							(height - font().GetHeight()) / 2
											+ std::abs(font().GetDescender()));
		}
	}

	// ----------------------

	Expander::Expander ()
	: AbstractVectorContainer(2), m_vao(0), m_frame_height(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton);
		Frame* frame = Manage(new Frame);

		SetSubWidget(0, title_button);
		SetSubWidget(1, frame);

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
		m_frame_height = frame->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);

		InitializeExpander();
	}

	Expander::Expander (const String& title)
	: AbstractVectorContainer(2), m_vao(0), m_frame_height(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton(title));
		Frame* frame = Manage(new Frame);

		SetSubWidget(0, title_button);
		SetSubWidget(1, frame);

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
		m_frame_height = frame->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);

		InitializeExpander();
	}

	Expander::~Expander ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool Expander::Setup (AbstractWidget* widget)
	{
		Frame* frame = dynamic_cast<Frame*>(sub_widget(1));
		if(frame->Setup(widget)) {
			return true;
		}

		return false;
	}

	Size Expander::GetPreferredSize() const
	{
		Size prefer;

		Size tmp;
		for(WidgetVector::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			tmp = (*it)->GetPreferredSize();
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

		for(WidgetVector::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool Expander::IsExpandY () const
	{
		bool expand = false;

		for(WidgetVector::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	void Expander::UpdateContainer(const ContainerUpdateRequest& request)
	{
		if (request.target() == this) {
			switch (request.type()) {

				case ContainerMargin: {

					const Margin* margin_p =
					        static_cast<const Margin*>(request.data());
					set_margin(*margin_p);
					FillInExpander(position(), size(), *margin_p);

					break;
				}

				default:
					break;
			}
		}

		ReportContainerUpdate(request);
	}

	bool Expander::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		/*
		if(request.source() == this) {

			return AbstractVectorContainer::UpdateGeometryTest(request);

		} else {	// called by sub widget

			switch(request.type()) {
				case WidgetSize:
					return false;

				case WidgetPosition:
					return false;

				default:
					return false;
			}
		}
		*/

		// Allow container to resize this
		return true;
	}

	void Expander::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
									static_cast<const Size*>(request.data());
					FillInExpander(position(), *size_p, margin());

					VertexTool tool;
					tool.Setup(*size_p, 0, RoundNone, 0);
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());

					set_size(*size_p);
					Refresh();
					break;
				}

				case WidgetPosition: {
					const Point* pos_p =
									static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					set_position(*pos_p);
					MoveSubWidgets(x, y);
					break;
				}

				default:
					break;
			}

		}

		ReportGeometryUpdate(request);
	}

	ResponseType Expander::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);
		glBindVertexArray(0);

		program->Reset();

		return AcceptAndContinue;
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
		//int sum = 0;
		ExpandButton* button = dynamic_cast<ExpandButton*>(sub_widget(0));
		Frame* frame = dynamic_cast<Frame*>(sub_widget(1));

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
		dynamic_cast<ExpandButton*>(sub_widget(0))->SetText(text);
	}

	const String& Expander::GetTitle () const
	{
		ExpandButton* button = dynamic_cast<ExpandButton*>(sub_widget(0));

		return button->text();
	}
	
	void Expander::InitializeExpander ()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		glBindVertexArray(m_vao);
		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

	void Expander::OnToggled (bool toggle)
	{
		ExpandButton* button = dynamic_cast<ExpandButton*>(sub_widget(0));
		Frame* frame = dynamic_cast<Frame*>(sub_widget(1));

		if(toggle) {
			int x = position().x();
			int y = position().y() + size().height();
			frame->SetVisible(false);
			m_frame_height = frame->size().height();
			Resize(size().width(), button->size().height() + margin().vsum());
			y = y - size().height();
			SetPosition(x, y);
		} else {
			int x = position().x();
			int y = position().y() + size().height();

			frame->SetVisible(true);

			Resize(size().width(),
							button->size().height() + m_frame_height + margin().vsum());
			y = y - size().height();
			SetPosition(x, y);
		}

		Refresh();
	}

}
