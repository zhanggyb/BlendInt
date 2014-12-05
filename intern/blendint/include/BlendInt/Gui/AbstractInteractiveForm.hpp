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

#ifndef _BLENDINT_GUI_ABSTRACTINTERACTIVEFORM_HPP_
#define _BLENDINT_GUI_ABSTRACTINTERACTIVEFORM_HPP_

#include <vector>
#include <pthread.h>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/HID/MouseEvent.hpp>
#include <BlendInt/HID/KeyEvent.hpp>
#include <BlendInt/HID/ContextMenuEvent.hpp>

#include <BlendInt/Gui/Profile.hpp>

namespace BlendInt {

	class AbstractInteractiveForm;

	typedef RefPtr<AbstractInteractiveForm> AbstractInteractiveFormPtr;

	template<typename T>
	T* Manage (T* obj, bool val = true)
	{
		obj->set_manage(val);
		return obj;
	}

	/**
	 * @brief Check if a widget is contained in a container
	 */
	extern bool IsContained (AbstractInteractiveForm* container, AbstractInteractiveForm* widget);

	class InteractiveFormUpdateRequest
	{
	public:

		InteractiveFormUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target)
		: m_source(source),
		  m_target(target)
		{

		}

		~InteractiveFormUpdateRequest ()
		{

		}

		AbstractInteractiveForm* source () const
		{
			return m_source;
		}

		AbstractInteractiveForm* target () const
		{
			return m_target;
		}

	private:

		/**
		 * Disabled
		 */
		InteractiveFormUpdateRequest();

