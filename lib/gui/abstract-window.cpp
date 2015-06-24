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

#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <blendint/core/types.hpp>
#include <blendint/core/timer.hpp>

#include <blendint/opengl/opengl.hpp>

#include <blendint/font/fc-pattern.hpp>
#include <blendint/font/fc-config.hpp>

#include <blendint/gui/managed-ptr.hpp>
#include <blendint/gui/abstract-frame.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

std::thread::id AbstractWindow::kMainThreadID;
AbstractWindow* AbstractWindow::kMainWindow   = 0;
glm::mat4 AbstractWindow::default_view_matrix = glm::lookAt(
    glm::vec3(0.f, 0.f, 1.f),           // eye
    glm::vec3(0.f, 0.f, 0.f),           // center
    glm::vec3(0.f, 1.f, 0.f)            // up
);

AbstractWindow::AbstractWindow (int flags)
: AbstractView(),
  active_frame_(nullptr),
  focused_frame_(nullptr),
  stencil_count_(0),
  current_cursor_shape_(ArrowCursor),
  floating_frame_count_(0),
  pressed_(false),
  mouse_tracking_(false),
  overlap_(false)
{
  set_view_type(ViewTypeWindow);

  set_size(640, 480);
  set_refresh(true);

  if (kMainWindow == 0) kMainWindow = this;
}

AbstractWindow::AbstractWindow (int width, int height, int flags)
: AbstractView(width, height),
  active_frame_(nullptr),
  focused_frame_(nullptr),
  stencil_count_(0),
  current_cursor_shape_(ArrowCursor),
  floating_frame_count_(0),
  pressed_(false),
  mouse_tracking_(false),
  overlap_(false)
{
  set_view_type(ViewTypeWindow);

  set_refresh(true);

  if (kMainWindow == 0) kMainWindow = this;
}

AbstractWindow::~AbstractWindow ()
{
  if (kMainWindow == this) kMainWindow = 0;

  if (subview_count() > 0) {
    ClearSubViews();
  } else {
    DBG_ASSERT(subview_count_ == 0);
    DBG_ASSERT(first_ == 0);
    DBG_ASSERT(last_ == 0);
  }

  if (super_) {
    super_->RemoveSubView(this);
  } else {
    DBG_ASSERT(previous_ == 0);
    DBG_ASSERT(next_ == 0);
  }
}

AbstractFrame* AbstractWindow::AddFrame (AbstractFrame* frame)
{
  if (frame == nullptr) return 0;

  if (frame->floating()) {

    if (PushBackSubView(frame)) {

      if (frame->focusable()) {
        if (focused_frame_ != nullptr) focused_frame_->PerformFocusOff(this);
        focused_frame_ = frame;
        focused_frame_->PerformFocusOn(this);
      }

      floating_frame_count_++;
      RequestRedraw();
    }

  } else {

    int index = subview_count() - floating_frame_count_ - 1;

    if (index < 0) {	// no normal frame

      if (InsertSubView(0, frame)) {
        if (frame->focusable()) {
          if (focused_frame_ != nullptr) focused_frame_->PerformFocusOff(this);
          focused_frame_ = frame;
          focused_frame_->PerformFocusOn(this);
        }
        RequestRedraw();
      }

    } else {

      AbstractFrame* top_normal_frame =
          dynamic_cast<AbstractFrame*>(GetSubViewAt(index));
      if (InsertSiblingAfter(top_normal_frame, frame)) {
        if (frame->focusable()) {
          if (focused_frame_ != nullptr) focused_frame_->PerformFocusOff(this);
          focused_frame_ = frame;
          focused_frame_->PerformFocusOn(this);
        }
        RequestRedraw();
      }

    }

  }

  return frame;
}

