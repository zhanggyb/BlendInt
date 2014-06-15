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

	void ExpandButton::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				UpdateTextPosition(*size_p, round_corner_type(),
								round_corner_radius(), text());
				Refresh();
				break;
			}

			case WidgetRoundCornerType: {
				const int* type_p = static_cast<const int*>(request.data());
				UpdateTextPosition(size(), *type_p, round_corner_radius(),
								text());
				Refresh();
				break;
			}

			case WidgetRoundCornerRadius: {
				const int* radius_p =
								static_cast<const int*>(request.data());
				UpdateTextPosition(size(), round_corner_type(), *radius_p,
								text());
				Refresh();
				break;
			}

			default:
				break;
		}

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

	SingleBox::SingleBox ()
	: AbstractSingleContainer()
	{
		set_size(400, 300);
	}

	SingleBox::~SingleBox ()
	{
	}

	bool SingleBox::Setup (AbstractWidget* widget)
	{
		bool ret = false;

		if (SetSubWidget(widget)) {

			int x = position().x() + margin().left();
			int y = position().y() + margin().bottom();

			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			FillSubWidget(x, y, w, h);

			ret = true;
		}

		return ret;
	}

	bool SingleBox::Remove (AbstractWidget* widget)
	{
		bool ret = false;

		if(RemoveSubWidget(widget)) {

			ret = true;
		}

		return ret;
	}

	bool SingleBox::IsExpandX() const
	{
		if(sub_widget())
			return sub_widget()->IsExpandX();
		else
			return false;
	}

	bool SingleBox::IsExpandY() const
	{
		if(sub_widget())
			return sub_widget()->IsExpandY();
		else
			return false;
	}

	Size SingleBox::GetPreferredSize() const
	{
		Size prefer(400, 300);

		if(sub_widget()) {
			prefer = sub_widget()->GetPreferredSize();

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void SingleBox::UpdateContainer (const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerMargin: {

				if (sub_widget()) {
					const Margin* margin_p =
									static_cast<const Margin*>(request.data());
					set_margin(*margin_p);

					FillSubWidget(position(), size(), *margin_p);
				}
				break;
			}

			case ContainerRefresh: {
				Refresh();
				break;
			}

			default:
				break;

		}
	}

	bool SingleBox::UpdateGeometryTest (const WidgetUpdateRequest& request)
	{
		if(request.source() == this) {

			return AbstractSingleContainer::UpdateGeometryTest(request);

		} else if (request.source() == container()) {

			return true;

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
	}

	void SingleBox::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(request.source() == this || request.source() == container()) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
									static_cast<const Size*>(request.data());
					if (sub_widget()) {
						FillSubWidget(position(), *size_p, margin());
					}

					break;
				}

				case WidgetPosition: {
					if (sub_widget()) {
						const Point* pos_p =
										static_cast<const Point*>(request.data());
						SetSubWidgetPosition(sub_widget(),
										pos_p->x() + margin().left(),
										pos_p->y() + margin().bottom());
					}
					break;
				}

				default:
					break;
			}

		}
	}

	ResponseType SingleBox::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType SingleBox::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType SingleBox::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType SingleBox::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType SingleBox::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType SingleBox::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType SingleBox::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType SingleBox::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	// ----------------------

	Expander::Expander ()
	: AbstractVectorContainer(2), m_vao(0), m_frame_height(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton);
		SingleBox* box = Manage(new SingleBox);

		SetSubWidget(0, title_button);
		SetSubWidget(1, box);

		int width = 0;
		int height = 0;

		Size tmp = title_button->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		tmp = box->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		width = width + margin().left() + margin().right();
		height = height + margin().top() + margin().bottom();

		set_size(width, height);
		set_margin(2, 2, 2, 2);

		FillInExpander(position(), size(), margin());
		m_frame_height = box->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);

		InitializeExpander();
	}

	Expander::Expander (const String& title)
	: AbstractVectorContainer(2), m_vao(0), m_frame_height(0)
	{
		ExpandButton* title_button = Manage(new ExpandButton(title));
		SingleBox* box = Manage(new SingleBox);

		SetSubWidget(0, title_button);
		SetSubWidget(1, box);

		int width = 0;
		int height = 0;

		Size tmp = title_button->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		tmp = box->GetPreferredSize();
		width = std::max(width, tmp.width());
		height += tmp.height();

		width = width + margin().left() + margin().right();
		height = height + margin().top() + margin().bottom();

		set_size(width, height);
		set_margin(2, 2, 2, 2);

		FillInExpander(position(), size(), margin());
		m_frame_height = box->size().height();

		events()->connect(title_button->toggled(), this, &Expander::OnToggled);

		InitializeExpander();
	}

	Expander::~Expander ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool Expander::Setup (AbstractWidget* widget)
	{
		SingleBox* box = dynamic_cast<SingleBox*>(sub_widget(1));
		if(box->Setup(widget)) {
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

	void Expander::UpdateContainer(const WidgetUpdateRequest& request)
	{
		switch (request.type()) {


			case ContainerMargin: {

				const Margin* margin_p = static_cast<const Margin*>(request.data());
				set_margin(*margin_p);
				FillInExpander(position(), size(), *margin_p);

				break;
			}

			case ContainerRefresh: {
				Refresh();
				break;
			}

			default:
				break;
		}

	}

	bool Expander::UpdateGeometryTest (const WidgetUpdateRequest& request)
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

	void Expander::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p =
								static_cast<const Size*>(request.data());
				FillInExpander(position(), *size_p, margin());

				VertexTool tool;
				tool.Setup(*size_p, 0, RoundNone, 0);
				tool.UpdateInnerBuffer(m_inner.get());
				break;
			}

			case WidgetPosition: {
				const Point* pos_p =
								static_cast<const Point*>(request.data());

				int x = pos_p->x() - position().x();
				int y = pos_p->y() - position().y();

				MoveSubWidgets(x, y);
				break;
			}

			default:
				break;
		}

	}

	ResponseType Expander::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);
		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);

		glEnableVertexAttribArray(0);
		DrawTriangleStrip(0, m_inner.get());
		glDisableVertexAttribArray(0);

		program->Reset();
		glBindVertexArray(0);

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
		SingleBox* box = dynamic_cast<SingleBox*>(sub_widget(1));

		button_preferred_height = button->GetPreferredSize().height();

		if(box->visiable()) {

			if(button_preferred_height < height) {

				y = y + height;
				ResizeSubWidget(button, width, button_preferred_height);
				y -= button_preferred_height;
				SetSubWidgetPosition(button, x, y);

				ResizeSubWidget(box, width, height - button_preferred_height);
				y -= box->size().height();
				SetSubWidgetPosition(box, x, y);

			} else {

				ResizeSubWidget(button, width, height);
				SetSubWidgetPosition(button, x, y);
				ResizeSubWidget(box, width, 0);
				SetSubWidgetPosition(box, x, y);
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

		glBindVertexArray(m_vao);
		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);
		m_inner = tool.GenerateInnerBuffer();
		glBindVertexArray(0);
	}

	void Expander::OnToggled (bool toggle)
	{
		ExpandButton* button = dynamic_cast<ExpandButton*>(sub_widget(0));
		SingleBox* box = dynamic_cast<SingleBox*>(sub_widget(1));

		if(toggle) {
			int x = position().x();
			int y = position().y() + size().height();
			box->SetVisible(false);
			m_frame_height = box->size().height();
			Resize(size().width(), button->size().height() + margin().vsum());
			y = y - size().height();
			SetPosition(x, y);
		} else {
			int x = position().x();
			int y = position().y() + size().height();

			box->SetVisible(true);

			Resize(size().width(),
							button->size().height() + m_frame_height + margin().vsum());
			y = y - size().height();
			SetPosition(x, y);
		}

		Refresh();
	}

}
