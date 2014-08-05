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

#include <BlendInt/Gui/FileSystemModel.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Margin ComboBox::default_combobox_padding = Margin(2, 2, 2, 2);

	ComboBox::ComboBox ()
	: AbstractWidget(),
	  status_down_(false),
	  list_(0)
	{
		set_round_type(RoundAll);

		int h = font_.GetHeight();

		set_size(
		        h + round_radius() * 2 + default_combobox_padding.hsum() + 100,
		        h + default_combobox_padding.vsum());

		InitializeComboBox();
	}

	ComboBox::~ComboBox ()
	{
		glDeleteVertexArrays(2, vaos_);
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

		int max_font_height = font_.GetHeight();

		preferred_size.set_height(max_font_height + default_combobox_padding.vsum());	// top padding: 2, bottom padding: 2

		if (text_.empty()) {
			preferred_size.set_width(
							max_font_height + default_combobox_padding.hsum()
											+ radius_plus + 100);
		} else {
			size_t width = font_.GetTextWidth(text_);
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

	void ComboBox::SetModel (const RefPtr<AbstractItemModel>& model)
	{
		model_ = model;
		if(list_) {
			list_->SetModel(model_);
		}
	}

	void ComboBox::PerformSizeUpdate (const SizeUpdateRequest& request)
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
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void ComboBox::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
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
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			set_round_type(*request.round_type());
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void ComboBox::PerformRoundRadiusUpdate (
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
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType ComboBox::Draw(const RedrawEvent& event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), 0.f);

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3fv(Shaders::instance->triangle_uniform_position(), 1, glm::value_ptr(pos));
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		if(status_down_) {
			glUniform1i(Shaders::instance->triangle_uniform_gamma(), 20);
		} else {
			if(hover()) {
				glUniform1i(Shaders::instance->triangle_uniform_gamma(), 15);
			} else {
				glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
			}
		}

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(), Theme::instance->menu().outline.data());
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		program->Reset();

		RefPtr<VertexIcon> icon = Stock::Icons::instance->icon_menu();

		//glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(1.15, 1.15, 1.15));
		//glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (glm::mediump_float)(M_PI * 1.5), glm::vec3(0.0, 0.0, 1.0));
		//glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(icon->size().width()/2.f, icon->size().height()/2.f, 0.0));

		//icon->Draw(mvp * translate * rotate * scale);

		pos.x += size().width() - icon->size().width()/2.f;
		pos.y += size().height()/2.f;

		icon->Draw(pos, 0, 1.f, Color(0xEFEFEFFF));

		return Accept;
	}
	
	ResponseType ComboBox::MousePressEvent (const MouseEvent& event)
	{
		status_down_ = true;

		Context* context = event.context();

		if(list_) {
			context->Remove(list_);
			delete list_;
			list_ = 0;
			SetRoundCornerType(RoundAll);
		} else {
			list_ = Manage(new ListView);

			if(model_) {
				list_->SetModel(model_);
			}

			list_->Resize(200, list_->size().height());
			list_->SetPosition(position().x(), position().y() + size().height());
			context->PushBack(list_);
			SetRoundCornerType(RoundBottomLeft | RoundBottomRight);
			context->SetFocusedWidget(list_);	// FIXME: if not set the menu focused, it will cause segment fault after click the menu several times.
		}

		Refresh();

		return Accept;
	}
	
	ResponseType ComboBox::MouseReleaseEvent (const MouseEvent& event)
	{
		status_down_ = false;

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
		glGenVertexArrays(2, vaos_);

		VertexTool tool;
		tool.Setup(size(),
						DefaultBorderWidth(),
						round_type(),
						round_radius(),
						Theme::instance->menu().inner,
						Vertical,
						Theme::instance->menu().shadetop,
						Theme::instance->menu().shadedown);

		glBindVertexArray(vaos_[0]);
		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		tool.SetInnerBufferData(inner_.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_color());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->triangle_attrib_color(), 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->Generate();
		outer_->Bind();
		tool.SetOuterBufferData(outer_.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,	GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::Reset();
		glBindVertexArray(0);
	}

	void ComboBox::OnMenuActionTriggered (Action* item)
	{
	}

}
