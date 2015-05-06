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

#include <thread>  // std::thread
#include <mutex>  // std::mutex
#include <vector>

#include <blendint/cppevent/event.hpp>

#include <blendint/opengl/opengl.hpp>

#include <blendint/core/types.hpp>
#include <blendint/core/object.hpp>
#include <blendint/core/point.hpp>
#include <blendint/core/size.hpp>

namespace BlendInt {

class AbstractWindow;
class AbstractFrame;
class ManagedPtr;
struct ColorScheme;

enum ViewFlagsMask
{

  ViewManageMask = 0x1 << 0,

  ViewRefreshMask = 0x1 << 1,

  ViewDestroyingMask = 0x1 << 2,

  ViewTypeMask = 0x07 << 3

};

enum ViewType
{

  ViewTypeUndefined = 0x0,

  ViewTypeWindow = (ViewTypeUndefined + 1) << 3,  // 0x08

  ViewTypeFrame = (ViewTypeUndefined + 2) << 3,   // 0x10

  ViewTypeWidget = (ViewTypeUndefined + 3) << 3,  // 0x18

  ViewTypeNode = (ViewTypeUndefined + 4) << 3     // 0x20

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
class AbstractView: public CppEvent::Trackable
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

  void RequestRedraw ();

  virtual bool IsExpandX () const;

  virtual bool IsExpandY () const;

  virtual Size GetPreferredSize () const;

  virtual bool Contain (const Point& point) const;

  // always return (0, 0) except AbstractScrollable
  virtual Point GetOffset () const;

  virtual AbstractView* GetFirstSubView () const;

  virtual AbstractView* GetLastSubView () const;

  virtual AbstractView* GetNextSubView (const AbstractView* view) const;

  virtual AbstractView* GetPreviousSubView (const AbstractView* view) const;

  virtual int GetSubViewCount () const;

  /**
   * @brief Check if the given subview is visible in this view
   */
  virtual bool IsSubViewActive (const AbstractView* subview) const;

  virtual bool SizeUpdateTest (const AbstractView* source,
                               const AbstractView* target,
                               int width,
                               int height) const;

  virtual bool PositionUpdateTest (const AbstractView* source,
                                   const AbstractView* target,
                                   int x,
                                   int y) const;

  inline const Point& position () const
  {
    return position_;
  }

  inline const Size& size () const
  {
    return size_;
  }

  /*
  inline bool managed () const
  {
    return view_flag_ & ViewManageMask;
  }
  */

  inline int reference_count () const
  {
    return reference_count_;
  }
  
  inline bool refresh () const
  {
    return view_flag_ & ViewRefreshMask;
  }

  inline bool destroying () const
  {
    return view_flag_ & ViewDestroyingMask;
  }

  inline AbstractView* super () const
  {
    return super_;
  }

  /**
   * @brief Check if the given view is visible and interactive in a window
   */
  static bool IsActive (const AbstractView* view);

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

  inline AbstractView* first () const
  {
    return first_;
  }

  inline AbstractView* last () const
  {
    return last_;
  }

  inline int subview_count () const
  {
    return subview_count_;
  }

  inline void set_refresh (bool refresh)
  {
    if (refresh)
      SETBIT(view_flag_, ViewRefreshMask);
    else
      CLRBIT(view_flag_, ViewRefreshMask);
  }

  inline void report_size_update (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height) const
  {
    if (super_) super_->PerformSizeUpdate(source, target, width, height);
  }