bool AbstractWindow::SetFocusedFrame (AbstractFrame *frame)
{
  if (focused_frame_ == frame) return true;

  if (frame == nullptr) {

    if (focused_frame_ != nullptr) {
      DBG_ASSERT(focused_frame_->focusable());
      focused_frame_->PerformFocusOff(this);
    }

    return true;
  }

  if (!frame->focusable()) {
    DBG_PRINT_MSG("%s", "Frame is not focusable!");
    return false;
  }

  // if frame is not the root frame in this window, find and switch to it
  if (frame->super_ != this) {

    AbstractView* root_frame = frame;
    AbstractView* window = root_frame->super_;

    while (window->super_ != nullptr) {
      root_frame = root_frame->super_;
      window = root_frame->super_;
    }

    if (window != this) {
      DBG_PRINT_MSG("%s", "the frame is not in this window");
      return false;
    }

    frame = dynamic_cast<AbstractFrame*>(root_frame);
    if (frame == nullptr) return false;
  }

  if (frame->floating()) {

    if (focused_frame_ != nullptr) {
      DBG_ASSERT(focused_frame_->focusable());
      focused_frame_->PerformFocusOff(this);
    }

    MoveToLast(frame);
    focused_frame_ = frame;
    focused_frame_->PerformFocusOn(this);

    RequestRedraw();
    return true;

  }

  int index = subview_count() - floating_frame_count_ - 1;

  if (index < 0) return false;	// no normal frame

  AbstractFrame* top_regular_frame = dynamic_cast<AbstractFrame*>(GetSubViewAt(
      index));

  if (top_regular_frame == frame) {

    if (focused_frame_ != nullptr) {
      DBG_ASSERT(focused_frame_->focusable());
      focused_frame_->PerformFocusOff(this);
    }

    focused_frame_ = frame;
    focused_frame_->PerformFocusOn(this);

  } else {

    // move the frame to the top
    if (InsertSiblingAfter(top_regular_frame, frame)) {
      if (focused_frame_ != nullptr) {
        DBG_ASSERT(focused_frame_->focusable());
        focused_frame_->PerformFocusOff(this);
      }
      focused_frame_ = frame;
      focused_frame_->PerformFocusOn(this);
      RequestRedraw();
    }

  }

  return true;
}

bool AbstractWindow::Contain (const Point& point) const
{
  return true;
}

void AbstractWindow::SetCursor (CursorShape cursor_type)
{
  current_cursor_shape_ = cursor_type;
}

void AbstractWindow::PushCursor ()
{
  cursor_stack_.push(current_cursor_shape_);
}

void AbstractWindow::PopCursor ()
{
  CursorShape cursor = ArrowCursor;

  if (!cursor_stack_.empty()) {
    cursor = cursor_stack_.top();
    cursor_stack_.pop();
  }

  SetCursor(cursor);
}

void AbstractWindow::BeginPushStencil ()
{
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  if (stencil_count_ == 0) {
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NEVER, 1, 0xFF);	// GL_NEVER: always fails
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)
  } else {
    glStencilFunc(GL_LESS, stencil_count_, 0xFF);
    glStencilOp(GL_INCR, GL_KEEP, GL_KEEP); // increase 1s on test fail (always)
  }

  stencil_count_++;
}

