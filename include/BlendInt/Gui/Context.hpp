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
		RefPtr<GLTexture2D> tex_buf_ptr;
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

		bool Add (AbstractWidget* widget);

		bool Remove (AbstractWidget* widget);

		void SetFocusedWidget (AbstractWidget* widget);

		void RefreshLayer (int layer);

		int GetMaxLayer () const;

		size_t index_size () const
		{
			return m_index.size();
		}

		size_t layer_size () const
		{
			return m_layers.size();
		}

		AbstractWidget* focused_widget () const
		{
			return m_focused_widget;
		}

		/**
		 * @brief Set the cursor
		 */
		virtual void SetCursor (int cursor_type);

		virtual int GetCursor () const;

		virtual void PushCursor (int cursor_type);

		virtual int PopCursor ();

		Cpp::EventRef<const Size&> resized ()
		{
			return m_resized;
		}

#ifdef DEBUG
		void PrintLayers ();
#endif

		void set_max_tex_buffer_cache_size (unsigned int size)
		{
			m_max_tex_buffer_cache_size = size;
		}

		unsigned int max_tex_buffer_cache_size () const
		{
			return m_max_tex_buffer_cache_size;
		}

	protected:

		virtual void UpdateContainer (const WidgetUpdateRequest& request);

		virtual bool UpdateGeometryTest (const WidgetUpdateRequest& request);

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual void BroadcastUpdate (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual bool InsertSubWidget (AbstractWidget* widget);

		virtual bool RemoveSubWidget (AbstractWidget* widget);

		virtual IteratorPtr CreateIterator (const DeviceEvent& event);

	private:

		void RemoveWidgetFromHoverList (AbstractWidget* widget);

		void InitializeContext ();

		void DrawMainBuffer (const glm::mat4& mvp);

		void DrawLayers (const glm::mat4& mvp);

		void RenderToLayerBuffer (const RedrawEvent& event,
				int layer,
				std::set<AbstractWidget*>* widgets,
				GLTexture2D* texture);

		void RenderToMainBuffer (const RedrawEvent& event);

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

		/**
		 * @brief Focused widget
		 *
		 * There's one focused widget in each context to access key and button events
		 */
		AbstractWidget* m_focused_widget;

		/**
		 * @brief A stack to store unused texture buffer
		 */
		std::stack<RefPtr<GLTexture2D> > m_tex_buffer_cache;

		unsigned int m_max_tex_buffer_cache_size;

		ScissorStatus scissor_status;

		bool refresh_once;

		bool force_refresh_all;

		Cpp::Event<const Size&> m_resized;

		static std::set<Context*> context_set;
	};

}


#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
