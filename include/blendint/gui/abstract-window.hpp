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

#include <blendint/core/input.hpp>
#include <blendint/gui/abstract-view.hpp>

#include <blendint/stock/icons.hpp>
#include <blendint/stock/theme.hpp>
#include <blendint/stock/shaders.hpp>

namespace BlendInt {

enum WindowFlagMask
{

  WindowFocusedMask = 0x1 << 0,

  WindowIconifiedMask = 0x1 << 1,

  WindowResizableMask = 0x1 << 2,

  /**
   * 0: invisible
   * 1: visible
   */
  WindowVisibleMask = 0x1 << 3,

  WindowDecoratedMask = 0x1 << 4,

  WindowAutoIconifyMask = 0x1 << 5,

  /**
   * 0: regular window
   * 1: floating window
   */
  WindowFloatingMask = 0x1 << 6,

  /**
   * 0: regular window
   * 1: fullscreen
   */
  WindowFullscreenMask = 0x1 << 7

};

enum WindowFlags
{

  WindowRegular = WindowFocusedMask | WindowResizableMask | WindowDecoratedMask
      | WindowVisibleMask,

  WindowInvisible = 0,

  WindowFloating = WindowRegular | WindowFloatingMask,

  WindowFullscreen = WindowRegular | WindowFullscreenMask

};

class AbstractFrame;

/**
 * @brief Abstract class for window
 *
 * The AbstractWindow class represents a window in the desktop.
 *
 * A sub class should override the virual functions and make sure
 * an OpenGL context is created.
 *
 * @ingroup blendint_gui_windows
 * @see Window
 */
class AbstractWindow: public AbstractView
{

public:

  /**
   * @brief Default constructor
   *
   * Create a 640, 480 window
   */
  AbstractWindow (int flag = WindowRegular);

  /**
   * @brief Constructor with window size
   */
  AbstractWindow (int width, int height, int flag = WindowRegular);

  /**
   * @brief Destructor
   */
  virtual ~AbstractWindow ();

  /**
   * @brief Add a frame in this window
   */
  AbstractFrame* AddFrame (AbstractFrame* frame);

  bool SetFocusedFrame (AbstractFrame* frame);

  virtual bool Contain (const Point& point) const;

  virtual AbstractWindow* CreateSharedContext (int width,
                                               int height,
                                               int flags) = 0;

  /**
   * @brief Make this window as the current OpenGL Context
   */
  virtual void MakeCurrent () = 0;

  virtual void SwapBuffer () = 0;

  /**
   * @brief Synchronize this window to redraw contents
   */
  virtual void Synchronize () = 0;

  virtual void Exec () = 0;

  virtual void SetCursor (CursorShape cursor_type);

  void PushCursor ();

  void PopCursor ();

  void BeginPushStencil ();

  void EndPushStencil ();

  void BeginPopStencil ();

  void EndPopStencil ();

  virtual int GetKeyInput () const = 0;

  virtual int GetScancode () const = 0;

  virtual MouseAction GetMouseAction () const = 0;

  virtual KeyAction GetKeyAction () const = 0;

  virtual int GetModifiers () const = 0;

  virtual MouseButton GetMouseButton () const = 0;

  virtual const String& GetTextInput () const = 0;

  virtual const Point& GetGlobalCursorPosition () const = 0;

  Point GetAbsolutePosition (const AbstractView* widget);

  Point GetRelativePosition (const AbstractView* widget);

  virtual bool SizeUpdateTest (const AbstractView* source,
                               const AbstractView* target,
                               int width,
                               int height) const;

  virtual bool PositionUpdateTest (const AbstractView* source,
                                   const AbstractView* target,
                                   int x,
                                   int y) const;

  inline void set_mouse_tracking (bool tracking)
  {
    mouse_tracking_ = tracking;
  }

  inline bool mouse_tracking () const
  {
    return mouse_tracking_;
  }

  inline void set_local_cursor_position (const Point& point)
  {
    local_cursor_position_ = point;
  }

  inline void set_local_cursor_position (int x, int y)
  {
    local_cursor_position_.reset(x, y);
  }

  inline const Point& local_cursor_position () const
  {
    return local_cursor_position_;
  }

  inline AbstractFrame* active_frame () const
  {
    return active_frame_;
  }

