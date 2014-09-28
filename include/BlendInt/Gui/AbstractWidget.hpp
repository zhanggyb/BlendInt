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

#ifndef _BLENDINT_GUI_ABSTRACTWIDGET_HPP_
#define _BLENDINT_GUI_ABSTRACTWIDGET_HPP_

#include <stack>
#include <vector>

#include <boost/smart_ptr.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/RefPtr.hpp>
#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Rect.hpp>
#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/Window/MouseEvent.hpp>
#include <BlendInt/Window/KeyEvent.hpp>
#include <BlendInt/Window/ContextMenuEvent.hpp>

#include <BlendInt/Gui/Profile.hpp>
#include <BlendInt/Gui/Shadow.hpp>

namespace BlendInt {

	class AbstractWidget;

	typedef RefPtr<AbstractWidget> AbstractWidgetPtr;

	template<typename T>
	T* Manage (T* obj, bool val = true)
	{
		obj->set_manage(val);
		return obj;
	}

	/**
	 * @brief Check if a widget is contained in a container
	 */
	extern bool IsContained (AbstractWidget* container, AbstractWidget* widget);

	class WidgetUpdateRequest
	{
	public:

		WidgetUpdateRequest (AbstractWidget* source, AbstractWidget* target)
		: m_source(source),
		  m_target(target)
		{

		}

		~WidgetUpdateRequest ()
		{

		}

		AbstractWidget* source () const
		{
			return m_source;
		}

		AbstractWidget* target () const
		{
			return m_target;
		}

	private:

		/**
		 * Disabled
		 */
		WidgetUpdateRequest();

		AbstractWidget* m_source;
		AbstractWidget* m_target;
	};

	class SizeUpdateRequest: public WidgetUpdateRequest
	{
	public:

		SizeUpdateRequest (AbstractWidget* source, AbstractWidget* target)
		: WidgetUpdateRequest(source, target), m_size(0)
		{
		}

		SizeUpdateRequest (AbstractWidget* source, AbstractWidget* target, const Size* size)
		: WidgetUpdateRequest(source, target), m_size(size)
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

	class PositionUpdateRequest: public WidgetUpdateRequest
	{
	public:

		PositionUpdateRequest (AbstractWidget* source, AbstractWidget* target)
		: WidgetUpdateRequest(source, target), m_position(0)
		{
		}

		PositionUpdateRequest (AbstractWidget* source, AbstractWidget* target, const Point* pos)
		: WidgetUpdateRequest(source, target), m_position(pos)
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

	class RoundTypeUpdateRequest: public WidgetUpdateRequest
	{
	public:

		RoundTypeUpdateRequest (AbstractWidget* source, AbstractWidget* target)
		: WidgetUpdateRequest(source, target), m_round_type(0)
		{
		}

		RoundTypeUpdateRequest (AbstractWidget* source, AbstractWidget* target, const int* pos)
		: WidgetUpdateRequest(source, target), m_round_type(pos)
		{

		}

		const int* round_type() const
		{
			return m_round_type;
		}

		void set_round_type (const int* type)
		{
			m_round_type = type;
		}

	private:

		const int* m_round_type;
	};

	class RoundRadiusUpdateRequest: public WidgetUpdateRequest
	{
	public:

		RoundRadiusUpdateRequest (AbstractWidget* source, AbstractWidget* target)
		: WidgetUpdateRequest(source, target), m_round_radius(0)
		{
		}

		RoundRadiusUpdateRequest (AbstractWidget* source, AbstractWidget* target, const float* radius)
		: WidgetUpdateRequest(source, target), m_round_radius(radius)
		{

		}

		const float* round_radius() const
		{
			return m_round_radius;
		}

		void set_round_radius (const float* radius)
		{
			m_round_radius = radius;
		}

	private:

		const float* m_round_radius;
	};

	class VisibilityUpdateRequest: public WidgetUpdateRequest
	{
	public:

		VisibilityUpdateRequest (AbstractWidget* source, AbstractWidget* target)
		: WidgetUpdateRequest(source, target), m_visibility(0)
		{

		}

		VisibilityUpdateRequest (AbstractWidget* source, AbstractWidget* target, const bool* visibility)
		: WidgetUpdateRequest(source, target), m_visibility (visibility)
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

	class RefreshRequest: public WidgetUpdateRequest
	{
	public:

