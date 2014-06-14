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

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Rect.hpp>

#include <BlendInt/Core/RefPtr.hpp>
#include <BlendInt/Window/MouseEvent.hpp>
#include <BlendInt/Window/KeyEvent.hpp>
#include <BlendInt/Window/ContextMenuEvent.hpp>
#include <BlendInt/Window/RedrawEvent.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Gui/Shadow.hpp>

namespace BlendInt {

	class Context;
	class GLTexture2D;
	class AbstractWidget;
	class AbstractContainer;

	struct WidgetTheme;

	enum WidgetGeometryType {
		WidgetPosition,
		WidgetSize,
		WidgetRoundCornerType,
		WidgetRoundCornerRadius,
		WidgetLayer,
		WidgetVisibility
	};

	enum ContainerRequestType {
		ContainerMargin,
		ContainerRefresh
	};

	typedef RefPtr<AbstractWidget> AbstractWidgetPtr;

	template<typename T>
	T* Manage (T* obj, bool val = true)
	{
		obj->set_manage(val);
		return obj;
	}

	class WidgetUpdateRequest: public UpdateRequest
	{
	public:

		WidgetUpdateRequest (AbstractWidget* source, AbstractWidget* target)
		: UpdateRequest(0, 0),
		  m_source(source),
		  m_target(target)
		{

		}

		WidgetUpdateRequest (AbstractWidget* source, AbstractWidget* target,
						int type, const void* data)
		: UpdateRequest(type, data),
		  m_source(source), m_target(target)
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

		WidgetUpdateRequest();

		AbstractWidget* m_source;

		AbstractWidget* m_target;
	};

	/**
	 * @brief Proxy class to be used in container to set its sub widget property
	 */
	class SubWidgetProxy
	{
	private:
		friend class AbstractContainer;

		SubWidgetProxy(AbstractWidget* source, AbstractWidget* target);

		~SubWidgetProxy ();

		void Resize (AbstractWidget* widget, const Size& size);

		void Resize (AbstractWidget* widget, int width, int height);

		void SetPosition (AbstractWidget* widget, int x, int y);

		void SetPosition (AbstractWidget* widget, const Point& position);

		WidgetUpdateRequest m_request;
	};

	/**
	 * @brief Proxy class to be used in sub widget to set its container property
	 */
	class ContainerProxy
	{
	private:
		friend class AbstractWidget;

		explicit ContainerProxy (AbstractWidget* source, AbstractWidget* target);

		~ContainerProxy ();

		void RequestRefresh (AbstractContainer* container);

		bool SubwidgetPositionUpdateTest (AbstractContainer* container, const Point& pos);

		bool SubWidgetSizeUpdateTest (AbstractContainer* container, const Size& size);

		void SubWidgetPositionUpdate (AbstractContainer* container, const Point& pos);

		void SubWidgetSizeUpdate (AbstractContainer* container, const Size& size);

		WidgetUpdateRequest m_request;
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

		void SetRoundCornerRadius (int radius);

		void SetLayer (int z);

		void SetVisible (bool visible);

		virtual bool IsExpandX () const
		{
			return false;
		}

		virtual bool IsExpandY () const
		{
			return false;
		}

		virtual bool Contain (const Point& point) const;

		virtual bool Contain (int x, int y) const;

		void RenderToTexture (size_t border, GLTexture2D* texture);

		void RenderToFile (const char* filename, unsigned int border = 10);

		void Refresh ();

		const Size& size () const
		{
			return m_size;
		}

		const int& layer () const
		{
			return m_z;
		}

		const int& z () const
		{
			return m_z;
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

		int round_corner_type () const
		{
			return m_flags & 0x0F;
		}

		int round_corner_radius () const
		{
			return m_round_corner_radius;
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

		static void SetDefaultBorderWidth (int border);

		static int DefaultBorderWidth ();

		void DrawTriangleFan (const GLint attrib, const GLArrayBuffer* buffer);

		void DrawShadedTriangleFan (const GLint coord, const GLint color, GLArrayBuffer* buffer);

		void DrawTriangleStrip (const GLint attrib, GLArrayBuffer* buffer);

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
		inline void set_size (unsigned int width, unsigned int height)
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

		virtual bool UpdateGeometryTest (const WidgetUpdateRequest& request) = 0;

		/**
		 * @brief Update opengl data (usually the GL buffer) for Render
		 * @param[in] type the enumeration defined by each form class, e.g.
		 * FormPropertySize
		 * @param[in] data the pointer to the new property data
		 *
		 * This virtual function should be implemented in each derived class,
		 * and should only use the form's property to draw opengl elements once.
		 */
		virtual void UpdateGeometry (const WidgetUpdateRequest& request) = 0;

		virtual void BroadcastUpdate (const WidgetUpdateRequest& request) = 0;

		virtual ResponseType Draw (const RedrawEvent& event) = 0;

		bool ResizeTestInContainer (const Size& size);

		bool PositionTestInContainer (const Point& point);

		void ResizeUpdateInContainer (const Size& size);

		void PositionUpdateInContainer (const Point& point);

		Context* GetContext ();

		int GetOutlineVertices () const;

		/**
		 * @brief Used to get emboss vertices
		 * @return
		 */
		int GetHalfOutlineVertices () const;

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

		void set_scissor_test (bool status)
		{
			if(status) {
				SETBIT(m_flags, WidgetFlagScissorTest);
			} else {
				CLRBIT(m_flags, WidgetFlagScissorTest);
			}
		}

		void set_round_corner_type (int type)
		{
			m_flags = (m_flags & 0xFFF0) + (type & 0x0F);
		}

		void set_round_corner_radius (int radius)
		{
			m_round_corner_radius = radius;
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

		static void DispatchRender (AbstractWidget* obj);

		static ResponseType dispatch_key_press_event (AbstractWidget* obj, const KeyEvent& event);

		static ResponseType dispatch_mouse_move_event (AbstractWidget* obj, const MouseEvent& event);

		static ResponseType dispatch_mouse_press_event (AbstractWidget* obj, const MouseEvent& event);

		static ResponseType dispatch_mouse_release_event (AbstractWidget* obj, const MouseEvent& event);

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

			WidgetFlagDropShadow = (1 << 10)

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

		/**
		 * @brief the depth(layer) of the widget
		 */
		int m_z;

		unsigned int m_flags;

		int m_round_corner_radius;

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		Cpp::Event<AbstractWidget*> m_destroyed;

		AbstractContainer* m_container;

		RefPtr<Shadow> m_shadow;

		static int default_border_width;

	};

} /* namespace BlendInt */
#endif /* _BLENDINT_GUI_ABSTRACTWIDGET_HPP_ */
