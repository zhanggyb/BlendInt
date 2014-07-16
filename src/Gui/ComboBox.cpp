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

#include <math.h>

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

#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Icons.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Margin ComboBox::default_combobox_padding = Margin(2, 2, 2, 2);

	ComboBox::ComboBox ()
	: AbstractWidget(),
	  m_status_down(false)
	{
		set_round_type(RoundAll);

		int h = m_font.GetHeight();

		set_size(
		        h + round_radius() * 2 + default_combobox_padding.hsum() + 100,
		        h + default_combobox_padding.vsum());

		InitializeComboBox();
	}

	ComboBox::~ComboBox ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	Size ComboBox::GetPreferredSize () const
	{
		Size preferred_size;

		int radius_plus = 0;

		if((round_type() & RoundTopLeft) || (round_type() & RoundBottomLeft)) {
			radius_plus += round_radius();
		}

		if((round_type() & RoundTopRight) || (round_type() & RoundBottomRight)) {
			radius_plus += round_radius();
		}

		int max_font_height = m_font.GetHeight();

		preferred_size.set_height(max_font_height + default_combobox_padding.vsum());	// top padding: 2, bottom padding: 2

		if (m_text.empty()) {
			preferred_size.set_width(
							max_font_height + default_combobox_padding.hsum()
											+ radius_plus + 100);
		} else {
			size_t width = m_font.GetTextWidth(m_text);
			preferred_size.set_width(width
							+ default_combobox_padding.hsum()
							+ radius_plus);	// left padding: 2, right padding: 2
		}

		return preferred_size;
	}

	bool ComboBox::IsExpandX() const
	{
		return true;
	}

	void ComboBox::ProcessSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(),
							DefaultBorderWidth(),
							round_type(),
							round_radius(),
							Theme::instance->menu().inner,
							Vertical,
							Theme::instance->menu().shadetop,
							Theme::instance->menu().shadedown);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());
			m_outer->Bind();
			tool.SetOuterBufferData(m_outer.get());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void ComboBox::ProcessRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(size(),
							DefaultBorderWidth(),
							*request.round_type(),
							round_radius(),
							Theme::instance->menu().inner,
							Vertical,
							Theme::instance->menu().shadetop,
							Theme::instance->menu().shadedown);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());
			m_outer->Bind();
			tool.SetOuterBufferData(m_outer.get());

			set_round_type(*request.round_type());
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void ComboBox::ProcessRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(size(),
							DefaultBorderWidth(),
							round_type(),
							*request.round_radius(),
							Theme::instance->menu().inner,
							Vertical,
							Theme::instance->menu().shadetop,
							Theme::instance->menu().shadedown);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());
			m_outer->Bind();
			tool.SetOuterBufferData(m_outer.get());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType ComboBox::Draw(const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_AA", 0);

		if(m_status_down) {
			program->SetUniform1i("u_gamma", 20);
		} else {
			if(hover()) {
				program->SetUniform1i("u_gamma", 15);
			} else {
				program->SetUniform1i("u_gamma", 0);
			}
		}

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		program->SetVertexAttrib4fv("a_color", Theme::instance->menu().outline.data());
		program->SetUniform1i("u_AA", 1);
		program->SetUniform1i("u_gamma", 0);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		program->Reset();

		RefPtr<VertexIcon> icon = Stock::Icons::instance->icon_num();

		//glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(1.15, 1.15, 1.15));
		//glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (glm::mediump_float)(M_PI * 1.5), glm::vec3(0.0, 0.0, 1.0));
		//glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(icon->size().width()/2.f, icon->size().height()/2.f, 0.0));

		//icon->Draw(mvp * translate * rotate * scale);

		return Accept;
	}
	
	ResponseType ComboBox::MousePressEvent (const MouseEvent& event)
	{
		m_status_down = true;

		Context* context = event.context();
		if(context) {
			if(m_menu->container()) {
				context->Remove(m_menu.get());
				SetRoundCornerType(RoundAll);
			} else {
				//int max_layer = context->GetMaxLayer();
				//m_menu->SetLayer(max_layer + 1);
				m_menu->SetPosition(position().x(), position().y() + size().height());
				context->PushBack(m_menu.get());
				SetRoundCornerType(RoundBottomLeft | RoundBottomRight);
				//context->SetFocusedWidget(m_menu.get());	// FIXME: no use, context will reset to this combobox.
			}

			Refresh();
		}

		return Accept;
	}
	
	ResponseType ComboBox::MouseReleaseEvent (const MouseEvent& event)
	{
		m_status_down = false;

		Refresh();
		return Accept;
	}
	
	ResponseType ComboBox::CursorEnterEvent (bool entered)
	{
		Refresh();
		return Accept;
	}

	ResponseType ComboBox::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ComboBox::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ComboBox::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ComboBox::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ComboBox::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void ComboBox::InitializeComboBox()
	{
		glGenVertexArrays(2, m_vao);

		VertexTool tool;
		tool.Setup(size(),
						DefaultBorderWidth(),
						round_type(),
						round_radius(),
						Theme::instance->menu().inner,
						Vertical,
						Theme::instance->menu().shadetop,
						Theme::instance->menu().shadedown);

		glBindVertexArray(m_vao[0]);
		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(m_vao[1]);
		m_outer.reset(new GLArrayBuffer);
		m_outer->Generate();
		m_outer->Bind();
		tool.SetOuterBufferData(m_outer.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::Reset();
		glBindVertexArray(0);

		// Now create menu

		m_menu.reset(new Menu);

		m_menu->SetRoundCornerType(RoundTopLeft | RoundTopRight);
		//m_menu->SetPosition(200, 200);
		//menu->Resize (200, 200);

		m_menu->AddAction(Stock::Icons::instance->icon_check(), "MenuItem1", "Ctrl + 1");
		m_menu->AddAction("MenuItem2", "Ctrl + 1");
		m_menu->AddAction("MenuItem3", "Ctrl + 1");
		m_menu->AddAction("MenuItem4", "Ctrl + 1");
		m_menu->AddAction("MenuItem5");

		events()->connect(m_menu->triggered(), this, &ComboBox::OnMenuActionTriggered);
	}

	void ComboBox::OnMenuActionTriggered (Action* item)
	{
		Context* context = Context::GetContext(this);

		context->Remove(m_menu.get());
		SetRoundCornerType(RoundAll);

		Refresh();
	}

}
