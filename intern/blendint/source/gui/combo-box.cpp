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

#include <opengl/opengl.hpp>

#include <gui/dialog.hpp>
#include <gui/filesystem-model.hpp>
#include <gui/abstract-window.hpp>

#include <gui/combo-box.hpp>
#include <gui/menu.hpp>

namespace BlendInt {

	Margin ComboBox::kPadding = Margin(2, 2, 2, 2);

	ComboBox::ComboBox ()
	: AbstractRoundWidget(),
	  status_down_(false),
	  popup_(0)
	{
		set_round_type(RoundAll);

		Font font;	// default font
		int h = font.height();

		set_size(h + pixel_size(kPadding.hsum()),
		        h + pixel_size(kPadding.vsum()));

		InitializeComboBox();
	}

	ComboBox::~ComboBox ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	Size ComboBox::GetPreferredSize () const
	{
		int w = 0;
		int h = 0;

		if(model_) {

		} else {
			Font font;	// default font
			w = font.height();
			h = font.height();
		}

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		return Size(w, h);
	}

	void ComboBox::SetModel (const RefPtr<AbstractItemModel>& model)
	{
		model_ = model;
		//if(list_) {
		//	list_->SetModel(model_);
		//}
	}

	void ComboBox::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (AbstractWindow::theme->menu().shaded) {
				GenerateRoundedVertices(Vertical,
						AbstractWindow::theme->menu().shadetop,
						AbstractWindow::theme->menu().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			vbo_.bind(0);
			vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			vbo_.bind(1);
			vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			vbo_.reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ComboBox::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->menu().shaded) {
			GenerateRoundedVertices(Vertical,
						AbstractWindow::theme->menu().shadetop,
						AbstractWindow::theme->menu().shadedown,
						&inner_verts,
						&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	void ComboBox::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->menu().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->menu().shadetop,
					AbstractWindow::theme->menu().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	Response ComboBox::Draw(AbstractWindow* context)
	{
		AbstractWindow::shaders->widget_inner_program()->use();

		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				AbstractWindow::theme->menu().inner.data());

		if (status_down_) {
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 20);
		} else {
			if (hover()) {
				glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA),
						15);
			} else {
				glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA),
						0);
			}
		}

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1,
				AbstractWindow::theme->menu().outline.data());
		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION), 0.f, 0.f);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
				GetOutlineVertices(round_type()) * 2 + 2);

//		if (emboss()) {
//			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
//			        1.0f, 1.0f, 0.16f);
//			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
//			        0.f, - 1.f);
//			glDrawArrays(GL_TRIANGLE_STRIP, 0,
//			        GetHalfOutlineVertices(round_type()) * 2);
//		}

		//RefPtr<VertexIcon> icon = Icons::instance->icon_menu();

		//glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(1.15, 1.15, 1.15));
		//glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (glm::mediump_float)(M_PI * 1.5), glm::vec3(0.0, 0.0, 1.0));
		//glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(icon->size().width()/2.f, icon->size().height()/2.f, 0.0));

		//icon->Draw(mvp * translate * rotate * scale);

		float x = size().width() - AbstractWindow::icons->menu()->size().width()/2.f;
		float y = size().height()/2.f;

		AbstractWindow::icons->menu()->Draw(x, y, Color(0xEFEFEFFF));

		return Finish;
	}
	
	Response ComboBox::PerformMousePress (AbstractWindow* context)
	{
		status_down_ = true;

		if(popup_) {
			delete popup_;
			popup_ = 0;
			SetRoundType(RoundAll);
            RequestRedraw();
            return Finish;
		} else {
			popup_ = Manage(new Menu);
			popup_->Resize(160, 240);

			events()->connect(popup_->destroyed(), this, &ComboBox::OnPopupListDestroyed);

            Point pos = context->GetAbsolutePosition(this);

            int top = pos.y() + size().height() + popup_->size().height();
            int bottom = pos.y() - popup_->size().height();
            
            if(top <= context->size().height()) {
                popup_->MoveTo(pos.x(), pos.y() + size().height());
                SetRoundType(RoundBottomLeft | RoundBottomRight);
            } else {
                
                if(bottom >= 0) {
                    popup_->MoveTo(pos.x(), pos.y() - popup_->size().height());
                    SetRoundType(RoundTopLeft | RoundTopRight);
                } else {
                    
                    int diff = top - context->size().height() + bottom;
                    if(diff <= 0) {
                        popup_->MoveTo(pos.x(), pos.y() + size().height());
                        SetRoundType(RoundBottomLeft | RoundBottomRight);
                    } else {
                        popup_->MoveTo(pos.x(), pos.y() - popup_->size().height());
                        SetRoundType(RoundTopLeft | RoundTopRight);
                    }
                    
                }
                
            }

            context->AddFrame(popup_);
           
            return Ignore;
		}
		
		/*
		if(popup_) {
			delete popup_;
			popup_ = 0;
			SetRoundType(RoundAll);
		} else {
			ListView* list = Manage(new ListView);

			if(model_) {
				list->SetModel(model_);
			}

			list->Resize(200, list->size().height());

			popup_ = Manage(new PopupFrame);
			popup_->Resize(list->size());
			popup_->AddWidget(list);

			events()->connect(popup_->destroyed(), this, &ComboBox::OnPopupListDestroyed);

			Point pos = context->active_frame()->GetAbsolutePosition(this);

			int top = pos.y() + size().height() + popup_->size().height();
			int bottom = pos.y() - popup_->size().height();

			if(top <= context->size().height()) {
				popup_->MoveTo(pos.x(), pos.y() + size().height());
				SetRoundType(RoundBottomLeft | RoundBottomRight);
			} else {

				if(bottom >= 0) {
					popup_->MoveTo(pos.x(), pos.y() - popup_->size().height());
					SetRoundType(RoundTopLeft | RoundTopRight);
				} else {

					int diff = top - context->size().height() + bottom;
					if(diff <= 0) {
						popup_->MoveTo(pos.x(), pos.y() + size().height());
						SetRoundType(RoundBottomLeft | RoundBottomRight);
					} else {
						popup_->MoveTo(pos.x(), pos.y() - popup_->size().height());
						SetRoundType(RoundTopLeft | RoundTopRight);
					}

				}

			}

			context->AddFrame(popup_);
		}
		*/

		RequestRedraw();

		return Finish;
	}
	
	Response ComboBox::PerformMouseRelease (AbstractWindow* context)
	{
		status_down_ = false;

		RequestRedraw();
		return Finish;
	}
	
	void ComboBox::PerformHoverIn(AbstractWindow* context)
	{
		RequestRedraw();
	}

	void ComboBox::PerformHoverOut(AbstractWindow* context)
	{
		RequestRedraw();
	}

	void ComboBox::InitializeComboBox()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->menu().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->menu().shadetop,
					AbstractWindow::theme->menu().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.generate();

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);
		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(
				AttributeCoord);
		glVertexAttribPointer(AttributeCoord,
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		glEnableVertexAttribArray (AttributeCoord);
		glVertexAttribPointer (AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		vbo_.reset();
	}

	void ComboBox::OnPopupListDestroyed(AbstractFrame* frame)
	{
		//assert(frame == popup_);
		popup_->destroyed().disconnectOne(this, &ComboBox::OnPopupListDestroyed);
		popup_ = 0;
		SetRoundType(RoundAll);
	}

}
