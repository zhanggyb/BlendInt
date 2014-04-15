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

#ifndef _BLENDINT_CONTEXTMANAGER_HPP_
#define _BLENDINT_CONTEXTMANAGER_HPP_

#include <map>
#include <set>
#include <deque>

#include <boost/smart_ptr.hpp>

#include <glm/glm.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>

#include <BlendInt/OpenGL/ScissorStatus.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	class Interface;
	class AbstractWidget;
	class GLTexture2D;
	class ScreenBuffer;

	/**
	 * @brief Class to hold and manage widget objects for Render
	 */
	class ContextManager: public AbstractWidget
	{
	public:

		friend class Interface;
		friend class AbstractWidget;
		friend class AbstractContainer;

		static ContextManager* instance;

		bool AddSubWidget (AbstractWidget* obj);

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

#ifdef DEBUG

		void print ();

#endif

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

	private:

#ifdef DEBUG
		void DrawGrid (int width, int height);
#endif

		ContextManager ();

		~ContextManager ();

		bool Unregister (AbstractWidget* obj);

		void ResizeFromInterface (const Size& size);

		void ResizeFromInterface (unsigned int width, unsigned int height);

		void DrawFromInterface ();

		static bool Initialize ();

		static void Release ();

		void OffScreenRenderToTexture (int layer, std::set<AbstractWidget*>* widgets, GLTexture2D* texture);

		void RenderToScreenBuffer ();

		void PreDrawContext (bool fbo = false);

		void DispatchDrawEvent (AbstractWidget* widget, const RedrawEvent& event);

		void OnDestroyObject (AbstractWidget* obj);

		/**
		 * @brief add the drawable in the context list
		 * @param obj
		 *
		 * add or Update the Drawable object in the context list
		 */
		void AddWidget (AbstractWidget* obj);

		/**
		 * @breif remove a drawable from the context list
		 * @param obj
		 * @return
		 */
		bool RemoveWidget (AbstractWidget* obj);

		void RemoveWidgetFromHoverDeque (AbstractWidget* widget);

		void SetFocusedWidget (AbstractWidget* widget);

		void BuildWidgetListAtCursorPoint (const Point& cursor_point, AbstractWidget* parent);

		std::map<int, ContextLayer> m_layers;

		std::map<AbstractWidget*, int> m_index;

		GLTexture2D* m_main_buffer;

		ScreenBuffer* m_screenbuffer;

		RedrawEvent m_redraw_event;

		std::deque<GLTexture2D*> m_deque;

		/**
		 * @brief The widget stack to contain the current mouse cursor
		 */
		boost::scoped_ptr<std::deque<AbstractWidget*> > m_hover_deque;

		static ScissorStatus scissor_status;

		static bool refresh_once;

		static bool force_refresh_all;
	};

}

#endif	// _BIL_CONTEXTMANAGER_HPP_
