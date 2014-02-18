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

#ifndef _BLENDINT_POPUPWIDGET_HPP_
#define _BLENDINT_POPUPWIDGET_HPP_

#include <BlendInt/UI/Widget.hpp>

#include <BlendInt/UI/Shadow.hpp>

#include <BlendInt/UI/VertexIcon.hpp>

namespace BlendInt {

	/**
	 * @brief A special widget which is constructed in higher layer for
	 * popup menus, tooltips
	 *
	 * @note a popupwidget cannot be bound to another form in different layer,
	 * trying to do this will raise an exception.
	 *
	 * To layout a popupwidget with others, use a @ref LadderWidget to pack it.
	 *
	 * @sa LadderWidget
	 *
	 * @ingroup widgets
	 */
	class PopupWidget: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(PopupWidget);

	public:

		PopupWidget ();

		virtual ~PopupWidget();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Draw ();

	private:

		Shadow m_shadow;
	};

}

#endif /* _BLENDINT_POPUPWIDGET_HPP_ */
