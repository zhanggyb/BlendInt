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

#include <set>

#include <boost/smart_ptr.hpp>

#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	/**
	 * @brief Layout to hold and manage all widgets in a OpenGL window
	 *
	 * Context is a special container which holds and manage all widgets in a OpenGL window.
	 * There should be at least one Context object to work with Interface to show and dispatch events.
	 */
	class Context: public AbstractView
	{
		DISALLOW_COPY_AND_ASSIGN(Context);

	public:

		static bool Initialize ();

		static void Release ();

		Context ();

		virtual ~Context ();

		void AddFrame (AbstractFrame* vp);

		void Draw ();

		void DispatchKeyEvent (const KeyEvent& event);

		void DispatchMouseEvent (const MouseEvent& event);

		/**
		 * @brief Always return true
		 */
		virtual bool Contain (const Point& point) const;

		virtual void SynchronizeWindow ();

		virtual void MakeGLContextCurrent ();

		Cpp::EventRef<const Size&> resized ()
		{
			return resized_;
		}

		static Context* GetContext (AbstractView* widget);

		static glm::mat4 default_view_matrix;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void SetFocusedFrame (AbstractFrame* frame);

		Cpp::ConnectionScope* events() const {return events_.get();}

	private:

		static void GetGLVersion (int *major, int *minor);

		static void GetGLSLVersion (int *major, int *minor);

		void InitializeContext ();

		void DispatchHoverEvent (const MouseEvent& event);

		void OnFocusedFrameDestroyed (AbstractFrame* frame);

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		//void RenderToBuffer (Profile& profile);

        //GLuint vao_;
        
		Profile profile_;

		AbstractFrame* focused_frame_;
        
        //GLTexture2D texture_buffer_;

		//GLBuffer<> vertex_buffer_;

		Cpp::Event<const Size&> resized_;

		static std::set<Context*> context_set;

	};

}


#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
