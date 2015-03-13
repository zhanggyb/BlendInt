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

#include <glm/gtx/transform.hpp>

#include <gui/message-box.hpp>
#include <gui/linear-layout.hpp>

#include <gui/abstract-window.hpp>
#include <gui/font.hpp>

namespace BlendInt {
    
    MessageBox::MessageBox(const String& title, const String& description)
    : AbstractDialog(),
      title_(0),
      text_(0),
      close_(0)
    {
		title_ = new Label(title, AlignCenter);
		text_ = new Label(description, AlignCenter);
		close_ = new CloseButton;

		title_->SetForeground(AbstractWindow::theme()->menu_back().text_sel);
		text_->SetForeground(AbstractWindow::theme()->menu_back().text);

		events()->connect(close_->clicked(), this, &MessageBox::OnClose);

		LinearLayout* hlayout = new LinearLayout(Horizontal);
		hlayout->SetMargin(Margin(0, 0, 0, 0));
		hlayout->SetSpace(0);
		hlayout->AddWidget(close_);
		hlayout->AddWidget(title_);

		LinearLayout* vlayout = new LinearLayout(Vertical);
		vlayout->AddWidget(hlayout);
		vlayout->AddWidget(text_);

		vlayout->Resize(vlayout->GetPreferredSize());

		set_round_type(RoundAll);
		set_round_radius(5.f);
		set_size(vlayout->size());
		set_refresh(true);
		EnableViewBuffer();

		PushBackSubView(vlayout);

		projection_matrix_  = glm::ortho(
				0.f, (float)size().width(),
				0.f, (float)size().height(),
				100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme()->menu_back().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme()->menu_back().shadetop,
					AbstractWindow::theme()->menu_back().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(2, vao_);
		vbo_.generate();

		glBindVertexArray(vao_[0]);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3,
		GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));
    }
    
    MessageBox::~MessageBox()
    {
        glDeleteVertexArrays(2, vao_);
    }

    void MessageBox::SetTitleFont (const BlendInt::Font& font)
    {
    	title_->SetFont(font);
    }

    void MessageBox::SetTextFont (const BlendInt::Font& font)
    {
    	text_->SetFont(font);
    }

    void MessageBox::PerformSizeUpdate (const SizeUpdateRequest& request)
    {
    	if(request.target() == this) {

    		set_size(*request.size());

    		projection_matrix_  = glm::ortho(
    				0.f,
    				0.f + (float)size().width(),
    				0.f,
    				0.f + (float)size().height(),
    				100.f, -100.f);

    		if(view_buffer()) {
    			view_buffer()->Resize(size());
    		}

    		shadow_->Resize(size());

    		std::vector<GLfloat> inner_verts;
    		std::vector<GLfloat> outer_verts;

    		if (AbstractWindow::theme()->menu_back().shaded) {
    			GenerateRoundedVertices(Vertical,
    					AbstractWindow::theme()->menu_back().shadetop,
    					AbstractWindow::theme()->menu_back().shadedown,
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

    		ResizeSubView(first_subview(), size());

    		RequestRedraw();

    	}

    	if(request.source() == this) {
    		ReportSizeUpdate(request);
    	}
    }

    bool MessageBox::PreDraw (AbstractWindow* context)
    {
		if(!visiable()) return false;

		context->register_active_frame(this);

		if(refresh() && view_buffer()) {
			RenderSubFramesToTexture(this,
					context,
					projection_matrix_,
					model_matrix_,
					view_buffer()->texture());
		}

		return true;
    }

    Response BlendInt::MessageBox::Draw (AbstractWindow* context)
    {
    	shadow_->Draw(position().x(), position().y());

		AbstractWindow::shaders()->frame_inner_program()->use();

		glUniform2f(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4fv(AbstractWindow::shaders()->location(Shaders::FRAME_INNER_COLOR), 1, AbstractWindow::theme()->menu_back().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		if(view_buffer()) {

			AbstractWindow::shaders()->frame_image_program()->use();

			glUniform2f(AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_POSITION), position().x(), position().y());
			glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_TEXTURE), 0);
			glUniform1i(AbstractWindow::shaders()->location(Shaders::FRAME_IMAGE_GAMMA), 0);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			view_buffer()->Draw(0, 0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		} else {

            glViewport(position().x(), position().y(), size().width(), size().height());

            AbstractWindow::shaders()->SetWidgetProjectionMatrix(projection_matrix_);
            AbstractWindow::shaders()->SetWidgetModelMatrix(model_matrix_);

			DrawSubViewsOnce(context);

			glViewport(0, 0, context->size().width(), context->size().height());

		}

		AbstractWindow::shaders()->frame_outer_program()->use();

		glUniform2f(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		glUniform4fv(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_COLOR), 1, AbstractWindow::theme()->menu_back().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

        return Finish;
    }

	void MessageBox::OnClose (AbstractButton* btn)
	{
		AbstractView* super = superview();
		delete this;
		super->RequestRedraw();
	}

}
