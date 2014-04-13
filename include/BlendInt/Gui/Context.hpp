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

#ifndef _BLENDINT_GUI_CONTEXT_HPP_
#define _BLENDINT_GUI_CONTEXT_HPP_

#include <map>
#include <set>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	struct ContextLayerExt {

		ContextLayerExt ();
		~ContextLayerExt ();

		bool refresh;	/** If refresh this layer */
		std::set<AbstractWidgetExt*>* widgets;	/** A set to store sub widgets in this layer */
		GLTexture2D* buffer;	/** The OpenGL Texture as a buffer for display */
	};

	/**
	 * @brief Container to hold and manage all widgets in a OpenGL window
	 *
	 * Context is a special container which holds and manage all widgets in a OpenGL window.
	 * There should be at least on Context object to work with Interface to show and dispatch events.
	 */
	class Context: public AbstractContainerExt
	{
		DISALLOW_COPY_AND_ASSIGN(Context);

		friend class Interface;

	public:

		Context ();

		virtual ~Context ();

		size_t index_size () const
		{
			return m_index.size();
		}

		size_t layer_size () const
		{
			return m_layers.size();
		}

		int GetMaxLayer () const;

		void RefreshLayer (int layer);

	protected:

		virtual bool Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual bool AddSubWidget (AbstractWidgetExt* widget);

		virtual bool RemoveSubWidget (AbstractWidgetExt* widget);

		virtual AbstractWidgetIterator* First (const DeviceEvent& event);

		virtual bool End (const DeviceEvent& event, AbstractWidgetIterator* iter);

	private:

		void OnSubWidgetDestroyed (AbstractWidgetExt* widget);

		std::map<int, ContextLayerExt> m_layers;

		std::map<AbstractWidgetExt*, int> m_index;

		static bool refresh_once;

		static bool force_refresh_all;

	};

}


#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
