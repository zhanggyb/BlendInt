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
#include <BlendInt/Window/RedrawEvent.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/Gui/Shadow.hpp>

namespace BlendInt {

	class Context;
	class Section;
	//class GLTexture2D;
	class AbstractWidget;
	class AbstractContainer;

	typedef RefPtr<AbstractWidget> AbstractWidgetPtr;

	template<typename T>
	T* Manage (T* obj, bool val = true)
	{
		obj->set_manage(val);
		return obj;
	}

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

		VisibilityUpdateRequest (AbstractWidget* source, AbstractWidget* target) :
				WidgetUpdateRequest(source, target), m_visibility(0)
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

	/**
	 * @brief Proxy class to be used in sub widget to communicate with its container
	 */
	class ContainerProxy
	{
	private:
		friend class AbstractWidget;

		ContainerProxy ();

		~ContainerProxy ();

		static inline bool RequestSizeUpdateTest (AbstractContainer* container, const SizeUpdateRequest& request);

		static inline bool RequestPositionUpdateTest (AbstractContainer* container, const PositionUpdateRequest& request);

		static inline bool RequestRoundTypeUpdateTest (AbstractContainer* container, const RoundTypeUpdateRequest& request);

		static inline bool RequestRoundRadiusUpdateTest (AbstractContainer* container, const RoundRadiusUpdateRequest& request);

		static inline bool RequestVisibilityUpdateTest (AbstractContainer* container, const VisibilityUpdateRequest& request);

		static inline void RequestSizeUpdate (AbstractContainer* container, const SizeUpdateRequest& request);

		static inline void RequestPositionUpdate (AbstractContainer* container, const PositionUpdateRequest& request);

		static inline void RequestRoundTypeUpdate (AbstractContainer* container, const RoundTypeUpdateRequest& request);

		static inline void RequestRoundRadiusUpdate (AbstractContainer* container, const RoundRadiusUpdateRequest& request);

		static inline void RequestVisibilityUpdate (AbstractContainer* container, const VisibilityUpdateRequest& request);

		static inline void RequestRefresh (AbstractContainer* container, const RefreshRequest& request);
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
		friend class AbstractContainer;
		friend class SubWidgetProxy;

		template <typename T> friend T* Manage (T* obj, bool val);

		/**
		 * @brief The default constructor
		 */
		AbstractWidget ();

		/**
		 * @brief Destructor
		 */
		virtual ~AbstractWidget ();

		const Point& position () const
		{
			return m_position;
		}

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

		void SetRoundCornerType (int type);

		void SetRoundCornerRadius (float radius);

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

		const Size& size () const
		{
			return m_size;
		}

		void activate_events ()
		{
			SETBIT(m_flags, WidgetFlagFireEvents);
		}

		void deactivate_events ()
		{
			CLRBIT(m_flags, WidgetFlagFireEvents);
		}

		bool fire_events () const
		{
			return m_flags & WidgetFlagFireEvents;
		}

		inline bool focused () const
		{
			return m_flags & WidgetFlagFocus;
		}

		inline bool hover () const
		{
			return m_flags & WidgetFlagHover;
		}

		inline bool visiable () const
		{
			return m_flags & WidgetFlagVisibility;
		}

		inline bool scissor_test () const
		{
			return m_flags & WidgetFlagScissorTest;
		}

		inline bool managed () const
		{
			return m_flags & WidgetFlagManaged;
		}

		inline bool emboss () const
		{
			return m_flags & WidgetFlagEmboss;
		}

		int round_type () const
		{
			return m_flags & 0x0F;
		}

		float round_radius () const
		{
			return m_round_radius;
		}

		bool drop_shadow () const
		{
			return m_flags & WidgetFlagDropShadow;
		}

		/**
		 * @brief move this object along x axis
		 * @param offset_x
		 */
		inline void move_x (int offset_x)
		{
			SetPosition(position().x() + offset_x, position().y());
		}

		/**
		 * @brief move this object along y axis
		 * @param offset_y
		 */
		inline void move_y (int offset_y)
		{
			SetPosition(position().x(), position().y() + offset_y);
		}

		/**
		 * @brief move this object
		 * @param offset_x
		 * @param offset_y
		 */
		inline void move (int offset_x, int offset_y)
		{
			SetPosition(position().x() + offset_x, position().y() + offset_y);
		}

		Cpp::EventRef<AbstractWidget*> destroyed () {return m_destroyed;}

