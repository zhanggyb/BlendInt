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

#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>

#include <BlendInt/OpenGL/ScissorStatus.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>
#include <BlendInt/Gui/ContextLayer.hpp>

namespace BlendInt {

	class Interface;

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

		virtual void UpdateContainer (const ContainerUpdateRequest& request);

		virtual bool UpdateGeometryTest (const GeometryUpdateRequest& request);

		virtual void UpdateGeometry (const GeometryUpdateRequest& request);

		virtual void BroadcastUpdate (const GeometryUpdateRequest& request);

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

		void InitializeContext ();

		void DrawMainBuffer (const glm::mat4& mvp);

		void DrawLayers (const glm::mat4& mvp);

		void RenderToLayerBuffer (const RedrawEvent& event,
				int layer,
				const std::set<AbstractWidget*>& widgets,
				GLTexture2D* texture);

		void RenderToMainBuffer (const RedrawEvent& event);

		void PreDrawContext (bool fbo = false);

		void DispatchDrawEvent (AbstractWidget* widget, const RedrawEvent& event);

		bool DispatchMousePressEvent (int layer, const MouseEvent& event);

		bool DispatchMouseReleaseEvent (int layer, const MouseEvent& event);

		bool DispatchMouseMoveEvent (int layer, const MouseEvent& event);

		void BuildCursorHoverList (int layer);

		void AppendCursorHoverList (std::deque<AbstractWidget*>& deque, AbstractWidget* parent);

		void RemoveWidgetFromHoverList (AbstractWidget* widget, bool cursor_event = false);

		void RemoveSubWidgetFromHoverList (AbstractContainer* container, bool cursor_event = false);

		AbstractWidget* GetWidgetUnderCursor (const MouseEvent& event, AbstractWidget* parent);

		void OnSubWidgetDestroyed (AbstractWidget* widget);

		std::map<int, ContextLayer> m_layers;

		std::deque<GLTexture2D*> m_deque;

		GLTexture2D* m_main_buffer;

		RefPtr<GLArrayBuffer> m_vbo;

		GLuint m_vao;

		RedrawEvent m_redraw_event;

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
