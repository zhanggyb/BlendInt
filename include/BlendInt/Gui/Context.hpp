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
#include <deque>
#include <set>

#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>

#include <BlendInt/OpenGL/ScissorStatus.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/AbstractContainer.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	struct ContextLayer {

		ContextLayer ();
		~ContextLayer ();

		/** If refresh this layer */
		bool refresh;

		/** A set to store sub widgets in this layer */
		std::set<AbstractWidget*>* widgets;

		/** The OpenGL Texture as a buffer for display */
		GLTexture2D* buffer;
	};

	/**
	 * @brief Container to hold and manage all widgets in a OpenGL window
	 *
	 * Context is a special container which holds and manage all widgets in a OpenGL window.
	 * There should be at least on Context object to work with Interface to show and dispatch events.
	 */
	class Context: public AbstractContainer
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

		bool Add (AbstractWidget* widget);

		bool Remove (AbstractWidget* widget);

		void SetFocusedWidget (AbstractWidget* widget);

		void RemoveWidgetFromHoverList (AbstractWidget* widget);

#ifdef DEBUG
		void PrintLayers ();
#endif

	protected:

		void Draw ();

		virtual bool Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual bool AddSubWidget (AbstractWidget* widget);

		virtual bool RemoveSubWidget (AbstractWidget* widget);

		virtual AbstractContainerIterator* First (const DeviceEvent& event);

		virtual bool End (const DeviceEvent& event, AbstractContainerIterator* iter);

	private:

		void InitOnce ();

		void DrawMainBuffer (const glm::mat4& mvp);

		void RenderLayer (const RedrawEvent& event,
				int layer,
				std::set<AbstractWidget*>* widgets,
				GLTexture2D* texture);

		void RenderMainBuffer (const RedrawEvent& event);

		void PreDrawContext (bool fbo = false);

		void DispatchDrawEvent (AbstractWidget* widget, const RedrawEvent& event);

		void BuildCursorHoverList (const MouseEvent& event, AbstractWidget* parent);

		void OnSubWidgetDestroyed (AbstractWidget* widget);

		std::map<int, ContextLayer> m_layers;

		std::map<AbstractWidget*, int> m_index;

		std::deque<GLTexture2D*> m_deque;

		GLTexture2D* m_main_buffer;

		RefPtr<GLArrayBuffer> m_vbo;
		RefPtr<GLArrayBuffer> m_tbo;

		GLuint m_vao;

		RedrawEvent m_redraw_event;

		boost::scoped_ptr<std::deque<AbstractWidget*> > m_hover_deque;

		RefPtr<GLSLProgram> m_program;

		static ScissorStatus scissor_status;

		static bool refresh_once;

		static bool force_refresh_all;
	};

}


#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
