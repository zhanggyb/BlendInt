
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

#include <algorithm>

#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Gui/Stack.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Stack::Stack()
	: AbstractStackLayout(),
	  active_widget_(0),
	  vao_(0)
	{
		set_drop_shadow(true);
		InitializeStack();
	}

	Stack::~Stack ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Stack::PushFront (AbstractWidget* widget)
	{
		if(PushFrontSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			if(widget_count() == 1) {
				active_widget_ = widget;
				active_widget_->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void Stack::PushBack (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			if(widget_count() == 1) {
				active_widget_ = widget;
				active_widget_->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void Stack::Insert (int index, AbstractWidget* widget)
	{
		if(InsertSubWidget(index, widget)) {
			int w = size().width() - margin().left() - margin().right();
			int h = size().height() - margin().top() - margin().bottom();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			widget->SetVisible(false);
		}
	}

	void Stack::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {

			if(active_widget_ == widget) {

				if(widget_count() == 0) {
					active_widget_ = 0;
				} else {
					active_widget_ = first();
					active_widget_->SetVisible(true);
				}

			}
		}
	}

	int Stack::GetIndex() const
	{
		int index = 0;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p == active_widget_) {
				break;
			}

			index++;
		}

		if(index >= widget_count()) index = -1;

		return index;
	}

	void Stack::SetIndex (int index)
	{
		int count = widget_count();

		if(index > (count - 1)) return;

		if(count) {

			AbstractWidget* widget = GetWidgetAt(index);
			if(active_widget_ == widget) {
				return;
			}

			active_widget_->SetVisible(false);
			active_widget_ = widget;
			active_widget_->SetVisible(true);
		}
	}

	bool Stack::IsExpandX () const
	{
		bool ret = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	bool Stack::IsExpandY () const
	{
		bool ret = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	Size Stack::GetPreferredSize () const
	{
		Size prefer(400, 300);

		if(first()) {

			prefer.set_width(0);
			prefer.set_height(0);

			Size tmp;
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				tmp = p->GetPreferredSize();
				prefer.set_width(std::max(prefer.width(), tmp.width()));
				prefer.set_height(std::max(prefer.height(), tmp.height()));
			}

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());

		}

		return prefer;
	}

	void Stack::PerformMarginUpdate(const Margin& request)
	{
		int w = size().width() - request.hsum();
		int h = size().height() - request.vsum();

		ResizeSubWidgets(w, h);
	}

	ResponseType Stack::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(),
		        (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.447f,
		        0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);
		glBindVertexArray(0);

		program->reset();
		return Ignore;
	}

	void Stack::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void Stack::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			set_size(*request.size());
			ResizeSubWidgets(w, h);
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Stack::HideSubWidget(int index)
	{
		if(widget_count() && index < (widget_count() - 1)) {
			AbstractWidget* p = GetWidgetAt(index);
			p->SetVisible(false);
		}
	}

	ResponseType Stack::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Stack::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Stack::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Stack::InitializeStack()
	{
		glGenVertexArrays(1, &vao_);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0);

		glBindVertexArray(vao_);

		inner_.reset(new GLArrayBuffer);

		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

}