  inline void report_position_update (const AbstractView* source,
                                      const AbstractView* target,
                                      int x,
                                      int y) const
  {
    if (super_) super_->PerformPositionUpdate(source, target, x, y);
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

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

  virtual void PerformPositionUpdate (const AbstractView* source,
                                      const AbstractView* target,
                                      int x,
                                      int y);

  AbstractView* GetSubViewAt (int i) const;

  AbstractView* PushFrontSubView (AbstractView* view);

  AbstractView* InsertSubView (int index, AbstractView* view);

  AbstractView* PushBackSubView (AbstractView* view);

  virtual AbstractView* RemoveSubView (AbstractView* view);

  virtual void PerformAfterAdded ();

  virtual void PerformBeforeRemoved ();

  void ClearSubViews ();

  void ResizeSubView (AbstractView* sub, int width, int height);

  void ResizeSubView (AbstractView* sub, const Size& size);

  void MoveSubViewTo (AbstractView* sub, int x, int y);

  void MoveSubViewTo (AbstractView* sub, const Point& pos);

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
   * @brief Destroy a view object
   * @param[in] view the view needs to be destroyed
   * @return
   * 	- A valid pointer to the view which is referenced by a ManagedPtr
   * 	- 0 if the view is destroyed immediately
   */
  static AbstractView* Destroy (AbstractView* view);
  
  /**
   * @brief Swap 2 views' position in their superview
   *
   * The 2 views must have the same superview, otherwise, this function just retern false and do nothing.
   */
  static bool SwapIndex (AbstractView *view1, AbstractView *view2);

  static bool InsertSiblingBefore (AbstractView* src, AbstractView* dst);

  static bool InsertSiblingAfter (AbstractView* src, AbstractView* dst);

  static void GenerateVertices (float xmin,
                                float ymin,
                                float xmax,
                                float ymax,
                                float border,
                                int round_type,
                                float radius,
                                std::vector<GLfloat>* inner,
                                std::vector<GLfloat>* outer);

  static void GenerateVertices (const Size& size,
                                float border,
                                int round_type,
                                float radius,
                                std::vector<GLfloat>* inner,
                                std::vector<GLfloat>* outer);

  static void GenerateVertices (float xmin,
                                float ymin,
                                float xmax,
                                float ymax,
                                float border,
                                int round_type,
                                float radius,
                                Orientation shadedir,
                                short shadetop,
                                short shadedown,
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

  static void GenerateVertices (const Size& size,
                                float border,
                                int round_type,
                                float radius,
                                const ColorScheme& color_scheme,
                                std::vector<GLfloat>* inner,
                                std::vector<GLfloat>* outer);

  static int GetOutlineVertices (int round_type);

  /**
   * @brief Used to get emboss vertices
   * @return
   */
  static int GetHalfOutlineVertices (int round_type);

  static inline int outline_vertex_count (int round_type)
  {
    return kOutlineVertexTable[round_type & 0x0F];
  }

  static inline int emboss_vertex_count (int round_type)
  {
    return kEmbossVertexTable[round_type & 0x0F];
  }

  static inline AbstractView* previous (const AbstractView* view)
  {
    return view->previous_;
  }

  static inline AbstractView* next (const AbstractView* view)
  {
    return view->next_;
  }
  
private:

  friend class AbstractWindow;
  friend class AbstractFrame;
  friend class AbstractWidget;
  friend class AbstractNode;
  friend class AbstractAdjustment;
  friend class ManagedPtr;

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

  /*
  inline void set_manage (bool value)
  {
    if (value)
      SETBIT(view_flag_, ViewManageMask);
    else
      CLRBIT(view_flag_, ViewManageMask);
  }
  */

  inline void set_view_type (ViewType type)
  {
    view_flag_ = (view_flag_ & (~ViewTypeMask)) | (type & ViewTypeMask);
  }

  inline void set_destroying (bool destroying)
  {
    if (destroying)
      SETBIT(view_flag_, ViewDestroyingMask);
    else
      CLRBIT(view_flag_, ViewDestroyingMask);
  }
  
  int reference_count_;
  
  int view_flag_;

  int subview_count_;  // count of sub views

  AbstractView* super_;

  AbstractView* previous_;

  AbstractView* next_;

  /**
   * @brief The first sub view
   */
  AbstractView* first_;

  AbstractView* last_;

  Point position_;

  Size size_;

#ifdef DEBUG
  std::string name_;
#endif

  static std::mutex kRefreshMutex;

  static float kBorderWidth;

  static const float cornervec[WIDGET_CURVE_RESOLU][2];

  static const int kOutlineVertexTable[16];

  static const int kEmbossVertexTable[16];
};

/**
 * @brief Check if a view is contained in a container
 */
extern bool IsContained (AbstractView* container, AbstractView* view);

} /* namespace BlendInt */
