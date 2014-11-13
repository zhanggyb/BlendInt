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

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Widget::Widget()
	: AbstractWidget()
	{
		destroyed_.reset(new Cpp::Event<Widget*>);
	}

	Widget::~Widget()
	{
		destroyed_->fire(this);
	}

	bool Widget::PreDraw(Profile& profile)
	{
		if(!visiable()) return false;

		//glm::mat4 model;
		//Shaders::instance->GetUIModelMatrix(model);

//		Point pos = GetGlobalPosition();

		glm::mat4 matrix = glm::translate(Shaders::instance->widget_model_matrix(),
				glm::vec3(position().x() + offset().x(), position().y() + offset().y(), 0.f));
//		glm::mat4 matrix = glm::translate(glm::mat4(1.f), glm::vec3(pos.x() + offset_x(), pos.y() + offset_y(), 0.f));

		Shaders::instance->PushWidgetModelMatrix();
		Shaders::instance->SetWidgetModelMatrix(matrix);

		return true;
	}

	ResponseType Widget::Draw(Profile& profile)
	{
		return subs_count() ? Ignore : Accept;
	}

	void Widget::PostDraw(Profile& profile)
	{
		Shaders::instance->PopWidgetModelMatrix();
	}

	void Widget::FocusEvent (bool focus)
	{
	}

	void Widget::MouseHoverInEvent(const MouseEvent& event)
	{

	}

	void Widget::MouseHoverOutEvent(const MouseEvent& event)
	{

	}

	ResponseType Widget::KeyPressEvent (const KeyEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Widget::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Widget::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Widget::MousePressEvent (const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Widget::MouseReleaseEvent (const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}
	
	ResponseType Widget::MouseMoveEvent (const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

}
