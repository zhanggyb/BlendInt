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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	ScrollArea::ScrollArea ()
	: AbstractVectorContainer(4)
	{
		set_margin(2, 2, 2, 2);
		set_size(360, 240);

		InitializeScrollArea();
	}

	ScrollArea::~ScrollArea ()
	{
	}

	void ScrollArea::SetViewport (AbstractWidget* widget)
	{
		if (!widget)
			return;

		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));
		view->SetViewport(widget);

		int width = size().width() - margin().hsum();
		int height = size().height() - margin().vsum();

		if (widget->size().width() <= width) {
			sub_widget(HScrollBarIndex)->SetVisible(false);
		} else {
			sub_widget(HScrollBarIndex)->SetVisible(true);
		}

		if (widget->size().height() <= height) {
			sub_widget(VScrollBarIndex)->SetVisible(false);
		} else {
			sub_widget(VScrollBarIndex)->SetVisible(true);
		}

		AdjustGeometries(position().x() + margin().left(),
						position().y() + margin().bottom(), width, height);
	}

	void ScrollArea::CentralizeViewport () const
	{
		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));
		view->CentralizeViewport();
	}

	bool ScrollArea::IsExpandX() const
	{
		return true;
	}

	bool ScrollArea::IsExpandY() const
	{
		return true;
	}

	ResponseType ScrollArea::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ScrollArea::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void ScrollArea::UpdateContainer(const ContainerUpdateRequest& request)
	{
		// TODO: set margin

		ReportContainerUpdate(request);
	}

	void ScrollArea::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());
					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();
					set_position(*pos_p);
					MoveSubWidgets(x, y);
					break;
				}

				case WidgetSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					VertexTool tool;
					tool.Setup(*size_p, 0, RoundNone, 0);
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());

					AdjustGeometries(position(), *size_p, margin());
					set_size(*size_p);

					break;
				}

				default:
					break;
			}

		}

		ReportGeometryUpdate(request);
	}

	ResponseType ScrollArea::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 0);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);
		glBindVertexArray(0);

		program->Reset();

		return AcceptAndContinue;
	}

	void ScrollArea::InitializeScrollArea ()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());

		glBindVertexArray(m_vao);
		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

		ScrollView* view = Manage(new ScrollView);
		ScrollBar* hbar = Manage(new ScrollBar(Horizontal));
		ScrollBar* vbar = Manage(new ScrollBar(Vertical));

		SetSubWidget(0, view);
		SetSubWidget(1, hbar);
		SetSubWidget(2, vbar);

		//m_hbar->SetVisible(false);
		//m_vbar->SetVisible(false);

		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();
		int w = size().width() - margin().hsum();
		int h = size().height() - margin().vsum();
		int bh = hbar->size().height();
		int rw = vbar->size().width();

		SetSubWidgetPosition(hbar, x, y);
		SetSubWidgetPosition(vbar, x + w - rw, y + bh);
		SetSubWidgetPosition(view, x, y + bh);

		ResizeSubWidget (hbar, w - rw, bh);
		ResizeSubWidget (vbar, rw, h - bh);
		ResizeSubWidget (view, w - rw, h - bh);

		events()->connect(hbar->slider_moved(), this, &ScrollArea::OnHorizontalScroll);
		events()->connect(vbar->slider_moved(), this, &ScrollArea::OnVerticalScroll);
	}
	
	void ScrollArea::AdjustGeometries (const Point& out_pos, const Size& out_size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		AdjustGeometries(x, y, w, h);
	}

	void ScrollArea::AdjustGeometries (int x, int y, int width, int height)
	{
		int bh = 0;
		int rw = 0;

		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));
		ScrollBar* hbar = dynamic_cast<ScrollBar*>(sub_widget(HScrollBarIndex));
		ScrollBar* vbar = dynamic_cast<ScrollBar*>(sub_widget(VScrollBarIndex));

		if(hbar->visiable()) {
			bh = hbar->size().height();
		}

		if(vbar->visiable()) {
			rw = vbar->size().width();
		}

		SetSubWidgetPosition(view, x, y + bh);
		ResizeSubWidget (view, width - rw, height - bh);

		//view->CentralizeViewport();

		AbstractWidget* widget = view->viewport();

		if(hbar->visiable()) {
			SetSubWidgetPosition(hbar, x, y);
			ResizeSubWidget (hbar, width - rw, bh);

			if(widget) {
				int percent = view->GetHPercentage();
				hbar->SetMaximum(widget->size().width());
				hbar->SetMinimum(view->size().width());
				hbar->SetSliderPercentage(percent);
			}
		}

		if(vbar->visiable()) {
			SetSubWidgetPosition(vbar, x + width - rw, y + bh);
			ResizeSubWidget (vbar, rw, height - bh);

			if(widget) {
				int percent = view->GetVPercentage();
				vbar->SetMaximum(widget->size().height());
				vbar->SetMinimum(view->size().height());
				vbar->SetSliderPercentage(percent);
			}
		}

	}

	void ScrollArea::OnHorizontalScroll (int value)
	{
		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));

		AbstractWidget* p = view->viewport();
		if (p) {
			view->SetReletivePosition(value - p->size().width(),
			        p->position().y() - view->position().y());
		}
	}

	void ScrollArea::OnVerticalScroll (int value)
	{
		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));

		AbstractWidget* p = view->viewport();
		if (p) {
			view->SetReletivePosition(
			        p->position().x() - view->position().x(),
			        value - p->size().height());
		}
	}

}
