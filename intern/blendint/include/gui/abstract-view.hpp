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

#pragma once

#include <vector>
#include <boost/thread.hpp>

#include <opengl/opengl.hpp>

#include <core/types.hpp>
#include <core/object.hpp>
#include <core/point.hpp>
#include <core/size.hpp>
#include <core/margin.hpp>
#include <core/mutex.hpp>

#include <cppevents/Cpp/Events.hpp>

namespace BlendInt {

  class AbstractWindow;
  class AbstractView;

  enum ViewFlagsMask {

    ViewManageMask = 0x1 << 0,

    ViewVisibleMask = 0x1 << 1,

    ViewRefreshMask = 0x1 << 2,

    ViewTypeMask = 0x07 << 3

  };

  enum ViewType {

    ViewTypeUndefined = 0x0,

    ViewTypeWindow = (ViewTypeUndefined + 1) << 3,  // 0x08

    ViewTypeFrame = (ViewTypeUndefined + 2) << 3,   // 0x10

    ViewTypeWidget = (ViewTypeUndefined + 3) << 3,  // 0x18

    ViewTypeNode = (ViewTypeUndefined + 4) << 3     // 0x20

  };

  /**
   * @brief Set/reset the managed flag of a View
   *
   * A managed View will be deleted when the superview is destroyed.
   */
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
        : m_source(source), m_target(target)
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
    GeometryUpdateRequest ();

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

    SizeUpdateRequest (AbstractView* source,
                       AbstractView* target,
                       const Size* size)
        : GeometryUpdateRequest(source, target), m_size(size)
    {

    }

    const Size* size () const
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

    PositionUpdateRequest (AbstractView* source,
                           AbstractView* target,
                           const Point* pos)
        : GeometryUpdateRequest(source, target), m_position(pos)
    {

    }

    const Point* position () const
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

    VisibilityUpdateRequest (AbstractView* source,
                             AbstractView* target,
                             const bool* visibility)
        : GeometryUpdateRequest(source, target), m_visibility(visibility)
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
   * AbstractView is the base class of all user interface objects in
   * BlendInt, it receives mouse, keyboard and other events from the
   * window system, and draw a representation of itself with OpenGL
   * APIs in the OpenGL Context.
   *
   * There're 2 main different groups of the sub classes of
   * AbstractView:
   *
   *	- frames
   *	- widgets
   *
   * A Frame inherit from AbstractFrame and works like a window in
   * the desktop, it's used to contains other widgets or other
   * frames, dispatch render or input events, and usually manages
   * its own projection/view/model matrix to display its sub views.
   *
   * The other group is called widget. All widgets inherit from
   * AbstractWidget. A widget must be contained in a frame for
   * display and interact with the user.
   *
   * @see AbstractFrame
   * @see AbstractWidget
   * @see Context
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

    AbstractView (int width, int height);

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

    void SetVisible (bool visible);

    void RequestRedraw ();

    AbstractView* operator [] (int i) const;

    AbstractView* GetSubViewAt (int i) const;

    inline const Point& position () const
    {
      return position_;
    }

    virtual bool IsExpandX () const
    {
      return false;
    }

    virtual bool IsExpandY () const
    {
      return false;
    }

    virtual bool Contain (const Point& point) const;

    // always return (0, 0) except AbstractScrollable
    virtual Point GetOffset () const
    {
      return Point(0, 0);
    }

    inline const Size& size () const
    {
      return size_;
    }

    inline bool visiable () const
    {
      return view_flag_ & ViewVisibleMask;
    }

    inline bool managed () const
    {
      return view_flag_ & ViewManageMask;
    }

    inline bool refresh () const
    {
      return view_flag_ & ViewRefreshMask;
    }

    inline int subs_count () const
    {
      return subs_count_;
    }

    inline AbstractView* superview () const
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

    static void SetDefaultBorderWidth (float border);

    static inline bool is_window (const AbstractView* view)
    {
      return view ? (view->view_flag_ & ViewTypeMask) == ViewTypeWindow : false;
    }

    static inline bool is_frame (const AbstractView* view)
    {
      return view ? (view->view_flag_ & ViewTypeMask) == ViewTypeFrame : false;
    }

    static inline bool is_widget (const AbstractView* view)
    {
      return view ? (view->view_flag_ & ViewTypeMask) == ViewTypeWidget : false;
    }

    static inline bool is_node (const AbstractView* view)
    {
      return view ? (view->view_flag_ & ViewTypeMask) == ViewTypeNode : false;
    }

    static inline bool is_undefined_type (const AbstractView* view)
    {
      return view ? (view->view_flag_ & ViewTypeMask) == ViewTypeUndefined : false;
    }

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

    inline const std::string& name () const
    {
      return name_;
    }

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

    inline void set_visible (bool visiable)
    {
      if (visiable)
        SETBIT(view_flag_, ViewVisibleMask);
      else
        CLRBIT(view_flag_, ViewVisibleMask);
    }

    inline void set_refresh (bool refresh)
    {
      if (refresh)
        SETBIT(view_flag_, ViewRefreshMask);
      else
        CLRBIT(view_flag_, ViewRefreshMask);
    }