		RefreshRequest (AbstractWidget* source, AbstractWidget* target)
		: WidgetUpdateRequest(source, target)
		{

		}

		~RefreshRequest ()
		{

		}
	};

	// ----------------------------------------------------

	/**
	 * @brief The base abstract class for widgets
	 *
	 * @ingroup gui
	 */
	class AbstractWidget: public Object
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractWidget);

	public:

		friend class Context;
		friend class Section;
		friend class AbstractScreen;

		template <typename T> friend T* Manage (T* obj, bool val);

		/**
		 * @brief The default constructor
		 */
		AbstractWidget ();

		/**
		 * @brief Destructor
		 */
		virtual ~AbstractWidget ();

		Point GetGlobalPosition () const;

		virtual Size GetPreferredSize () const;

		/**
		 * @brief Resize the widget
		 * @param[in] width The new width of the widget
		 * @param[in] height The new height of the widget
		 *
		 * Call Update() to check the parameters, if valid, resize the
		 * widget.
		 */
		void Resize (int width, int height);

		/**
		 * @brief Resize the widget
		 * @param[in] size The new size of the widget
		 *
		 * Call Update() to check the parameters, if valid, resize the
		 * widget.
		 */
		void Resize (const Size& size);

		void SetPosition (int x, int y);

		void SetPosition (const Point& pos);

		void SetRoundType (int type);

		void SetRoundRadius (float radius);

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

		void Refresh ();

		AbstractWidget* operator [] (int i) const;

		AbstractWidget* GetWidgetAt (int i) const;

		void MoveBackward ();

		void MoveForward ();

		void MoveToFirst ();

		void MoveToLast ();

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

		void activate_events ()
		{
			SETBIT(flags_, WidgetFlagFireEvents);
		}

		void deactivate_events ()
		{
			CLRBIT(flags_, WidgetFlagFireEvents);
		}

		bool fire_events () const
		{
			return flags_ & WidgetFlagFireEvents;
		}

		inline bool focus () const
		{
			return flags_ & WidgetFlagFocus;
		}

		inline bool hover () const
		{
			return flags_ & WidgetFlagHover;
		}

		inline bool visiable () const
		{
			return flags_ & WidgetFlagVisibility;
		}

		inline bool managed () const
		{
			return flags_ & WidgetFlagManaged;
		}

		inline bool emboss () const
		{
			return flags_ & WidgetFlagEmboss;
		}

		inline int round_type () const
		{
			return flags_ & 0x0F;
		}

		inline float round_radius () const
		{
			return round_radius_;
		}

		inline bool refresh () const
		{
			return flags_ & WidgetFlagRefresh;
		}

		inline int subs_count () const
		{
			return subs_count_;
		}

		inline AbstractWidget* parent() const
		{
			return parent_;
		}

		inline AbstractWidget* previous () const
		{
			return previous_;
		}

		inline AbstractWidget* next () const
		{
			return next_;
		}

		inline AbstractWidget* first_child () const
		{
			return first_child_;
		}

		inline AbstractWidget* last_child() const
		{
			return last_child_;
		}

		Cpp::EventRef<AbstractWidget*> destroyed ()
		{
			return *destroyed_;
		}

		/**
		 * @brief Check if the widget and its all container are under cursor position
		 *
		 * @note There's no meaning to use this function to test Context or Section.
		 */
		static bool IsHoverThrough (const AbstractWidget* widget, const Point& cursor);

		static bool IsHoverThroughExt (const AbstractWidget* widget, const Point& global_cursor_position);

		static void SetDefaultBorderWidth (float border);

		static float DefaultBorderWidth ();

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
		 * @brief preset the position of this widget
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
		 * @brief preset the position of this widget
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

		inline void set_round_radius (float radius)
		{
			round_radius_ = radius;
		}

		inline void set_focus (bool focus)
		{
			if(focus) {
				SETBIT(flags_, WidgetFlagFocus);
			} else {
				CLRBIT(flags_, WidgetFlagFocus);
			}
		}

		inline void set_hover (bool hover)
		{
			if(hover) {
				SETBIT(flags_, WidgetFlagHover);
			} else {
				CLRBIT(flags_, WidgetFlagHover);
			}
		}

		inline void set_visible (bool visiable)
		{
			if(visiable) {
				SETBIT(flags_, WidgetFlagVisibility);
			} else {
				CLRBIT(flags_, WidgetFlagVisibility);
			}
		}

