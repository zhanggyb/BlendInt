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

#include <gui/tab-button.hpp>
#include <gui/tab-header.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	TabHeader::TabHeader()
	: AbstractWidget(),
	  vao_(0)
	{
		set_size(320, 20);

		events()->connect(group_.button_index_clicked(), &m_button_index_clicked, &Cpp::Event<int>::fire);
		//events()->connect(m_group.button_index_clicked(), this, &TabHeader::OnButtonIndexClicked);
		events()->connect(group_.button_index_toggled(), this, &TabHeader::OnButtonIndexToggled);

		// FIXME: cannot use the following line
		//events()->connect(m_group.button_index_toggled(), &m_button_index_toggled, &Cpp::Event<int, bool>::fire);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(0.f, 0.f, size().width(), kBaseLine, 0.f, RoundNone, 0.f,
                     &inner_verts, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    vbo_.generate();
    vbo_.bind();
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);

    glBindVertexArray(0);
    vbo_.reset();

	}

	TabHeader::~TabHeader()
	{
	  glDeleteVertexArrays(1, &vao_);
	}

	bool TabHeader::AddButton (TabButton* button)
	{
		int x = GetLastPosition ();
		int y = kBaseLine - 1;
		int h = size().height();

		if (PushBackSubView(button)) {

			if (button->IsExpandY()) {
				ResizeSubView(button, button->size().width(), h);
			} else {

				if (button->size().height() > h) {
					ResizeSubView(button, button->size().width(), h);
				}

			}

      if (subs_count() == 1) {
        MoveSubViewTo(button, x, y);
        button->SetRoundType(RoundTopLeft | RoundTopRight);
      } else {
        MoveSubViewTo(button, x - 1, y);
        TabButton* orig_last = dynamic_cast<TabButton*>(button->previous_view());
        orig_last->SetRoundType(orig_last->round_type() & ~RoundTopRight);
        button->SetRoundType(RoundTopRight);
      }

			group_.AddButton(button);

			if(group_.button_count() == 1) {
				button->SetChecked(true);
			}

			return true;
		}

		return false;
	}

  bool TabHeader::InsertButton (int index, TabButton* button)
  {


    return false;
  }

	bool TabHeader::IsExpandX () const
	{
		return true;
	}

	Size TabHeader::GetPreferredSize () const
	{
		if(subs_count() == 0) {
		  return Size(320, Font::default_height());
		} else {
			Size tmp;
			int w = kLeftPadding + kRightPadding;
			int h = 0;

			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(p->visiable()) {
					tmp = p->GetPreferredSize();
					w += tmp.width();
					h = std::max(h, tmp.height());
				}
			}

			return Size(w, h);
		}
	}

	Response TabHeader::Draw (AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	void TabHeader::OnButtonIndexToggled(int index, bool toggled)
	{
		m_button_index_toggled.fire(index, toggled);
	}

  void TabHeader::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {
      set_size(*request.size());

      std::vector<GLfloat> inner_verts;
      GenerateVertices(0.f, 0.f, size().width(), kBaseLine, 0.f, RoundNone, 0.f,
                       &inner_verts, 0);

      vbo_.bind();
      vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                        &inner_verts[0]);
      vbo_.reset();

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  void TabHeader::PostDraw (AbstractWindow* context)
  {
    Color baseline_color = context->theme()->tab().inner_sel;

    if (context->theme()->tab().shaded)
      baseline_color = baseline_color + context->theme()->tab().shadedown;

    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        baseline_color.data());

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    return AbstractWidget::PostDraw(context);
  }

	int TabHeader::GetLastPosition() const
	{
		int x = kLeftPadding;

		if(subs_count()) {
			x = last_subview()->position().x()+ last_subview()->size().width();
		}

		return x;
	}

}