    virtual bool PreDraw (AbstractWindow* context) = 0;

    virtual Response Draw (AbstractWindow* context) = 0;

    virtual void PostDraw (AbstractWindow* context) = 0;

    virtual void PerformFocusOn (AbstractWindow* context) = 0;

    virtual void PerformFocusOff (AbstractWindow* context) = 0;

    virtual void PerformHoverIn (AbstractWindow* context) = 0;

    virtual void PerformHoverOut (AbstractWindow* context) = 0;

    virtual Response PerformKeyPress (AbstractWindow* context) = 0;

    virtual Response PerformContextMenuPress (AbstractWindow* context) = 0;

    virtual Response PerformContextMenuRelease (AbstractWindow* context) = 0;

    virtual Response PerformMousePress (AbstractWindow* context) = 0;

    virtual Response PerformMouseRelease (AbstractWindow* context) = 0;

    virtual Response PerformMouseMove (AbstractWindow* context) = 0;

    virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

    virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

    virtual bool VisibilityUpdateTest (const VisibilityUpdateRequest& request);

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

    virtual void PerformVisibilityUpdate (const VisibilityUpdateRequest& request);

    bool PushFrontSubView (AbstractView* view);

    bool InsertSubView (int index, AbstractView* view);

    bool PushBackSubView (AbstractView* view);

    virtual bool RemoveSubView (AbstractView* view);

    virtual void PerformAfterAdded ();

    virtual void PerformBeforeRemoved ();

    void ClearSubViews ();

    void ReportSizeUpdate (const SizeUpdateRequest& request);

    void ReportPositionUpdate (const PositionUpdateRequest& request);

    void ReportVisibilityRequest (const VisibilityUpdateRequest& request);

    void ResizeSubView (AbstractView* sub, int width, int height);

    void ResizeSubView (AbstractView* sub, const Size& size);

    void MoveSubViewTo (AbstractView* sub, int x, int y);

    void MoveSubViewTo (AbstractView* sub, const Point& pos);

    void SetSubViewVisibility (AbstractView* sub, bool visible);

    Response RecursiveDispatchKeyEvent (AbstractView* view,
                                        AbstractWindow* context);

    AbstractView* RecursiveDispatchMousePress (AbstractView* view,
                                               AbstractWindow* context);

    Response RecursiveDispatchMouseMoveEvent (AbstractView* view,
                                              AbstractWindow* context);

    Response RecursiveDispatchMouseReleaseEvent (AbstractView* view,
                                                 AbstractWindow* context);

    void DrawSubViewsOnce (AbstractWindow* context);

    /**
     * @brief Swap 2 views' position in their superview
     *
     * The 2 views must have the same superview, otherwise, this function just retern false and do nothing.
     */
    static bool SwapIndex (AbstractView *view1, AbstractView *view2);

    static bool InsertSiblingBefore (AbstractView* src, AbstractView* dst);

    static bool InsertSiblingAfter (AbstractView* src, AbstractView* dst);

    static void GenerateVertices (const Size& size,
                                  float border,
                                  int round_type,
                                  float radius,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    static void GenerateVertices (const Size& size,
                                  float border,
                                  int round_type,
                                  float radius,
                                  Orientation shadedir,
                                  short shadetop,
                                  short shadedown,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    static int GetOutlineVertices (int round_type);

    /**
     * @brief Used to get emboss vertices
     * @return
     */
    static int GetHalfOutlineVertices (int round_type);

  private:

    friend class AbstractWindow;
    friend class AbstractFrame;
    friend class AbstractWidget;
    friend class AbstractNode;
    friend class AbstractAdjustment;

    template<typename T> friend T* Manage (T* obj, bool val);

    /**
     * @brief Dispatch draw
     * @param[in] view
     * @param[in] profile
     * @param[in] use_parent_status
     * 	- true: use superview refresh() status to set view's refresh flag
     * 	- false: set view's flag to false after Draw()
     */
    static void DispatchDrawEvent (AbstractView* view, AbstractWindow* context);

    static void GenerateTriangleStripVertices (const std::vector<GLfloat>* inner,
                                               const std::vector<GLfloat>* edge,
                                               unsigned int num,
                                               std::vector<GLfloat>* strip);

    static inline float make_shaded_offset (short shadetop,
                                            short shadedown,
                                            float fact);

    inline void set_manage (bool value)
    {
      if (value)
        SETBIT(view_flag_, ViewManageMask);
      else
        CLRBIT(view_flag_, ViewManageMask);
    }

    inline void set_view_type (ViewType type)
    {
      view_flag_ = (view_flag_ & (~ViewTypeMask)) | (type & ViewTypeMask);
    }

    int view_flag_;

    int subs_count_;  // count of sub widgets

    AbstractView* superview_;

    AbstractView* previous_view_;

    AbstractView* next_view_;

    /**
     * @brief The first sub view
     */
    AbstractView* first_subview_;

    AbstractView* last_subview_;

    Point position_;

    Size size_;

#ifdef DEBUG
    std::string name_;
#endif

    static Mutex kRefreshMutex;

    static float kBorderWidth;

    static const float cornervec[WIDGET_CURVE_RESOLU][2];

  };

} /* namespace BlendInt */