		inline void set_emboss (bool emboss)
		{
			if (emboss) {
				SETBIT(flags_, WidgetFlagEmboss);
			} else {
				CLRBIT(flags_, WidgetFlagEmboss);
			}
		}

		inline void set_refresh (bool refresh)
		{
			if(refresh) {
				SETBIT(flags_, WidgetFlagRefresh);
			} else {
				CLRBIT(flags_, WidgetFlagRefresh);
			}
		}

		virtual void PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile) = 0;

		virtual void PostDraw (Profile& profile);

		virtual ResponseType FocusEvent (bool focus) = 0;

		virtual ResponseType CursorEnterEvent (bool entered) = 0;

		virtual ResponseType KeyPressEvent (const KeyEvent& event) = 0;

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event) = 0;

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event) = 0;

		virtual ResponseType MousePressEvent (const MouseEvent& event) = 0;

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event) = 0;

		virtual ResponseType MouseMoveEvent (const MouseEvent& event) = 0;

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool RoundTypeUpdateTest (const RoundTypeUpdateRequest& request);

		virtual bool RoundRadiusUpdateTest (const RoundRadiusUpdateRequest& request);

		virtual bool VisibilityUpdateTest (const VisibilityUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual void PerformVisibilityUpdate (const VisibilityUpdateRequest& request);

		virtual void PerformRefresh (const RefreshRequest& request);

		void ReportSizeUpdate (const SizeUpdateRequest& request);

		void ReportPositionUpdate (const PositionUpdateRequest& request);

		void ReportRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		void ReportRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		void ReportVisibilityRequest (const VisibilityUpdateRequest& request);

		void ReportRefresh (const RefreshRequest& request);

		bool PushFrontSubWidget (AbstractWidget* widget);

		bool InsertSubWidget (int index, AbstractWidget* widget);

		bool PushBackSubWidget (AbstractWidget* widget);

		bool RemoveSubWidget (AbstractWidget* widget);

		void ClearSubWidgets ();

		void ResizeSubWidget (AbstractWidget* sub, int width, int height);

		void ResizeSubWidget (AbstractWidget* sub, const Size& size);

		void SetSubWidgetPosition (AbstractWidget* sub, int x, int y);

		void SetSubWidgetPosition (AbstractWidget* sub, const Point& pos);

		void SetSubWidgetRoundType (AbstractWidget* sub, int type);

		void SetSubWidgetRoundRadius (AbstractWidget* sub, float radius);

		void SetSubWidgetVisibility (AbstractWidget* sub, bool visible);

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

		Cpp::ConnectionScope* events() const {return events_.get();}

		void GenerateVertices (
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		void GenerateVertices (
				Orientation shadedir,
				short shadetop,
				short shadedown,
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

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

		static void DispatchDrawEvent (AbstractWidget* widget, Profile& profile);

	private:

		enum WidgetFlagIndex {

			WidgetFlagRoundTopLeft = (1 << 0),

			WidgetFlagRoundTopRight = (1 << 1),

			WidgetFlagRoundBottomRight = (1 << 2),

			WidgetFlagRoundBottomLeft = (1 << 3),

			WidgetFlagManaged = (1 << 4),

			WidgetFlagFireEvents = (1 << 5),

			WidgetFlagFocus = (1 << 6),

			/** If this widget is in cursor hover list in Context */
			WidgetFlagHover = (1 << 7),

			WidgetFlagVisibility = (1 << 8),

			WidgetFlagEmboss = (1 << 9),

			// only valid when use off-screen render in container
			WidgetFlagRefresh = (1 << 10)

		};

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
				SETBIT(flags_, WidgetFlagManaged);
			} else {
				CLRBIT(flags_, WidgetFlagManaged);
			}
		}

		Point position_;

		Point offset_;

		Size size_;

		unsigned int flags_;

		float round_radius_;

		int subs_count_;	// count of sub widgets

		AbstractWidget* parent_;

		AbstractWidget* previous_;

		AbstractWidget* next_;

		/**
		 * @brief The first sub widget
		 */
		AbstractWidget* first_child_;

		AbstractWidget* last_child_;

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractWidget*> > destroyed_;

#ifdef DEBUG
		std::string name_;
#endif

		static float default_border_width;

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

	};

} /* namespace BlendInt */
#endif /* _BLENDINT_GUI_ABSTRACTWIDGET_HPP_ */
