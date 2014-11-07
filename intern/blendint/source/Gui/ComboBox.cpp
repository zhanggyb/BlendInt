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
#include <BlendInt/Gui/Dialog.hpp>

#include <BlendInt/Gui/FileSystemModel.hpp>

namespace BlendInt {

	using Stock::Shaders;
	using Stock::Icons;

	Margin ComboBox::default_combobox_padding = Margin(2, 2, 2, 2);

	ComboBox::ComboBox ()
	: Widget(),
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

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Theme::instance->menu().shaded) {
				GenerateVertices(Vertical,
						Theme::instance->menu().shadetop,
						Theme::instance->menu().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateVertices(&inner_verts, &outer_verts);
			}

			inner_->bind();
			inner_->set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			outer_->bind();
			outer_->set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ComboBox::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_round_type(*request.round_type());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Theme::instance->menu().shaded) {
				GenerateVertices(Vertical,
						Theme::instance->menu().shadetop,
						Theme::instance->menu().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateVertices(&inner_verts, &outer_verts);
			}

			inner_->bind();
			inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			outer_->bind();
			outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void ComboBox::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {

			set_round_radius(*request.round_radius());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Theme::instance->menu().shaded) {
				GenerateVertices(Vertical,
						Theme::instance->menu().shadetop,
						Theme::instance->menu().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateVertices(&inner_verts, &outer_verts);
			}

			inner_->bind();
			inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			outer_->bind();
			outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType ComboBox::Draw(Profile& profile)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				Theme::instance->menu().inner.data());

		if (status_down_) {
			glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 20);
		} else {
			if (hover()) {
				glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA),
						15);
			} else {
				glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA),
						0);
			}
		}

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		Shaders::instance->widget_outer_program()->use();

		glUniform4fv(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1,
				Theme::instance->menu().outline.data());
		glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION), 0.f, 0.f);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
				GetOutlineVertices(round_type()) * 2 + 2);

//		if (emboss()) {
//			glUniform4f(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1.0f,
//			        1.0f, 1.0f, 0.16f);
//			glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
//			        0.f, - 1.f);
//			glDrawArrays(GL_TRIANGLE_STRIP, 0,
//			        GetHalfOutlineVertices(round_type()) * 2);
//		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		//RefPtr<VertexIcon> icon = Icons::instance->icon_menu();

		//glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(1.15, 1.15, 1.15));
		//glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (glm::mediump_float)(M_PI * 1.5), glm::vec3(0.0, 0.0, 1.0));
		//glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(icon->size().width()/2.f, icon->size().height()/2.f, 0.0));

		//icon->Draw(mvp * translate * rotate * scale);

		float x = size().width() - Icons::instance->menu()->size().width()/2.f;
		float y = size().height()/2.f;

		Icons::instance->menu()->Draw(x, y, Color(0xEFEFEFFF));

		return Accept;
	}
	
	ResponseType ComboBox::MousePressEvent (const MouseEvent& event)
	{
		status_down_ = true;

		Context* context = event.context();

		if(list_) {
			AbstractWidget* parent = list_->parent();
			delete parent;
			list_ = 0;
			SetRoundType(RoundAll);
		} else {
			list_ = Manage(new ListView);

			if(model_) {
				list_->SetModel(model_);
			}

			list_->Resize(200, list_->size().height());
			list_->SetPosition(position().x(), position().y() + size().height());

			Dialog* screen = Manage(new Dialog);
			screen->Resize(list_->size());
			screen->AddWidget(list_);

			Point pos = GetGlobalPosition();

			screen->SetPosition(pos.x(), pos.y() + size().height());
			context->AddFrame(screen);
			SetRoundType(RoundBottomLeft | RoundBottomRight);
			//context->SetFocusedWidget(list_);	// FIXME: if not set the menu focused, it will cause segment fault after click the menu several times.
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
	
	void ComboBox::MouseHoverInEvent(const MouseEvent& event)
	{
		Refresh();
	}

	void ComboBox::MouseHoverOutEvent(const MouseEvent& event)
	{
		Refresh();
	}

	void ComboBox::InitializeComboBox()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->menu().shaded) {
			GenerateVertices(Vertical,
					Theme::instance->menu().shadetop,
					Theme::instance->menu().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);
		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(
				Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		glEnableVertexAttribArray(
				Shaders::instance->location (Stock::WIDGET_OUTER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_OUTER_COORD),
				2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void ComboBox::OnMenuActionTriggered (Action* item)
	{
	}

}