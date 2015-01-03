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

#ifndef _BLENDINT_GUI_ABSTRACTVIEW_HPP_
#define _BLENDINT_GUI_ABSTRACTVIEW_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__
 
#include <vector>
#include <pthread.h>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Margin.hpp>

namespace BlendInt {

	class Context;
	class AbstractView;

	template<typename T>
	T* Manage (T* obj, bool val = true)
	{
		obj->set_manage(val);
		return obj;
	}

	/**
	 * @brief Check if a view is contained in a container
	 */
	extern bool IsContained (AbstractView* container, AbstractView* view);

	class GeometryUpdateRequest
	{
	public:

		GeometryUpdateRequest (AbstractView* source, AbstractView* target)
		: m_source(source),
		  m_target(target)
		{

		}

		~GeometryUpdateRequest ()
		{

		}

		AbstractView* source () const
		{
			return m_source;
		}

		AbstractView* target () const
		{
			return m_target;
		}

	private:

		/**
		 * Disabled
		 */
		GeometryUpdateRequest();

		AbstractView* m_source;
		AbstractView* m_target;
	};

	class SizeUpdateRequest: public GeometryUpdateRequest
	{
	public:

		SizeUpdateRequest (AbstractView* source, AbstractView* target)
		: GeometryUpdateRequest(source, target), m_size(0)
		{
		}

		SizeUpdateRequest (AbstractView* source, AbstractView* target, const Size* size)
		: GeometryUpdateRequest(source, target), m_size(size)
		{

		}

		const Size* size() const
		{
			return m_size;
		}

		void set_size (const Size* size)
		{
			m_size = size;
		}

	private:

		const Size* m_size;
	};

	class PositionUpdateRequest: public GeometryUpdateRequest
	{
	public:

		PositionUpdateRequest (AbstractView* source, AbstractView* target)
		: GeometryUpdateRequest(source, target), m_position(0)
		{
		}

		PositionUpdateRequest (AbstractView* source, AbstractView* target, const Point* pos)
		: GeometryUpdateRequest(source, target), m_position(pos)
		{

		}

		const Point* position() const
		{
			return m_position;
		}

		void set_position (const Point* pos)
		{
			m_position = pos;
		}

	private:

		const Point* m_position;
	};

	class VisibilityUpdateRequest: public GeometryUpdateRequest
	{
	public:

		VisibilityUpdateRequest (AbstractView* source, AbstractView* target)
		: GeometryUpdateRequest(source, target), m_visibility(0)
		{

		}

		VisibilityUpdateRequest (AbstractView* source, AbstractView* target, const bool* visibility)
		: GeometryUpdateRequest(source, target), m_visibility (visibility)
		{

		}

		const bool* visibility () const
		{
			return m_visibility;
		}

		void set_visibility (const bool* visibility)
		{
			m_visibility = visibility;
		}

	private:

		const bool* m_visibility;
	};

	// ----------------------------------------------------

	/**
	 * @brief Abstract class for all views
	 *
	 * A view is a unit of the user interface in BlendInt that knows how to draw itself.
	 * A view also knows how to process the event from window system, e.g. a mouse click.
	 *
	 * AbstractView is the basic class for all views, and there're 2 different groups of sub classes:
	 * 	- frames
	 * 	- widgets
	 *
	 * Frame works like a window in the desktop, it usually has a background, shadow, and contains widgets.
	 *
	 * The other group of view are called widgets, for example, buttons, sliders.
	 *
	 * @ingroup gui
	 */
	class AbstractView: public Object
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractView);

	public:

		/**
		 * @brief The default constructor
		 */
		AbstractView ();

		/**
		 * @brief Destructor
		 */
		virtual ~AbstractView ();

		Point GetGlobalPosition () const;

		virtual Size GetPreferredSize () const;

		/**
		 * @brief Resize the view
		 * @param[in] width The new width of the view
		 * @param[in] height The new height of the view
		 *
		 * Call Update() to check the parameters, if valid, resize the
		 * view.
		 */
		void Resize (int width, int height);

		/**
		 * @brief Resize the view
		 * @param[in] size The new size of the view
		 *
		 * Call Update() to check the parameters, if valid, resize the
		 * view.
		 */
		void Resize (const Size& size);

		void MoveTo (int x, int y);

		void MoveTo (const Point& pos);

		void SetRoundType (int type);

		void SetVisible (bool visible);

		void SetEmboss (bool emboss);

		virtual bool IsExpandX () const
		{
			return false;
		}

		virtual bool IsExpandY () const
		{
			return false;
		}

		virtual bool Contain (const Point& point) const;

		void RequestRedraw ();

		AbstractView* operator [] (int i) const;

		AbstractView* GetWidgetAt (int i) const;

		const Point& position () const
		{
			return position_;
		}

		const Point& offset () const
		{
			return offset_;
		}

		const Size& size () const
		{
			return size_;
		}

		inline bool focus () const
		{
			return flags_ & ViewFocused;
		}

		inline bool hover () const
		{
			return flags_ & ViewHover;
		}

		inline bool visiable () const
		{
			return flags_ & ViewVisible;
		}

		inline bool managed () const
		{
			return flags_ & ViewManaged;
		}

		inline bool emboss () const
		{
			return flags_ & ViewEmboss;
		}

		inline int round_type () const
		{
			return flags_ & 0x0F;
		}

		inline bool refresh () const
		{
			return flags_ & ViewRefresh;
		}

		inline bool pressed_ext () const
		{
			return flags_ & ViewPressed;
		}

		inline int subs_count () const
		{
			return subs_count_;
		}

		inline AbstractView* superview() const
		{
			return superview_;
		}

		inline AbstractView* previous_view () const
		{
			return previous_view_;
		}

		inline AbstractView* next_view () const
		{
			return next_view_;
		}

		inline AbstractView* first_subview () const
		{
			return first_subview_;
		}

		inline AbstractView* last_subview () const
		{
			return last_subview_;
		}

		static void MoveToFirst (AbstractView* view);

		static void MoveToLast (AbstractView* view);

		static void MoveForward (AbstractView* view);

		static void MoveBackward (AbstractView* view);

		/**
		 * @brief Check if the view and its all container are under cursor position
		 *
		 * @note There's no meaning to use this function to test Context or Frame.
		 */
		static bool IsHoverThrough (const AbstractView* view, const Point& cursor);

		static bool IsHoverThroughExt (const AbstractView* view, const Point& global_cursor_position);

		static void SetDefaultBorderWidth (float border);

		static inline float default_border_width ()
		{
			return kBorderWidth;
		}

