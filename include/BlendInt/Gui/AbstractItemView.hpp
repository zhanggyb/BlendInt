/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_ABSTRACTITEMVIEW_HPP_
#define _BLENDINT_GUI_ABSTRACTITEMVIEW_HPP_

#include <Cpp/Events.hpp>

#include <BlendInt/Gui/AbstractItemModel.hpp>
#include <BlendInt/Gui/AbstractScrollable.hpp>

namespace BlendInt {

	/**
	 * @brief The base class for item view classes.
	 *
	 * AbstractItemView class is the base class for every standard
	 * view that uses a AbstractItemModel. It provides a standard
	 * interface for interoperating with models.
	 *
	 * AbstractItemView is a subclass of AbstractScrollable, which
	 * means it contains 2 native ScrollBars. The sub class of this
	 * view should manage the display and behavior of these 2
	 * ScrollBars by itself.
	 *
	 * Usually the sub class should use native widgets to display
	 * the data in model.
	 */
	class AbstractItemView: public AbstractScrollable
	{
	public:

		AbstractItemView ();

		virtual ~AbstractItemView ();

		const RefPtr<AbstractItemModel>& model () const
		{
			return model_;
		}

		virtual void SetModel (const RefPtr<AbstractItemModel>& model);

		virtual ModelIndex GetIndexAt (const Point& point) const = 0;

		Cpp::EventRef<> model_changed ()
		{
			return model_changed_;
		}

	protected:

		void set_model (const RefPtr<AbstractItemModel>& model)
		{
			model_ = model;
		}

	private:

		RefPtr<AbstractItemModel> model_;

		Cpp::Event<> model_changed_;

	};

}

#endif	// _BLENDINT_GUI_ABSTRACTITEMVIEW_HPP_
