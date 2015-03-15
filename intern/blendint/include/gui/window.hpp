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

#include <map>

#include <GLFW/glfw3.h>

#include <core/string.hpp>
#include <gui/abstract-window.hpp>
#include <gui/abstract-cursor-theme.hpp>

namespace BlendInt {

  /**
   * @brief A pre-defined GLFW window
   *
   * @ingroup blendint_gui_windows
   */
  class Window: public AbstractWindow
  {
  public:

    Window (int width,
            int height,
            const char* title,
            int flags = WindowVisibleMask);

    virtual ~Window ();

    virtual AbstractWindow* CreateSharedContext (int width,
                                                 int height,
                                                 int flags = WindowRegular);

    virtual void MakeCurrent ();

    virtual void Synchronize ();

    virtual void Exec ();

    virtual void SetCursor (CursorShape cursor_type);

    virtual int GetKeyInput () const;

    virtual int GetScancode () const;

    virtual MouseAction GetMouseAction () const;

    virtual KeyAction GetKeyAction () const;

    virtual int GetModifiers () const;

    virtual MouseButton GetMouseButton () const;

    virtual const String& GetTextInput () const;

    virtual const Point& GetGlobalCursorPosition () const;

    Cpp::EventRef<const Size&> resized ()
    {
      return resized_;
    }

    static bool Initialize ();

    static void Terminate ();

  protected:

    virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    virtual bool PreDraw (AbstractWindow* context);

  private:

    void Close ();

    GLFWwindow* window_;

    Cpp::Event<const Size&> resized_;

    static GLFWcursor* kArrowCursor;

    static GLFWcursor* kCrossCursor;

    static GLFWcursor* kSplitVCursor;

    static GLFWcursor* kSplitHCursor;

    static GLFWcursor* kTopLeftCornerCursor;

    static GLFWcursor* kTopRightCornerCursor;

    static GLFWcursor* kIBeamCursor;

    static std::map<GLFWwindow*, Window*> kSharedWindowMap;

    static KeyAction kKeyAction;

    static int kKey;

    static int kModifiers;

    static int kScancode;

    static String kText;

    static MouseAction kMouseAction;

    static MouseButton kMouseButton;

    static Point kCursor;

    static void CbError (int error, const char* description);

    static void CbWindowSize (GLFWwindow* window, int w, int h);

    static void CbWindowPosition (GLFWwindow* window, int x, int y);

    static void CbKey (GLFWwindow* window,
                       int key,
                       int scancode,
                       int action,
                       int mods);

    static void CbChar (GLFWwindow* window, unsigned int character);

    static void CbMouseButton (GLFWwindow* window,
                               int button,
                               int action,
                               int mods);

    static void CbCursorPos (GLFWwindow* window, double xpos, double ypos);

    static void CbClose (GLFWwindow* window);

#ifdef __APPLE__

    // MUST set this callback to render the context when resizing in OSX
    static void CbWindowRefresh (GLFWwindow* window);

#endif

    static void CreateCursors ();

  };

}
