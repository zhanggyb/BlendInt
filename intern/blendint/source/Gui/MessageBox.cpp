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

#include <BlendInt/Gui/MessageBox.hpp>
#include <BlendInt/Gui/LinearLayout.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {
    
    MessageBox::MessageBox(const String& title, const String& description)
    : AbstractDialog(),
      title_(0),
      description_(0),
      close_(0)
    {
		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));

		title_ = new Label(title, AlignCenter);
		description_ = new Label(description);
		close_ = new CloseButton;

		events()->connect(close_->clicked(), this, &MessageBox::OnClose);

		LinearLayout* hlayout = new LinearLayout(Horizontal);
		hlayout->SetMargin(Margin(0, 0, 0, 0));
		hlayout->SetSpace(0);
		hlayout->AddWidget(close_);
		hlayout->AddWidget(title_);

		LinearLayout* vlayout = new LinearLayout(Vertical);
		vlayout->AddWidget(hlayout);
		vlayout->AddWidget(description_);

		vlayout->Resize(size());

		PushBackSubView(vlayout);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->dialog().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->dialog().shadetop,
					AbstractWindow::theme->dialog().shadedown,
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
    }
    
    MessageBox::~MessageBox()
    {
        glDeleteVertexArrays(2, vao_);
    }

    void MessageBox::UpdateLayout()
    {
		shadow_->Resize(size());

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->dialog().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->dialog().shadetop,
					AbstractWindow::theme->dialog().shadedown,
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
    }
    
    ResponseType BlendInt::MessageBox::Draw (AbstractWindow* context)
    {
    	shadow_->Draw(position().x(), position().y());

		AbstractWindow::shaders->frame_inner_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(AbstractWindow::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_INNER_COLOR), 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		ResponseType retval = AbstractDialog::Draw(context);

		AbstractWindow::shaders->frame_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_POSITION), position().x(), position().y());
		glUniform4f(AbstractWindow::shaders->location(Shaders::FRAME_OUTER_COLOR), 0.f, 0.f, 0.f, 1.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		return retval;
    }

	void MessageBox::OnClose (AbstractButton* btn)
	{
		delete this;
	}

}
