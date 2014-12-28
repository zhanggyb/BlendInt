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
#include <glm/gtx/matrix_transform_2d.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	using Stock::Shaders;

	AbstractWidget::AbstractWidget()
	: AbstractView()
	{
		events_.reset(new Cpp::ConnectionScope);
		destroyed_.reset(new Cpp::Event<AbstractWidget*>);
	}

	AbstractWidget::~AbstractWidget()
	{
		destroyed_->fire(this);
	}

	bool AbstractWidget::PreDraw(Profile& profile)
	{
		if(!visiable()) return false;

		//glm::mat4 model;
		//Shaders::instance->GetUIModelMatrix(model);

//		Point pos = GetGlobalPosition();

		glm::mat3 matrix = glm::translate(Shaders::instance->widget_model_matrix(),
				glm::vec2(position().x() + offset().x(), position().y() + offset().y()));
//		glm::mat4 matrix = glm::translate(glm::mat4(1.f), glm::vec3(pos.x() + offset_x(), pos.y() + offset_y(), 0.f));

		Shaders::instance->PushWidgetModelMatrix();
		Shaders::instance->SetWidgetModelMatrix(matrix);

		return true;
	}

	void AbstractWidget::PostDraw(Profile& profile)
	{
		Shaders::instance->PopWidgetModelMatrix();
	}

	void AbstractWidget::FocusEvent (bool focus)
	{
	}

	void AbstractWidget::MouseHoverInEvent(const MouseEvent& event)
	{

	}

	void AbstractWidget::MouseHoverOutEvent(const MouseEvent& event)
	{

	}

	ResponseType AbstractWidget::KeyPressEvent (const KeyEvent& event)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::MousePressEvent (const MouseEvent& event)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::MouseReleaseEvent (const MouseEvent& event)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::MouseMoveEvent (const MouseEvent& event)
	{
		return subs_count() ? Ignore : Finish;
	}

}
