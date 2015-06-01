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

#include <cassert>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <boost/filesystem.hpp>

#include <blendint/core/types.hpp>
#include <blendint/core/image.hpp>
#include <blendint/core/timer.hpp>

#include <blendint/font/fc-config.hpp>

#include <blendint/gui/window.hpp>

#include <blendint/config.hpp>

namespace BlendInt {

GLFWcursor* Window::kArrowCursor = NULL;
GLFWcursor* Window::kCrossCursor = NULL;
GLFWcursor* Window::kSplitVCursor = NULL;
GLFWcursor* Window::kSplitHCursor = NULL;
GLFWcursor* Window::kTopLeftCornerCursor = NULL;
GLFWcursor* Window::kTopRightCornerCursor = NULL;
GLFWcursor* Window::kIBeamCursor = NULL;

std::map<GLFWwindow*, Window*> Window::kSharedWindowMap;

KeyAction Window::kKeyAction = KeyNone;

int Window::kKey = 0;

int Window::kModifiers = 0;

int Window::kScancode = 0;

String Window::kText;

MouseAction Window::kMouseAction = MouseNone;

MouseButton Window::kMouseButton = MouseButtonLeft;

Point Window::kCursor;

Window::Window (int width, int height, const char* title, int flags)
: AbstractWindow(width, height, flags),
  window_(0),
  running_(true),
  visible_(false)
{
  visible_ = flags & WindowVisibleMask ? true : false;

  // TODO: support fullscreen mode
  glfwWindowHint(GLFW_VISIBLE, visible_ ? GL_TRUE : GL_FALSE);

  if (main_window() == this) {
    window_ = glfwCreateWindow(width, height, title, NULL, NULL);
  } else {
    Window* win = dynamic_cast<Window*>(main_window());
    DBG_ASSERT(win);
    window_ = glfwCreateWindow(width, height, title, NULL, win->window_);
  }

  if (window_ == NULL) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  if (main_window() == this) {

    /* Make the window's context current */
    glfwMakeContextCurrent(window_);

    if (!InitializeGLContext()) {
      DBG_PRINT_MSG("Critical: %s", "Cannot initialize GL Context");
      exit(EXIT_FAILURE);
    }

    glm::mat4 projection = glm::ortho(0.f,
                                      pixel_size(size().width()),
                                      0.f,
                                      pixel_size(size().height()),
                                      100.f,
                                      -100.f);

    shaders()->SetFrameProjectionMatrix(projection);
    shaders()->SetFrameViewMatrix(default_view_matrix);
    shaders()->SetFrameModelMatrix(glm::mat3(1.f));

    shaders()->SetWidgetViewMatrix(default_view_matrix);
    shaders()->SetWidgetModelMatrix(glm::mat3(1.f));

    Timer::SaveProgramTime();

  } else {

    kSharedWindowMap[window_] = this;

  }

  if (flags & WindowVisibleMask) {
    glfwSetWindowSizeCallback(window_, &CbWindowSize);
    glfwSetWindowPosCallback(window_, &CbWindowPosition);
    glfwSetKeyCallback(window_, &CbKey);
    glfwSetCharCallback(window_, &CbChar);
    glfwSetMouseButtonCallback(window_, &CbMouseButton);
    glfwSetCursorPosCallback(window_, &CbCursorPos);
    glfwSetWindowCloseCallback(window_, &CbClose);
#ifdef __APPLE__
    glfwSetWindowRefreshCallback(window_, &CbWindowRefresh);
#endif
  }
}

Window::~Window ()
{
  if (main_window() != this) kSharedWindowMap.erase(window_);

  //glfwDestroyWindow(window_);
  window_ = NULL;
}

AbstractWindow* Window::CreateSharedContext (int width, int height, int flags)
{
  Window* shared = new Window(width, height, "", flags);

  return shared;
}

void Window::MakeCurrent ()
{
  glfwMakeContextCurrent(window_);
}

void Window::SwapBuffer ()
{
  glfwSwapBuffers(window_);
}

void Window::Synchronize ()
{
  glfwPostEmptyEvent();
}

void Window::Exec ()
{
  GLFWwindow* main = dynamic_cast<Window*>(main_window())->window_;
  // bool main_visiable = dynamic_cast<Window*>(main_window())->visible_;

  std::map<GLFWwindow*, Window*>::iterator it;

  while (running_) {

    if (main_window()->refresh()) {
      main_window()->MakeCurrent();
#ifdef DEBUG
      // Timer::SaveCurrentTime();
#endif
      reset_refresh_status(main_window());
      if(predraw_window(main_window())) {
        draw_window(main_window());
        postdraw_window(main_window());
      }

      // DBG_PRINT_MSG("Time of one render cycle: %g (ms)", Timer::GetIntervalOfMilliseconds());

      main_window()->SwapBuffer();
    }

    for (it = kSharedWindowMap.begin(); it != kSharedWindowMap.end(); it++) {

      if (it->second->visible_ && it->second->refresh()) {

        glfwMakeContextCurrent(it->first);

        reset_refresh_status(it->second);

        if (predraw_window(it->second)) {
          draw_window(it->second);
          postdraw_window(it->second);
        }

        glfwSwapBuffers(it->first);
      }

    }

    if (glfwWindowShouldClose(main)) running_ = false;

    // TODO: fire event then go idle

    glfwWaitEvents();

  }
}

void Window::SetCursor (CursorShape cursor_type)
{
  switch (cursor_type) {

    case ArrowCursor: {
      glfwSetCursor(window_, kArrowCursor);
      break;
    }

    case CrossCursor: {
      glfwSetCursor(window_, kCrossCursor);
      break;
    }

    case SplitVCursor: {
      glfwSetCursor(window_, kSplitVCursor);
      break;
    }

    case SplitHCursor: {
      glfwSetCursor(window_, kSplitHCursor);
      break;
    }

    case SizeFDiagCursor: {
      glfwSetCursor(window_, kTopLeftCornerCursor);
      break;
    }

    case SizeBDiagCursor: {
      glfwSetCursor(window_, kTopRightCornerCursor);
      break;
    }

    case IBeamCursor: {
      glfwSetCursor(window_, kIBeamCursor);
      break;
    }

    default: {
      glfwSetCursor(window_, kArrowCursor);
      break;
    }
  }
}

int Window::GetKeyInput () const
{
  return kKey;
}

int Window::GetScancode () const
{
  return kScancode;
}

MouseAction Window::GetMouseAction () const
{
  return kMouseAction;
}

KeyAction Window::GetKeyAction () const
{
  return kKeyAction;
}

int Window::GetModifiers () const
{
  return kModifiers;
}

MouseButton Window::GetMouseButton () const
{
  return kMouseButton;
}

const String& Window::GetTextInput () const
{
  return kText;
}

const Point& Window::GetGlobalCursorPosition () const
{
  return kCursor;
}

bool Window::Initialize ()
{
  if (!Fc::Config::init()) {
    DBG_PRINT_MSG("Critical: %s", "Cannot initialize Fontconfig");
    return false;
  }

  /* Initialize the library */
  if (!glfwInit()) {
    DBG_PRINT_MSG("Critical: %s", "Cannot initialize GLFW");
    return false;
  }

  CreateCursors();

  glfwSetErrorCallback(&CbError);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  kMainThreadID = std::this_thread::get_id();

  return true;
}

void Window::Terminate ()
{
  glfwDestroyCursor(kArrowCursor);
  glfwDestroyCursor(kCrossCursor);
  glfwDestroyCursor(kSplitVCursor);
  glfwDestroyCursor(kSplitHCursor);
  glfwDestroyCursor(kTopLeftCornerCursor);
  glfwDestroyCursor(kTopRightCornerCursor);
  glfwDestroyCursor(kIBeamCursor);

  glfwTerminate();
  Fc::Config::fini();
}

void Window::PerformPositionUpdate (const AbstractView* source,
                                    const AbstractView* target,
                                    int x,
                                    int y)
{
  if (target == this) {
    set_position(x, y);

//			GLFWmonitor* monitor = glfwGetWindowMonitor(window_);
//
//			int monitor_width, monitor_height;
//			glfwGetMonitorPhysicalSize(monitor, &monitor_width, &monitor_height);

//			DBG_PRINT_MSG("monitor size: %d, %d", monitor_width, monitor_height);

    if (source == this) {
      glfwSetWindowPosCallback(window_, NULL);
      glfwSetWindowPos(window_, position().x(), position().y());
      glfwSetWindowPosCallback(window_, &CbWindowPosition);
    }
  }
}

void Window::PerformSizeUpdate (const AbstractView* source,
                                const AbstractView* target,
                                int width,
                                int height)
{
  if (target == this) {
    set_size(width, height);

    glm::mat4 projection = glm::ortho(0.f,
                                      pixel_size(size().width()),
                                      0.f,
                                      pixel_size(size().height()),
                                      100.f,
                                      -100.f);
    shaders()->SetFrameProjectionMatrix(projection);

    set_refresh(true);

    if (source == this) {
      glfwSetWindowSizeCallback(window_, NULL);
      glfwSetWindowSize(window_, size().width(), size().height());
      glfwSetWindowSizeCallback(window_, &CbWindowSize);
    }

    resized_.Invoke(size());
  }
}

bool Window::PreDraw (AbstractWindow* context)
{
  if (!visible_) return false;

  glClearColor(0.208f, 0.208f, 0.208f, 1.f);
  //glClearColor(0.105f, 0.105f, 0.105f, 0.75f);
  //glClearColor(1.f, 1.f, 1.f, 1.f);
  glClearStencil(0);
  glClearDepth(1.0);

  glClear(GL_COLOR_BUFFER_BIT |
  GL_DEPTH_BUFFER_BIT |
  GL_STENCIL_BUFFER_BIT);

  // Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  set_viewport_origin(0, 0);
  if (stencil_count() != 0) {
    DBG_PRINT_MSG("Warning: %s, stencil_count_: %u",
                  "stencil used but not released", stencil_count());
  }
  set_stencil_count(0);

  glViewport(0,
             0,
             pixel_size(size().width()),
             pixel_size(size().height()));

  return true;
}

void Window::Close ()
{
  // MUST clear sub views before releasing gl context, unload all fonts to make sure Fc::Fini success.
  ClearSubViews();
  ReleaseGLContext();
}

void Window::CbError (int error, const char* description)
{
  DBG_PRINT_MSG("Error: %s (error code: %d)", description, error);
}

void Window::CbWindowSize (GLFWwindow* window, int w, int h)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if (it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

  win->PerformSizeUpdate(0, win, w, h);
}

void Window::CbWindowPosition (GLFWwindow* window, int x, int y)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if (it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

  win->PerformPositionUpdate(0, win, x, y);
}

void Window::CbKey (GLFWwindow* window,
                    int key,
                    int scancode,
                    int action,
                    int mods)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if (it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

  switch (action) {
    case GLFW_PRESS:
      kKeyAction = KeyPress;
      break;
    case GLFW_RELEASE:
      kKeyAction = KeyRelease;
      break;
    case GLFW_REPEAT:
      kKeyAction = KeyRepeat;
      break;
    default:
      kKeyAction = KeyNone;
      break;
  }

  kKey = key;
  kModifiers = mods;
  kScancode = scancode;
  kText.clear();

  switch (kKeyAction) {

    case KeyPress: {
      win->PerformKeyPress(win);
      break;
    }

    case KeyRelease: {
      break;
    }

    case KeyRepeat: {
      break;
    }

    default:
      break;
  }
}

void Window::CbChar (GLFWwindow* window, unsigned int character)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if (it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

#ifdef __APPLE__
  // glfw3 in Mac OS will call this function if press some unprintalbe keys such as Left, Right, Up, Down
  if (character > 255) {
    DBG_PRINT_MSG("unprintable character in Mac: %u", character);
    return;
  }
#endif

  kText.clear();
  kText.push_back(character);

  switch (kKeyAction) {

    case KeyPress: {
      win->PerformKeyPress(win);
      break;
    }

    case KeyRelease: {
      break;
    }

    case KeyRepeat: {
      break;
    }

    default:
      break;
  }

}

void Window::CbMouseButton (GLFWwindow* window,
                            int button,
                            int action,
                            int mods)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if (it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

  switch (action) {
    case GLFW_RELEASE:
      kMouseAction = MouseRelease;
      break;
    case GLFW_PRESS:
      kMouseAction = MousePress;
      break;
    case GLFW_REPEAT:
      kMouseAction = MouseNone;
      break;
    default:
      break;
  }

  kMouseButton = MouseButtonNone;

  switch (button) {
    case GLFW_MOUSE_BUTTON_1:
      kMouseButton = MouseButtonLeft;
      break;
    case GLFW_MOUSE_BUTTON_2:
      kMouseButton = MouseButtonRight;
      break;
    case GLFW_MOUSE_BUTTON_3:
      kMouseButton = MouseButtonMiddle;
      break;
    default:
      break;
  }

  kModifiers = mods;

  switch (kMouseAction) {

    case MouseMove: {
      win->DispatchMouseHover();
      win->PerformMouseMove(win);
      break;
    }

    case MousePress: {
      win->DispatchMouseHover();
      win->PerformMousePress(win);
      break;
    }

    case MouseRelease: {
      win->PerformMouseRelease(win);
      win->DispatchMouseHover();
      break;
    }

    default:
      break;
  }

}

void Window::CbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if (it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

  kCursor.reset((int) xpos, win->size().height() - (int) ypos);

  kMouseAction = MouseMove;
  kMouseButton = MouseButtonNone;

  switch (kMouseAction) {

    case MouseMove: {
      win->DispatchMouseHover();
      win->PerformMouseMove(win);
      break;
    }

    case MousePress: {
      win->DispatchMouseHover();
      win->PerformMousePress(win);
      break;
    }

    case MouseRelease: {
      win->PerformMouseRelease(win);
      win->DispatchMouseHover();
      break;
    }

    default:
      break;
  }

}

#ifdef __APPLE__

// MUST set this callback to render the context when resizing in OSX
void Window::CbWindowRefresh (GLFWwindow* window)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if(it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

  win->set_refresh(false);
  if (win->PreDraw(win)) {
    win->Draw(win);
    win->PostDraw(win);
  }

  glfwSwapBuffers(win->window_);
}

#endif

void Window::CbClose (GLFWwindow* window)
{
  Window* win = 0;
  std::map<GLFWwindow*, Window*>::iterator it = kSharedWindowMap.find(window);
  if (it != kSharedWindowMap.end()) {
    win = it->second;
  } else {
    win = dynamic_cast<Window*>(main_window());
  }

  DBG_ASSERT(win);

  win->Close();
}

void Window::CreateCursors ()
{
  namespace fs = boost::filesystem;

  fs::path cursors_path(
  BLENDINT_INSTALL_PREFIX"/share/blendint/cursors");

  if (!fs::exists(cursors_path)) {
    cursors_path = fs::path(
    BLENDINT_PROJECT_SOURCE_DIR"/data/cursors");
  }

  if (!fs::exists(cursors_path)) return;

  GLFWimage cursor;
  Image img;

  std::string filepath;

  filepath = cursors_path.string() + "/" + "left_ptr.png";
  if (img.Read(filepath)) {
    cursor.width = img.width();
    cursor.height = img.height();
    cursor.pixels = const_cast<unsigned char*>(img.pixels());

    kArrowCursor = glfwCreateCursor(&cursor, 9, 5);
    DBG_ASSERT(kArrowCursor != nullptr);
  } else {
    DBG_PRINT_MSG("%s", "Fail to load cursor");
  }

  filepath = cursors_path.string() + "/" + "hand2.png";
  if (img.Read(filepath)) {
    cursor.width = img.width();
    cursor.height = img.height();
    cursor.pixels = const_cast<unsigned char*>(img.pixels());

    kCrossCursor = glfwCreateCursor(&cursor, 9, 4);
    DBG_ASSERT(kCrossCursor != nullptr);
  } else {
    DBG_PRINT_MSG("%s", "Fail to load cursor");
  }

  filepath = cursors_path.string() + "/" + "sb_v_double_arrow.png";
  if (img.Read(filepath)) {
    cursor.width = img.width();
    cursor.height = img.height();
    cursor.pixels = const_cast<unsigned char*>(img.pixels());

    kSplitVCursor = glfwCreateCursor(&cursor, 11, 10);
    DBG_ASSERT(kSplitVCursor != nullptr);
  } else {
    DBG_PRINT_MSG("%s", "Fail to load cursor");
  }

  filepath = cursors_path.string() + "/" + "sb_h_double_arrow.png";
  if (img.Read(filepath)) {
    cursor.width = img.width();
    cursor.height = img.height();
    cursor.pixels = const_cast<unsigned char*>(img.pixels());

    kSplitHCursor = glfwCreateCursor(&cursor, 11, 11);
    DBG_ASSERT(kSplitHCursor != nullptr);
  } else {
    DBG_PRINT_MSG("%s", "Fail to load cursor");
  }

  filepath = cursors_path.string() + "/" + "top_left_corner.png";
  if (img.Read(filepath)) {
    cursor.width = img.width();
    cursor.height = img.height();
    cursor.pixels = const_cast<unsigned char*>(img.pixels());

    kTopLeftCornerCursor = glfwCreateCursor(&cursor, 11, 11);
    DBG_ASSERT(kTopLeftCornerCursor != nullptr);
  } else {
    DBG_PRINT_MSG("%s", "Fail to load cursor");
  }

  filepath = cursors_path.string() + "/" + "top_right_corner.png";
  if (img.Read(filepath)) {
    cursor.width = img.width();
    cursor.height = img.height();
    cursor.pixels = const_cast<unsigned char*>(img.pixels());

    kTopRightCornerCursor = glfwCreateCursor(&cursor, 12, 11);
    DBG_ASSERT(kTopRightCornerCursor != nullptr);
  } else {
    DBG_PRINT_MSG("%s", "Fail to load cursor");
  }

  filepath = cursors_path.string() + "/" + "xterm.png";
  if (img.Read(filepath)) {
    cursor.width = img.width();
    cursor.height = img.height();
    cursor.pixels = const_cast<unsigned char*>(img.pixels());

    kIBeamCursor = glfwCreateCursor(&cursor, 12, 11);
    DBG_ASSERT(kIBeamCursor != nullptr);
  } else {
    DBG_PRINT_MSG("%s", "Fail to load cursor");
  }

}

}  // namespace BlendInt