		AbstractContainer* container() const {return m_container;}

		/**
		 * @brief Check if the widget and its all container are under cursor position
		 *
		 * @note There's no meaning to use this function to test Context or Section.
		 */
		static bool IsHoverThrough (const AbstractWidget* widget, const Point& cursor);

		static void SetDefaultBorderWidth (float border);

		static float DefaultBorderWidth ();

#ifdef DEBUG

		inline void set_name (const char* name)
		{
			m_name = name;
		}

		inline void set_name (const std::string& name)
		{
			m_name = name;
		}

		const std::string& name () const {return m_name;}

#endif

	protected:

		/**
		 * @brief preset the form's position
		 * @param x
		 * @param y
		 *
		 * @note this function should be called only in the constructor of subclass
		 * to set the position without through Update() for performance.
		 */
		void set_position (int x, int y)
		{
			m_position.set_x(x);
			m_position.set_y(y);
		}

		/**
		 * @brief preset the form's position
		 * @param pos
		 *
		 * @note this function should be called only in the constructor of subclass
		 * to set the position without through Update() for performance.
		 */
		void set_position (const Point& pos)
		{
			m_position = pos;
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
			m_size.set_width(width);
			m_size.set_height(height);
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
			m_size = size;
		}

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

		virtual ResponseType Draw (const RedrawEvent& event) = 0;

		void ReportSizeUpdate (const SizeUpdateRequest& request);

		void ReportPositionUpdate (const PositionUpdateRequest& request);

		void ReportRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		void ReportRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		void ReportVisibilityRequest (const VisibilityUpdateRequest& request);

		void ReportRefresh (const RefreshRequest& request);

		static int GetOutlineVertices (int round_type);

		/**
		 * @brief Used to get emboss vertices
		 * @return
		 */
		int GetHalfOutlineVertices (int round_type) const;

		void set_focus (bool focus)
		{
			if(focus) {
				SETBIT(m_flags, WidgetFlagFocus);
			} else {
				CLRBIT(m_flags, WidgetFlagFocus);
			}
		}

		void set_hover (bool hover)
		{
			if(hover) {
				SETBIT(m_flags, WidgetFlagHover);
			} else {
				CLRBIT(m_flags, WidgetFlagHover);
			}
		}

		void set_visible (bool visiable)
		{
			if(visiable) {
				SETBIT(m_flags, WidgetFlagVisibility);
			} else {
				CLRBIT(m_flags, WidgetFlagVisibility);
			}
		}

		void set_emboss (bool emboss)
		{
			if (emboss) {
				SETBIT(m_flags, WidgetFlagEmboss);
			} else {
				CLRBIT(m_flags, WidgetFlagEmboss);
			}
		}

		void set_scissor_test (bool status)
		{
			if(status) {
				SETBIT(m_flags, WidgetFlagScissorTest);
			} else {
				CLRBIT(m_flags, WidgetFlagScissorTest);
			}
		}

		void set_round_type (int type)
		{
			m_flags = (m_flags & 0xFFF0) + (type & 0x0F);
		}

		void set_round_radius (float radius)
		{
			m_round_radius = radius;
		}

		void set_drop_shadow (bool shadow)
		{
			if(shadow) {
				SETBIT(m_flags, WidgetFlagDropShadow);
			} else {
				CLRBIT(m_flags, WidgetFlagDropShadow);
			}
		}

		Cpp::ConnectionScope* events() const {return m_events.get();}

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

			/** If enable scissor test when drawing this the subwidgets, this flag is only workable for container */
			WidgetFlagScissorTest = (1 << 9),

			WidgetFlagDropShadow = (1 << 10),

			WidgetFlagEmboss = (1 << 11)

		};

		void set_manage (bool val)
		{
			if(val) {
				SETBIT(m_flags, WidgetFlagManaged);
			} else {
				CLRBIT(m_flags, WidgetFlagManaged);
			}
		}

		Point m_position;

		Size m_size;

		unsigned int m_flags;

		float m_round_radius;

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		Cpp::Event<AbstractWidget*> m_destroyed;

		AbstractContainer* m_container;

		RefPtr<Shadow> m_shadow;

#ifdef DEBUG
		std::string m_name;
#endif

		static float default_border_width;

	};

} /* namespace BlendInt */
#endif /* _BLENDINT_GUI_ABSTRACTWIDGET_HPP_ */