  inline const Point& viewport_origin () const
  {
    return viewport_origin_;
  }

  /**
   * @brief Get the current cursor shape
   */
  inline CursorShape current_cursor_shape () const
  {
    return current_cursor_shape_;
  }

  /**
   * @brief If there's a frame contains current cursor position
   *
   * This status is used in frame's DispatchMouseHover() to check if
   * there's popup frame overlap at the current cursor position.
   */
  inline bool overlap () const
  {
    return overlap_;
  }

  inline void set_overlap (bool overlap)
  {
    overlap_ = overlap;
  }
  
  static AbstractWindow* GetWindow (AbstractView* widget);

  static bool InitializeGLContext ();

  static void ReleaseGLContext ();

  static inline std::thread::id& main_thread_id ()
  {
    return kMainThreadID;
  }

  static inline AbstractWindow* main_window ()
  {
    return kMainWindow;
  }

  static inline Theme* theme ()
  {
    return kTheme;
  }

  static inline Icons* icons ()
  {
    return kIcons;
  }

  static inline Shaders* shaders ()
  {
    return kShaders;
  }

protected:

  virtual bool PreDraw (AbstractWindow* context);

  virtual Response Draw (AbstractWindow* context);

  virtual void PostDraw (AbstractWindow* context);

  virtual void PerformFocusOn (AbstractWindow* context);

  virtual void PerformFocusOff (AbstractWindow* context);

  virtual void PerformHoverIn (AbstractWindow* context);

  virtual void PerformHoverOut (AbstractWindow* context);

  virtual Response PerformKeyPress (AbstractWindow* context);

  virtual Response PerformContextMenuPress (AbstractWindow* context);

  virtual Response PerformContextMenuRelease (AbstractWindow* context);

  virtual Response PerformMousePress (AbstractWindow* context);

  virtual Response PerformMouseRelease (AbstractWindow* context);

  virtual Response PerformMouseMove (AbstractWindow* context);

  virtual AbstractView* RemoveSubView (AbstractView* view);

  void DispatchMouseHover ();

  inline void set_viewport_origin (int x, int y)
  {
    viewport_origin_.reset(x, y);
  }

  inline void set_viewport_origin (const Point& point)
  {
    viewport_origin_ = point;
  }

  inline GLuint stencil_count () const
  {
    return stencil_count_;
  }

  inline void set_stencil_count (GLuint count)
  {
    stencil_count_ = count;
  }

  static glm::mat4 default_view_matrix;

  static std::thread::id kMainThreadID;

  static inline void reset_refresh_status (AbstractWindow* window)
  {
    window->set_refresh(false);
  }

  static inline bool predraw_window (AbstractWindow* window)
  {
    return window->PreDraw(window);
  }

  static inline void draw_window (AbstractWindow* window)
  {
    window->Draw(window);
  }

  static inline void postdraw_window (AbstractWindow* window)
  {
    window->PostDraw(window);
  }

  static Theme* kTheme;

  static Icons* kIcons;

  static Shaders* kShaders;

private:

  friend class AbstractFrame;
  friend class AbstractWidget;

  static bool InitializeTheme ();

  static bool InitializeIcons ();

  static bool InitializeShaders ();

  static bool InitializeFont ();

  static void ReleaseTheme ();

  static void ReleaseIcons ();

  static void ReleaseShaders ();

  static void ReleaseFont ();

  static void GetGLVersion (int *major, int *minor);

  static void GetGLSLVersion (int *major, int *minor);

  AbstractFrame* active_frame_;

  AbstractFrame* focused_frame_;

  // the viewport offset
  Point viewport_origin_;

  Point local_cursor_position_;

  GLuint stencil_count_;

  CursorShape current_cursor_shape_;

  std::stack<CursorShape> cursor_stack_;

  int floating_frame_count_;

  bool pressed_;

  bool mouse_tracking_;

  /**
   * @brief If a frame contains the cursor
   */
  bool overlap_;

  static AbstractWindow* kMainWindow;
};

inline float pixel_size (float a)
{
  return a * AbstractWindow::theme()->pixel();
}

inline float pixel_size (int a)
{
  return a * AbstractWindow::theme()->pixel();
}

inline float pixel_size (unsigned int a)
{
  return a * AbstractWindow::theme()->pixel();
}

}  // namespace BlendInt
