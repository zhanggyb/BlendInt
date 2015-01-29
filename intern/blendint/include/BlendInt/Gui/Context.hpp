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

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Input.hpp>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/AbstractFrame.hpp>

#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	/**
	 * @brief The root view to interact with window system and manage
	 * the interface
	 *
	 * Context is a special view in BlendInt, it provide public
	 * entries for the windows system to control the display and input
	 * events.
	 *
	 * There should be at least one Context object to work with
	 * Interface to show and dispatch events.
	 */
	class Context: public AbstractView
	{
		DISALLOW_COPY_AND_ASSIGN(Context);

	public:

		static bool Initialize ();

		static void Release ();

		Context ();

		virtual ~Context ();

		bool AddFrame (AbstractFrame* frame, bool focus = true);

		bool InsertFrame (int index, AbstractFrame* frame, bool focus = true);

		void MoveFrameToTop (AbstractFrame* frame, bool focus = true);

		void Draw ();

		void DispatchKeyEvent (KeyAction action,
							   int key,
							   int modifier,
							   int scancode,
							   String text);

		void DispatchMouseEvent (int x,
								 int y,
								 MouseAction action,
								 MouseButton button,
								 int modifier);

		/**
		 * @brief Always return true
		 */
		virtual bool Contain (const Point& point) const;

		Cpp::EventRef<const Size&> resized ()
		{
			return resized_;
		}

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual ResponseType Draw (AbstractWindow* context);

		virtual void PostDraw (AbstractWindow* context);

		virtual void PerformFocusOn (AbstractWindow* context);

		virtual void PerformFocusOff (AbstractWindow* context);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual ResponseType PerformKeyPress (AbstractWindow* context);

		virtual ResponseType PerformContextMenuPress (AbstractWindow* context);

		virtual ResponseType PerformContextMenuRelease (AbstractWindow* context);

		virtual ResponseType PerformMousePress (AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (AbstractWindow* context);

		virtual ResponseType PerformMouseMove (AbstractWindow* context);

		virtual bool RemoveSubView (AbstractView* view);

		Cpp::ConnectionScope* events() const {return events_.get();}

	private:

		friend class AbstractFrame;
		friend class AbstractWidget;

		void InitializeContext ();

		void DispatchHoverEvent ();

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		Cpp::Event<const Size&> resized_;

		static std::set<Context*> context_set;

		static glm::mat4 default_view_matrix;
	};

}

#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