		AbstractInteractiveForm* m_source;
		AbstractInteractiveForm* m_target;
	};

	class SizeUpdateRequest: public InteractiveFormUpdateRequest
	{
	public:

		SizeUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target)
		: InteractiveFormUpdateRequest(source, target), m_size(0)
		{
		}

		SizeUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target, const Size* size)
		: InteractiveFormUpdateRequest(source, target), m_size(size)
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

	class PositionUpdateRequest: public InteractiveFormUpdateRequest
	{
	public:

		PositionUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target)
		: InteractiveFormUpdateRequest(source, target), m_position(0)
		{
		}

		PositionUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target, const Point* pos)
		: InteractiveFormUpdateRequest(source, target), m_position(pos)
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

	class RoundTypeUpdateRequest: public InteractiveFormUpdateRequest
	{
	public:

		RoundTypeUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target)
		: InteractiveFormUpdateRequest(source, target), m_round_type(0)
		{
		}

		RoundTypeUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target, const int* pos)
		: InteractiveFormUpdateRequest(source, target), m_round_type(pos)
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

	class RoundRadiusUpdateRequest: public InteractiveFormUpdateRequest
	{
	public:

		RoundRadiusUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target)
		: InteractiveFormUpdateRequest(source, target), m_round_radius(0)
		{
		}

		RoundRadiusUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target, const float* radius)
		: InteractiveFormUpdateRequest(source, target), m_round_radius(radius)
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

	class VisibilityUpdateRequest: public InteractiveFormUpdateRequest
	{
	public:

		VisibilityUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target)
		: InteractiveFormUpdateRequest(source, target), m_visibility(0)
		{

		}

		VisibilityUpdateRequest (AbstractInteractiveForm* source, AbstractInteractiveForm* target, const bool* visibility)
		: InteractiveFormUpdateRequest(source, target), m_visibility (visibility)
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
	 * @brief The base abstract class for widgets
	 *
	 * @ingroup gui
	 */
	class AbstractInteractiveForm: public Object
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractInteractiveForm);

	public:

		friend class Context;
		friend class AbstractFrame;

		template <typename T> friend T* Manage (T* obj, bool val);

		/**
		 * @brief The default constructor
		 */
		AbstractInteractiveForm ();

		/**
		 * @brief Destructor
		 */
		virtual ~AbstractInteractiveForm ();

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

		void RequestRedrawInThread ();

		bool TryRequestRedrawInThread ();

		AbstractInteractiveForm* operator [] (int i) const;

		AbstractInteractiveForm* GetWidgetAt (int i) const;

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

		inline bool focus () const
		{
			return flags_ & IFFocused;
		}

		inline bool hover () const
		{
			return flags_ & IFHover;
		}

		inline bool visiable () const
		{
			return flags_ & IFVisibility;
		}

		inline bool managed () const
		{
			return flags_ & IFManaged;
		}

		inline bool emboss () const
		{
			return flags_ & IFEmboss;
		}

		inline int round_type () const
		{
			return flags_ & 0x0F;
		}

		inline bool refresh () const
		{
			return flags_ & IFRefresh;
		}

		inline bool pressed_ext () const
		{
			return flags_ & IFPressed;
		}

		inline int subs_count () const
		{
			return subs_count_;
		}

		inline AbstractInteractiveForm* parent() const
		{
			return parent_;
		}

		inline AbstractInteractiveForm* previous () const
		{
			return previous_;
		}

		inline AbstractInteractiveForm* next () const
		{
			return next_;
		}

		inline AbstractInteractiveForm* first_child () const
		{
			return first_child_;
		}

		inline AbstractInteractiveForm* last_child() const
		{
			return last_child_;
		}

		/**
		 * @brief Check if the widget and its all container are under cursor position
		 *
		 * @note There's no meaning to use this function to test Context or Frame.
		 */
		static bool IsHoverThrough (const AbstractInteractiveForm* widget, const Point& cursor);

		static bool IsHoverThroughExt (const AbstractInteractiveForm* widget, const Point& global_cursor_position);

		static void SetDefaultBorderWidth (float border);

		static inline float default_border_width ()
		{
			return border_width;
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

		inline void set_focus (bool focus)
		{
			if(focus) {
				SETBIT(flags_, IFFocused);
			} else {
				CLRBIT(flags_, IFFocused);
			}
		}

		inline void set_hover (bool hover)
		{
			if(hover) {
				SETBIT(flags_, IFHover);
			} else {
				CLRBIT(flags_, IFHover);
			}
		}

		inline void set_visible (bool visiable)
		{
			if(visiable) {
				SETBIT(flags_, IFVisibility);
			} else {
				CLRBIT(flags_, IFVisibility);
			}
		}

		inline void set_emboss (bool emboss)
		{
			if (emboss) {
				SETBIT(flags_, IFEmboss);
			} else {
				CLRBIT(flags_, IFEmboss);
			}
		}

		inline void set_pressed (bool pressed)
		{
			if(pressed) {
				SETBIT(flags_, IFPressed);
			} else {
				CLRBIT(flags_, IFPressed);
			}
		}

		inline void set_refresh (bool refresh)
		{
			if(refresh) {
				SETBIT(flags_, IFRefresh);
			} else {
				CLRBIT(flags_, IFRefresh);
			}
		}

		virtual bool PreDraw (Profile& profile) = 0;

		virtual ResponseType Draw (Profile& profile) = 0;

		virtual void PostDraw (Profile& profile) = 0;

		virtual void FocusEvent (bool focus) = 0;

		virtual void MouseHoverInEvent (const MouseEvent& event) = 0;

		virtual void MouseHoverOutEvent (const MouseEvent& event) = 0;

		virtual ResponseType KeyPressEvent (const KeyEvent& event) = 0;

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event) = 0;

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event) = 0;

		virtual ResponseType MousePressEvent (const MouseEvent& event) = 0;

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event) = 0;

		virtual ResponseType MouseMoveEvent (const MouseEvent& event) = 0;

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

		bool PushFrontSubForm (AbstractInteractiveForm* widget);

		bool InsertSubForm (int index, AbstractInteractiveForm* widget);

		bool PushBackSubForm (AbstractInteractiveForm* widget);

		bool RemoveSubForm (AbstractInteractiveForm* widget);		// TODO: use virtual

		void ClearSubForms ();

		void ResizeSubForm (AbstractInteractiveForm* sub, int width, int height);

		void ResizeSubForm (AbstractInteractiveForm* sub, const Size& size);

		void MoveSubFormTo (AbstractInteractiveForm* sub, int x, int y);

		void MoveSubFormTo (AbstractInteractiveForm* sub, const Point& pos);

		void SetSubFormVisibility (AbstractInteractiveForm* sub, bool visible);

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

		void DrawSubFormsOnce (Profile& profile);

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

		enum InteractiveFormFlagIndex {

			IFRoundTopLeft = (1 << 0),

			IFRoundTopRight = (1 << 1),

			IFRoundBottomRight = (1 << 2),

			IFRoundBottomLeft = (1 << 3),

			IFManaged = (1 << 4),

			// set this flag when the widget or frame is pressed
			IFPressed = (1 << 5),

			IFFocused = (1 << 6),

			/** If this widget is in cursor hover list in Context */
			IFHover = (1 << 7),

			IFVisibility = (1 << 8),

			IFEmboss = (1 << 9),

			// only valid when use off-screen render in container
			IFRefresh = (1 << 10),

		};

		/**
		 * @brief Dispatch draw
		 * @param[in] widget
		 * @param[in] profile
		 * @param[in] use_parent_status
		 * 	- true: use parent refresh() status to set widget's refresh flag
		 * 	- false: set widget's flag to false after Draw()
		 */
		static void DispatchDrawEvent (AbstractInteractiveForm* widget, Profile& profile);

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
				SETBIT(flags_, IFManaged);
			} else {
				CLRBIT(flags_, IFManaged);
			}
		}

		Point position_;

		Point offset_;

		Size size_;

		unsigned int flags_;

		int subs_count_;	// count of sub widgets

		AbstractInteractiveForm* parent_;

		AbstractInteractiveForm* previous_;

		AbstractInteractiveForm* next_;

		/**
		 * @brief The first sub widget
		 */
		AbstractInteractiveForm* first_child_;

		AbstractInteractiveForm* last_child_;

#ifdef DEBUG
		std::string name_;
#endif

		static pthread_mutex_t refresh_mutex;

		static float border_width;

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

	};

} /* namespace BlendInt */

#endif /* _BLENDINT_GUI_ABSTRACTINTERACTIVEFORM_HPP_ */