#ifdef DEBUG

		inline void set_name (const char* name)
		{
			name_ = name;
		}

		inline void set_name (const std::string& name)
		{
			name_ = name;
		}

		const std::string& name () const {return name_;}

#endif

	protected:

		/**
		 * @brief preset the position of this view
		 * @param x
		 * @param y
		 *
		 * @note this function should be called only in the constructor of subclass
		 * to set the position without through Update() for performance.
		 */
		void set_position (int x, int y)
		{
			position_.reset(x, y);
		}

		/**
		 * @brief preset the position of this view
		 * @param pos
		 *
		 * @note this function should be called only in the constructor of subclass
		 * to set the position without through Update() for performance.
		 */
		void set_position (const Point& pos)
		{
			position_ = pos;
		}

		void set_offset (int x, int y)
		{
			offset_.reset(x, y);
		}

		void set_offset (const Point& pos)
		{
			offset_ = pos;
		}

		/**
		 * @brief preset the size of the form
		 * @param width
		 * @param height
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (int width, int height)
		{
			size_.reset(width, height);
		}

		/**
		 * @brief preset the size of the form
		 * @param size
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (const Size& size)
		{
			size_ = size;
		}

		inline void set_round_type (int type)
		{
			flags_ = (flags_ & 0xFFF0) + (type & 0x0F);
		}

		inline void set_focusable (bool focusable)
		{
			if(focusable) {
				SETBIT(flags_, ViewFocusable);
			} else {
				CLRBIT(flags_, ViewFocusable);
			}
		}

		inline void set_focus (bool focus)
		{
			if(focus) {
				SETBIT(flags_, ViewFocused);
			} else {
				CLRBIT(flags_, ViewFocused);
			}
		}

		inline void set_hover (bool hover)
		{
			if(hover) {
				SETBIT(flags_, ViewHover);
			} else {
				CLRBIT(flags_, ViewHover);
			}
		}

		inline void set_visible (bool visiable)
		{
			if(visiable) {
				SETBIT(flags_, ViewVisible);
			} else {
				CLRBIT(flags_, ViewVisible);
			}
		}

		inline void set_emboss (bool emboss)
		{
			if (emboss) {
				SETBIT(flags_, ViewEmboss);
			} else {
				CLRBIT(flags_, ViewEmboss);
			}
		}

		inline void set_pressed (bool pressed)
		{
			if(pressed) {
				SETBIT(flags_, ViewPressed);
			} else {
				CLRBIT(flags_, ViewPressed);
			}
		}

		inline void set_refresh (bool refresh)
		{
			if(refresh) {
				SETBIT(flags_, ViewRefresh);
			} else {
				CLRBIT(flags_, ViewRefresh);
			}
		}

		virtual bool PreDraw (const Context* context) = 0;

		virtual ResponseType Draw (const Context* context) = 0;

		virtual void PostDraw (const Context* context) = 0;

		virtual void PerformFocusOn (const Context* context) = 0;

		virtual void PerformFocusOff (const Context* context) = 0;

		virtual void PerformHoverIn (const Context* context) = 0;

		virtual void PerformHoverOut (const Context* context) = 0;

		virtual ResponseType PerformKeyPress (const Context* context) = 0;

		virtual ResponseType PerformContextMenuPress (const Context* context) = 0;

		virtual ResponseType PerformContextMenuRelease (const Context* context) = 0;

		virtual ResponseType PerformMousePress (const Context* context) = 0;

		virtual ResponseType PerformMouseRelease (const Context* context) = 0;

		virtual ResponseType PerformMouseMove (const Context* context) = 0;

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool VisibilityUpdateTest (const VisibilityUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformVisibilityUpdate (const VisibilityUpdateRequest& request);

		void ReportSizeUpdate (const SizeUpdateRequest& request);

		void ReportPositionUpdate (const PositionUpdateRequest& request);

		void ReportVisibilityRequest (const VisibilityUpdateRequest& request);

		bool PushFrontSubView (AbstractView* view);

		bool InsertSubView (int index, AbstractView* view);

		bool PushBackSubView (AbstractView* view);

		virtual bool RemoveSubView (AbstractView* view);

		void ClearSubViews ();

		void ResizeSubView (AbstractView* sub, int width, int height);

		void ResizeSubView (AbstractView* sub, const Size& size);

		void MoveSubViewTo (AbstractView* sub, int x, int y);

		void MoveSubViewTo (AbstractView* sub, const Point& pos);

		void SetSubViewVisibility (AbstractView* sub, bool visible);

		void MoveSubWidgets (int offset_x, int offset_y);

		void ResizeSubWidgets (const Size& size);

		void ResizeSubWidgets (int w, int h);

		void FillSingleWidget (int index, const Size& size, const Margin& margin);

		void FillSingleWidget (int index, const Point& pos, const Size& size);

		void FillSingleWidget (int index, int left, int bottom, int width, int height);

		void FillSubWidgetsAveragely (const Point& out_pos, const Size& out_size,
						const Margin& margin, Orientation orientation,
						int alignment, int space);

		void FillSubWidgetsAveragely (const Point& pos, const Size& size,
						Orientation orientation, int alignment, int space);

		/**
		 * @brief Fill in the container with average size
		 * @param[in] x the left position
		 * @param[in] y the bottom position
		 */
		void FillSubWidgetsAveragely (int x, int y, int width,
						int height, Orientation orientation,
						int alignment, int space);

		/**
		 * @brief Used to get emboss vertices
		 * @return
		 */
		int GetHalfOutlineVertices (int round_type) const;

		void DrawSubViewsOnce (const Context* context);

		static void GenerateVertices (
				const Size& size,
				float border,
				int round_type,
				float radius,
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		static void GenerateVertices (
				const Size& size,
				float border,
				int round_type,
				float radius,
				Orientation shadedir,
				short shadetop,
				short shadedown,
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		static int GetOutlineVertices (int round_type);

	private:

		friend class Context;
		friend class AbstractFrame;
		friend class AbstractWidget;

		template <typename T> friend T* Manage (T* obj, bool val);

		enum ViewFlagIndex {

			ViewRoundTopLeft = (1 << 0),

			ViewRoundTopRight = (1 << 1),

			ViewRoundBottomRight = (1 << 2),

			ViewRoundBottomLeft = (1 << 3),

			ViewManaged = (1 << 4),

			// set this flag when the view or frame is pressed
			ViewPressed = (1 << 5),

			ViewFocusable = (1 << 6),

			ViewFocused = (1 << 7),

			/** If this view is in cursor hover list in Context */
			ViewHover = (1 << 8),

			ViewVisible = (1 << 9),

			ViewEmboss = (1 << 10),

			// only valid when use off-screen render in container
			ViewRefresh = (1 << 11),

		};

		/**
		 * @brief Dispatch draw
		 * @param[in] view
		 * @param[in] profile
		 * @param[in] use_parent_status
		 * 	- true: use superview refresh() status to set view's refresh flag
		 * 	- false: set view's flag to false after Draw()
		 */
		static void DispatchDrawEvent (AbstractView* view, const Context* context);

		static void GenerateTriangleStripVertices (
						const std::vector<GLfloat>* inner,
						const std::vector<GLfloat>* edge,
						unsigned int num,
						std::vector<GLfloat>* strip);

		static inline float make_shaded_offset (short shadetop, short shadedown, float fact);

		void DistributeHorizontally (int x, int width, int space);

		void DistributeVertically (int y, int height, int space);

		void AlignHorizontally (int y, int height, int alignment);

		void AlignVertically (int x, int width, int alignment);

		void set_manage (bool val)
		{
			if(val) {
				SETBIT(flags_, ViewManaged);
			} else {
				CLRBIT(flags_, ViewManaged);
			}
		}

		Point position_;

		Point offset_;

		Size size_;

		unsigned int flags_;

		int subs_count_;	// count of sub widgets

		AbstractView* superview_;

		AbstractView* previous_view_;

		AbstractView* next_view_;

		/**
		 * @brief The first sub view
		 */
		AbstractView* first_subview_;

		AbstractView* last_subview_;

#ifdef DEBUG
		std::string name_;
#endif

		static float kBorderWidth;

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

	};

} /* namespace BlendInt */

#endif /* _BLENDINT_GUI_ABSTRACTVIEW_HPP_ */