void AbstractWindow::EndPushStencil ()
{
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glStencilFunc(GL_EQUAL, stencil_count_, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void AbstractWindow::BeginPopStencil ()
{
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glStencilFunc(GL_LESS, stencil_count_, 0xFF);
  glStencilOp(GL_DECR, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)
}

void AbstractWindow::EndPopStencil ()
{
  if (stencil_count_ > 0) {
    stencil_count_--;

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, stencil_count_, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    if (stencil_count_ == 0) {
      glDisable(GL_STENCIL_TEST);
    }

  }
}

Point AbstractWindow::GetAbsolutePosition (const AbstractView* widget)
{
#ifdef DEBUG
  DBG_ASSERT(widget);
#endif

  AbstractFrame* frame = 0;
  Point pos = widget->position();

  AbstractView* p = widget->super();
  while (p && (p != this)) {
    frame = dynamic_cast<AbstractFrame*>(p);
    if (frame) break;

    pos = pos + p->position() + p->GetOffset();
    p = p->super();
  }

  pos = pos + frame->position() + frame->GetOffset();
  return pos;
}

Point AbstractWindow::GetRelativePosition (const AbstractView* widget)
{
#ifdef DEBUG
  DBG_ASSERT(widget);
#endif

  AbstractFrame* frame = 0;
  Point pos = widget->position();

  AbstractView* p = widget->super();
  while (p && (p != this)) {

    frame = dynamic_cast<AbstractFrame*>(p);
    if (frame) break;

    pos = pos + p->position() + p->GetOffset();
    p = p->super();
  }

  return pos;
}

AbstractWindow* AbstractWindow::GetWindow (AbstractView* view)
{
  AbstractView* parent = view->super();

  if (parent == 0) return dynamic_cast<AbstractWindow*>(view);

  while (parent->super()) {
    parent = parent->super();
  }

  return dynamic_cast<AbstractWindow*>(parent);
}

bool AbstractWindow::InitializeGLContext (int width, int height)
{
  bool success = true;

#ifdef DEBUG
  int major, minor;
  GetGLVersion(&major, &minor);
  DBG_PRINT_MSG("OpenGL version: %d.%d", major, minor);
  GetGLSLVersion(&major, &minor);
  DBG_PRINT_MSG("OpenGL shading language version: %d.%d", major, minor);
#endif

  Timer::SaveCurrentTime();
  if (success && InitializeTheme()) {
    DBG_PRINT_MSG("Timer to intialize theme: %g (ms)",
                  Timer::GetIntervalOfMilliseconds());
  } else {
    DBG_PRINT_MSG("%s", "Cannot initialize Themes");
    success = false;
  }

  Timer::SaveCurrentTime();
  if (success && InitializeShaders(width, height)) {
    DBG_PRINT_MSG("Timer to intialize shaders: %g (ms)",
                  Timer::GetIntervalOfMilliseconds());
  } else {
    DBG_PRINT_MSG("%s", "The Shader Manager is not initialized successfully!");
    success = false;
  }

  Timer::SaveCurrentTime();
  if (success && InitializeIcons()) {
    DBG_PRINT_MSG("Timer to intialize icons: %g (ms)",
                  Timer::GetIntervalOfMilliseconds());
  } else {
    DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
    success = false;
  }

  Timer::SaveCurrentTime();
  // Create Default font: must call this after theme initialized as it read the default_font
  if (success && InitializeFont()) {
    DBG_PRINT_MSG("Timer to intialize font: %g (ms)",
                  Timer::GetIntervalOfMilliseconds());
  } else {
    DBG_PRINT_MSG("%s", "Cannot initialize font");
    success = false;
  }

  return success;
}

void AbstractWindow::ReleaseGLContext ()
{
  ReleaseFont();
  ReleaseIcons();
  ReleaseShaders();
  ReleaseTheme();
}

bool AbstractWindow::SizeUpdateTest (const AbstractView* source,
                                     const AbstractView* target,
                                     int width,
                                     int height) const
{
  return true;
}

bool AbstractWindow::PositionUpdateTest (const AbstractView* source,
                                         const AbstractView* target,
                                         int x,
                                         int y) const
{
  return true;
}

bool AbstractWindow::PreDraw (AbstractWindow* context)
{
  return true;
}

Response AbstractWindow::Draw (AbstractWindow* context)
{
  for (ManagedPtr p = first(); p; ++p) {
    p->PreDraw(context);
    p->Draw(context);
    p->set_refresh(this->refresh());
    p->PostDraw(context);
  }

  return Finish;
}

void AbstractWindow::PostDraw (AbstractWindow* context)
{
}

void AbstractWindow::PerformFocusOn (AbstractWindow* context)
{
}

void AbstractWindow::PerformFocusOff (AbstractWindow* context)
{
}

void AbstractWindow::PerformHoverIn (AbstractWindow* context)
{
}

void AbstractWindow::PerformHoverOut (AbstractWindow* context)
{
}

Response AbstractWindow::PerformKeyPress (AbstractWindow* context)
{
  Response response = Ignore;
  active_frame_ = 0;

  for (ManagedPtr p = last(); p; --p) {
    response = p->PerformKeyPress(context);
    if (response == Finish) break;
  }

  return response;
}

Response AbstractWindow::PerformContextMenuPress (AbstractWindow* context)
{
  return subview_count() ? Ignore : Finish;
}

Response AbstractWindow::PerformContextMenuRelease (AbstractWindow* context)
{
  return subview_count() ? Ignore : Finish;
}

Response AbstractWindow::PerformMousePress (AbstractWindow* context)
{
  Response response = Ignore;
  active_frame_ = 0;
  pressed_ = true;

  for (ManagedPtr p = last(); p; --p) {
    response = p->PerformMousePress(context);
    if (response == Finish) {
      break;
    }
  }

  return response;
}

Response AbstractWindow::PerformMouseRelease (AbstractWindow* context)
{
  Response response = Ignore;
  active_frame_ = 0;
  pressed_ = false;

  for (ManagedPtr p = last(); p; --p) {
    response = p->PerformMouseRelease(context);
    if (response == Finish) {
      break;
    }
  }

  return response;
}

Response AbstractWindow::PerformMouseMove (AbstractWindow* context)
{
  Response response = Ignore;

  active_frame_ = 0;
  if (pressed_) {

    for (ManagedPtr p = last(); p; --p) {
      response = p->PerformMouseMove(context);
      if (response == Finish) {
        break;
      }
    }

  } else {

    if (mouse_tracking_ && focused_frame_) {
      response = focused_frame_->PerformMouseMove(context);
    }

  }

  return response;
}

AbstractView* AbstractWindow::RemoveSubView (AbstractView* view)
{
  AbstractFrame* frame = dynamic_cast<AbstractFrame*>(view);
  if (frame->floating()) {
    floating_frame_count_--;
    DBG_ASSERT(floating_frame_count_ >= 0);
  }

  if (frame == focused_frame_) {

    AbstractView* prev = frame->previous_;
    AbstractFrame* previous_frame = dynamic_cast<AbstractFrame*>(prev);

    while ((prev != nullptr) && (!previous_frame->focusable())) {
      prev = prev->previous_;
      previous_frame = dynamic_cast<AbstractFrame*>(prev);
    }

    focused_frame_ = previous_frame;
    if (focused_frame_ != nullptr) {
      focused_frame_->PerformFocusOn(this);
    }

  }

  return AbstractView::RemoveSubView(view);
}

bool AbstractWindow::InitializeTheme ()
{
  if (!Theme::kTheme) {
    Theme::kTheme = new Theme;
    Theme::kTheme->Reset();
  }

  return true;
}

bool AbstractWindow::InitializeIcons ()
{
  if (!Icons::kIcons) Icons::kIcons = new Icons;

  return true;
}

bool AbstractWindow::InitializeShaders (int width, int height)
{
  bool ret = true;

  if (!Shaders::kShaders) {
    Shaders::kShaders = new Shaders;

    if (Shaders::kShaders) {
      ret = Shaders::kShaders->Setup(width, height);
    } else {
      ret = false;
    }
  }

  return ret;
}

bool AbstractWindow::InitializeFont ()
{
  bool retval = true;

  if (FontCache::kDefaultFontHash == 0) {

    Fc::Pattern p = Fc::Pattern::name_parse(
        (const FcChar8*) Theme::kTheme->default_font());

    Fc::Config::substitute(0, p, FcMatchPattern);
    p.default_substitute();

    FcResult result;
    Fc::Pattern match = Fc::Config::match(0, p, &result);

    if (match) {
      FontCache::Create(match);
      FontCache::kDefaultFontHash = match.hash();
    } else {
      retval = false;
    }

  }

  return retval;
}

void AbstractWindow::ReleaseTheme ()
{
  if (Theme::kTheme) {
    delete Theme::kTheme;
    Theme::kTheme = 0;
  }
}

void AbstractWindow::ReleaseIcons ()
{
  if (Icons::kIcons) {
    delete Icons::kIcons;
    Icons::kIcons = 0;
  }
}

void AbstractWindow::ReleaseShaders ()
{
  if (Shaders::kShaders) {
    delete Shaders::kShaders;
    Shaders::kShaders = 0;
  }
}

void AbstractWindow::ReleaseFont ()
{
  FontCache::ReleaseAll();
}

void AbstractWindow::GetGLVersion (int* major, int* minor)
{
  const char* verstr = (const char*) glGetString(GL_VERSION);
  if ((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
    *major = *minor = 0;
    fprintf(stderr, "Invalid GL_VERSION format!!!\n");
  }
}

void AbstractWindow::GetGLSLVersion (int* major, int* minor)
{
  int gl_major, gl_minor;
  GetGLVersion(&gl_major, &gl_minor);

  *major = *minor = 0;
  if (gl_major == 1) {
    /* GL v1.x can provide GLSL v1.00 only as an extension */
    const char* extstr = (const char*) glGetString(GL_EXTENSIONS);
    if ((extstr != NULL)
        && (strstr(extstr, "GL_ARB_shading_language_100") != NULL)) {
      *major = 1;
      *minor = 0;
    }
  } else if (gl_major >= 2) {
    /* GL v2.0 and greater must parse the version string */
    const char* verstr = (const char*) glGetString(
    GL_SHADING_LANGUAGE_VERSION);

    if ((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
      *major = *minor = 0;
      fprintf(stderr, "Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
    }
  }
}

void AbstractWindow::DispatchMouseHover ()
{
  try {

    active_frame_ = 0;
    overlap_ = false;
    
    Response response = Ignore;
    for (ManagedPtr p = last(); p; --p) {
      response = dynamic_cast<AbstractFrame*>(p.get())->PerformMouseHover(this);
      if (response == Finish) break;
    }

  } catch (std::bad_cast& e) {

    DBG_PRINT_MSG("Error: %s", "Only AbstractFrame should be added in window");
    exit(EXIT_FAILURE);

  }
}

}
