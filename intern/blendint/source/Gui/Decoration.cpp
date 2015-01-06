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
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Decoration.hpp>

#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Decoration::Decoration(const String& title)
	: AbstractDecoration(),
	  space_(4),
	  close_button_(nullptr)
	{
		// create close button
		close_button_ = Manage(new CloseButton);
		title_label_ = Manage(new Label(title));

		int h = std::max(close_button_->size().height(), title_label_->size().height());

		set_size(200, 5 + 5 + h);
		close_button_->MoveTo(5, size().height() - close_button_->size().height() - 5);
		title_label_->MoveTo(close_button_->position().x() + close_button_->size().width(), 5);

		PushBackSubView(close_button_);
		PushBackSubView(title_label_);

		set_round_type(RoundTopLeft | RoundTopRight);
		//set_round_radius(5.f);

		InitializeDecorationOnce();

		events()->connect(close_button_->clicked(), this, &Decoration::OnCloseButtonClicked);
	}

	Decoration::~Decoration ()
	{
		glDeleteVertexArrays(1, vao_);
	}

	/*
	bool Decoration::AddWidget(AbstractWidget* widget)
	{
		if(PushBackSubView(widget)) {
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool Decoration::InsertWidget(int index, AbstractWidget* widget)
	{
		if(InsertSubView(index, widget)) {
			RequestRedraw();
			return true;
		}

		return false;
	}
	*/

	void Decoration::SetTitle(const String& title)
	{

	}

	bool Decoration::IsExpandX () const
	{
		return true;
	}

	bool Decoration::IsExpandY () const
	{
		return true;
	}

	Size Decoration::GetPreferredSize () const
	{
		int h = std::max(close_button_->size().height(), title_label_->size().height());

		return Size(200, h + 5 + 5);
	}

	void Decoration::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;

			if (Context::theme->decoration().shaded) {
				GenerateRoundedVertices(Vertical,
						Context::theme->decoration().shadetop,
						Context::theme->decoration().shadedown,
						&inner_verts,
						nullptr);
			} else {
				GenerateRoundedVertices(&inner_verts, nullptr);
			}

			inner_.bind();
			inner_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			inner_.reset();

			UpdateLayout();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Decoration::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;

		if (Context::theme->decoration().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->decoration().shadetop,
					Context::theme->decoration().shadedown,
					&inner_verts,
					nullptr);
		} else {
			GenerateRoundedVertices(&inner_verts, nullptr);
		}

		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		inner_.reset();

		RequestRedraw();
	}

	void Decoration::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;

		if (Context::theme->decoration().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->decoration().shadetop,
					Context::theme->decoration().shadedown,
					&inner_verts,
					nullptr);
		} else {
			GenerateRoundedVertices(&inner_verts, nullptr);
		}

		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		inner_.reset();

		RequestRedraw();
	}

	ResponseType Decoration::Draw (const Context* context)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1, Context::theme->decoration().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);

		GLSLProgram::reset();
		return Ignore;
	}

	void Decoration::UpdateLayout()
	{
		/*
		int x = 5;
		int y = 0;
		for(AbstractView* p = first_subview(); p != nullptr; p = p->next_view())
		{
			y = (size().height() - p->size().height()) / 2;
			p->MoveTo(x, y);
			x += (p->size().width() + space_);
		}
		*/

		int x = 5;
		int y = (size().height() - close_button_->size().height()) / 2;
		close_button_->MoveTo(x, y);
		x += (close_button_->size().width() + space_);

		y = (size().height() - title_label_->size().height()) / 2;
		title_label_->MoveTo(x, y);
		title_label_->Resize(size().width() - 5 - close_button_->size().width() - space_, title_label_->size().height());

	}

	void Decoration::InitializeDecorationOnce()
	{
		std::vector<GLfloat> inner_verts;

		if (Context::theme->decoration().shaded) {
			GenerateRoundedVertices(Vertical,
					Context::theme->decoration().shadetop,
					Context::theme->decoration().shadedown,
					&inner_verts,
					nullptr);
		} else {
			GenerateRoundedVertices(&inner_verts, nullptr);
		}

		glGenVertexArrays(1, vao_);

		glBindVertexArray(vao_[0]);

		inner_.generate();
		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		inner_.reset();
	}

	void Decoration::OnCloseButtonClicked(AbstractButton* button)
	{
		fire_close_triggered();
	}

}
