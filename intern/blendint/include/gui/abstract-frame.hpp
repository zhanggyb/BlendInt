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

#include <boost/smart_ptr.hpp>
#include <glm/glm.hpp>

#include <gui/view-buffer.hpp>
#include <gui/abstract-view.hpp>
#include <gui/abstract-widget.hpp>

namespace BlendInt {

  enum FrameFlagMask
  {

    /**
     * whether the frame can be focused
     */
    FrameFocusableMask = (1 << 0),

    /**
     * whether the frame is floating above other regular frames, also called always-on-top.
     */
    FrameFloatingMask = (1 << 1)

  };

  enum FrameFlags {

    FrameRegular = FrameFocusableMask,

    FrameTopmost = FrameFocusableMask | FrameFloatingMask,

    FramePopup = FrameFloatingMask

  };

  class FrameSplitter;

  /**
   * @brief The abstract class of all frames
   *
   * In BlendInt, frame is used to divide the window (or GL context)
   * into different regions, hold and manage widgets inside and
   * process the HID events get from window.
   *
   * A frame can only be added and removed in a window, and use
   * framebuffer object to render widgets into a texture in general.
   * This makes sure the widgets whithin the frame will be rendered
   * only when needed.
   *
   * Frame can be focused. A focused frame has the high priority to
   * accept HID events from window and display on other frames.
   *
   * @ingroup blendint_gui_frames
   */
  class AbstractFrame: public AbstractView
  {
  public:

    friend class AbstractWindow;

    AbstractFrame (int frame_flag = FrameRegular);

    AbstractFrame (int width, int height, int frame_flag = FrameRegular);

    virtual ~AbstractFrame ();

    Point GetAbsolutePosition (const AbstractView* widget);

    Point GetRelativePosition (const AbstractView* widget);

    bool EnableViewBuffer ();

    void DisableViewBuffer ();

    inline bool has_view_buffer () const
    {
      return view_buffer_;
    }

    inline bool focusable () const
    {
      return frame_flag_ & FrameFocusableMask;
    }

    inline bool floating () const
    {
      return frame_flag_ & FrameFloatingMask;
    }

    Cpp::EventRef<AbstractFrame*> destroyed ()
    {
      return *destroyed_;
    }

    static AbstractFrame* GetFrame (AbstractView* view);

  protected:

    virtual Response PerformContextMenuPress (AbstractWindow* context);

    virtual Response PerformContextMenuRelease (AbstractWindow* context);

    virtual Response PerformMouseHover (AbstractWindow* context) = 0;

    AbstractWidget* DispatchMouseHover (AbstractWidget* orig,
                                        AbstractWindow* context);

    void ClearHoverWidgets (AbstractView* hovered_widget);

    void ClearHoverWidgets (AbstractView* hovered_widget,
                            AbstractWindow* context);

    inline const RefPtr<ViewBuffer>& view_buffer () const
    {
      return view_buffer_;
    }

    inline void set_view_buffer (const RefPtr<ViewBuffer>& buffer)
    {
      view_buffer_ = buffer;
    }

    const boost::scoped_ptr<Cpp::ConnectionScope>& events ()
    {
      if (!events_) {
        events_.reset(new Cpp::ConnectionScope);
      }

      return events_;
    }

    static inline Response dispatch_key_press (AbstractView* view,
                                               AbstractWindow* context)
    {
      return view->PerformKeyPress(context);
    }

    static inline Response dispatch_mouse_press (AbstractView* view,
                                                 AbstractWindow* context)
    {
      return view->PerformMousePress(context);
    }

    static inline Response dispatch_mouse_release (AbstractView* view,
                                                   AbstractWindow* context)
    {
      return view->PerformMouseRelease(context);
    }

    static inline Response dispatch_mouse_move (AbstractView* view,
                                                AbstractWindow* context)
    {
      return view->PerformMouseMove(context);
    }

    static void dispatch_focus_on (AbstractView* view, AbstractWindow* context)
    {
      view->PerformFocusOn(context);
    }

    static void dispatch_focus_off (AbstractView* view, AbstractWindow* context)
    {
      view->PerformFocusOff(context);
    }

    static inline void dispatch_mouse_hover_in (AbstractView* view,
                                                AbstractWindow* context)
    {
      view->PerformHoverIn(context);
    }

    static inline void dispatch_mouse_hover_out (AbstractView* view,
                                                 AbstractWindow* context)
    {
      view->PerformHoverOut(context);
    }

    static inline Response dispatch_mouse_hover (AbstractFrame* frame,
                                                 AbstractWindow* context)
    {
      return frame->PerformMouseHover(context);
    }

    /**
     * @brief Render to texture
     * @param[in] frame
     * @param[in] context
     * @param[out] texture
     */
    static bool RenderSubFramesToTexture (AbstractFrame* frame,
                                          AbstractWindow* context,
                                          const glm::mat4& projection,
                                          const glm::mat3& model,
                                          GLTexture2D* texture);

  private:

    friend class FrameSplitter;

    /**
     * this frame is the superview of orig
     */
    AbstractWidget* RecheckSubWidgetUnderCursor (AbstractWidget* orig,
                                                 AbstractWindow* context);

    AbstractWidget* RecheckWidgetUnderCursor (AbstractWidget* orig,
                                              AbstractWindow* context);

    AbstractWidget* FindWidgetUnderCursor (AbstractWindow* context);

    AbstractWidget* RecursiveDispatchHoverEvent (AbstractWidget* view,
                                                 AbstractWindow* context);

    uint32_t frame_flag_;

    RefPtr<ViewBuffer> view_buffer_;

    boost::scoped_ptr<Cpp::ConnectionScope> events_;

    boost::scoped_ptr<Cpp::Event<AbstractFrame*> > destroyed_;

    static glm::mat4 kViewMatrix;
  };

}
