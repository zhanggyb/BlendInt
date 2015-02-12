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

#include <assert.h>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <opengl/gl-framebuffer.hpp>

#include <gui/panel.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	Panel::Panel ()
	: Widget(),
	  layout_(0)
	{
		set_size(400, 300);
		set_refresh(true);

		InitializePanelOnce();
	}

	Panel::~Panel ()
	{
		glDeleteVertexArrays(3, vao_);
	}

	void Panel::SetLayout(AbstractLayout* layout)
	{
		if((layout == 0) || (layout == layout_)) return;

		if(layout_) {
			layout_->destroyed().disconnectOne(this, &Panel::OnLayoutDestroyed);
		}

		for(AbstractView* p = first_subview(); p; p->next_view()) {
			layout->AddWidget(dynamic_cast<AbstractWidget*>(p));
		}

		if(PushBackSubView(layout)) {
			layout_ = layout;
			events()->connect(layout_->destroyed(), this, &Panel::OnLayoutDestroyed);
			MoveSubViewTo(layout_, 0, 0);
			ResizeSubView(layout_, size());
		} else {
			DBG_PRINT_MSG("Warning: %s", "Fail to set layout");
		}

		RequestRedraw();
	}

	void Panel::AddWidget(AbstractWidget* widget)
	{
		if(layout_) {
			layout_->AddWidget(widget);
		} else {
			PushBackSubView(widget);
		}

		RequestRedraw();
	}

	void Panel::InsertWidget(int index, AbstractWidget* widget)
	{
		if(layout_) {
			layout_->InsertWidget(index, widget);
		} else {
			InsertSubView(index, widget);
		}

		RequestRedraw();
	}

	bool Panel::IsExpandX() const
	{
		if(layout_) {
			return layout_->IsExpandX();
		}

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandX()) return true;
		}

		return false;
	}

	bool Panel::IsExpandY() const
	{
		if(layout_) {
			return layout_->IsExpandY();
		}

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandY()) return true;
		}

		return false;
	}

	Size Panel::GetPreferredSize() const
	{
		Size prefer_size;

		if(subs_count() == 0) {
			prefer_size.reset(400, 300);
		} else {

			if(layout_) {
				assert(subs_count() == 1);
				prefer_size = layout_->GetPreferredSize();
			} else {

				int minx = 0;
				int miny = 0;
				int maxx = 0;
				int maxy = 0;

				for(AbstractView* p = first_subview(); p; p = p->next_view()) {
					minx = std::min(minx, p->position().x());
					miny = std::min(miny, p->position().y());
					maxx = std::max(maxx, p->position().x() + p->size().width());
					maxy = std::max(maxy, p->position().y() + p->size().height());
				}

				prefer_size.reset(maxx - minx, maxy - miny);
			}
		}

		return prefer_size;
	}

	void Panel::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (AbstractWindow::theme->regular().shaded) {
				GenerateRoundedVertices(Vertical,
						AbstractWindow::theme->regular().shadetop,
						AbstractWindow::theme->regular().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			buffer_.bind(2);
			float* ptr = (float*)buffer_.map();
			*(ptr + 4) = (float)size().width();
			*(ptr + 9) = (float)size().height();
			*(ptr + 12) = (float)size().width();
			*(ptr + 13) = (float)size().height();
			buffer_.unmap();

			buffer_.reset();

			if(layout_) {
				ResizeSubView(layout_, size());
			}

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Panel::PerformRoundTypeUpdate(int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->regular().shadetop,
					AbstractWindow::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		RequestRedraw();
	}

	void Panel::PerformRoundRadiusUpdate(float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->regular().shadetop,
					AbstractWindow::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		RequestRedraw();
	}

	Response Panel::Draw (AbstractWindow* context)
	{
		if(refresh()) {
			//DBG_PRINT_MSG("%s", "refresh once");
			RenderSubWidgetsToTexture (this, context, &texture_buffer_);
		}

		AbstractWindow::shaders->widget_inner_program()->use();

		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				AbstractWindow::theme->regular().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION), 0.f, 0.f);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1,
		        AbstractWindow::theme->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
			        0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        AbstractWindow::shaders->widget_image_program()->use();

        texture_buffer_.bind();
        glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_POSITION), 0.f, 0.f);
        glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
        glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_GAMMA), 0);

        glBindVertexArray(vao_[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        texture_buffer_.reset();
        GLSLProgram::reset();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return Finish;
	}

	void Panel::InitializePanelOnce ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->regular().shadetop,
					AbstractWindow::theme->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(3, vao_);
		buffer_.generate ();

		glBindVertexArray(vao_[0]);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[2]);
		GLfloat vertices[] = {
				// coord											uv
				0.f, 0.f,											0.f, 0.f,
				(float)size().width(), 0.f,							1.f, 0.f,
				0.f, (float)size().height(),						0.f, 1.f,
				(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		buffer_.bind(2);
		buffer_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (AttributeCoord);
		glEnableVertexAttribArray (AttributeUV);
		glVertexAttribPointer (AttributeCoord, 2, GL_FLOAT, GL_FALSE,
				sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (AttributeUV, 2,
		GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		buffer_.reset();
	}

	void Panel::OnLayoutDestroyed(AbstractWidget* layout)
	{
#ifdef DEBUG
		assert(layout == layout_);
#endif

		DBG_PRINT_MSG("layout %s is destroyed", layout->name().c_str());
		layout->destroyed().disconnectOne(this, &Panel::OnLayoutDestroyed);
		layout_ = 0;
	}

} /* namespace BlendInt */
