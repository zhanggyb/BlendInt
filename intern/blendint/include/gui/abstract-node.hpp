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

#include <core/rect.hpp>

#include <gui/abstract-view.hpp>
#include <gui/abstract-widget.hpp>

namespace BlendInt {

  class NodeView;

  /**
   * @brief Base class of node and nodeset
   */
  class AbstractNode: public AbstractView
  {
  public:

    AbstractNode (int flag = 0);

    AbstractNode (int width, int height, int flag = 0);

    virtual ~AbstractNode ();

    void SetRoundRadius (float radius);

    void SetRoundType (int type);

    Point GetRelativePosition (const AbstractWidget* widget);

    inline uint32_t round_type () const
    {
      return node_flag_ & 0x0F;
    }

    inline float round_radius () const
    {
      return round_radius_;
    }

    const boost::scoped_ptr<Cpp::ConnectionScope>& events ()
    {
      if (!events_) {
        events_.reset(new Cpp::ConnectionScope);
      }

      return events_;
    }

  protected:

    virtual bool PreDraw (AbstractWindow* context);

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

    /**
     * @brief Perform mouse hover event from NodeView
     */
    virtual Response PerformMouseHover (AbstractWindow* context);

    AbstractWidget* DispatchMouseHover (AbstractWidget* orig,
                                        AbstractWindow* context);

    void GenerateRoundedVertices (std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    void GenerateRoundedVertices (Orientation shadedir,
                                  short shadetop,
                                  short shadedown,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    virtual void PerformRoundTypeUpdate (int round);

    virtual void PerformRoundRadiusUpdate (float radius);

    void SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context);

    inline void set_round_type (int type)
    {
      node_flag_ = (node_flag_ & 0xFFF0) + (type & 0x0F);
    }

    inline void set_round_radius (float radius)
    {
      round_radius_ = radius;
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

    static inline Response dispatch_mouse_hover (AbstractNode* node,
                                                 AbstractWindow* context)
    {
      return node->PerformMouseHover(context);
    }

  private:

    friend class NodeView;

    enum NodeFlagIndex
    {

      NodeRoundTopLeft = (1 << 0),

      NodeRoundTopRight = (1 << 1),

      NodeRoundBottomRight = (1 << 2),

      NodeRoundBottomLeft = (1 << 3),

      /**
       * @brief If the cursor is on border
       */
      NodeCursorOnBorder = (1 << 4),

    };

    /**
     * this frame is the superview of orig
     */
    AbstractWidget* RecheckSubWidgetUnderCursor (AbstractWidget* orig,
                                                 AbstractWindow* context);

    AbstractWidget* RecheckWidgetUnderCursor (AbstractWidget* orig,
                                              AbstractWindow* context);

    AbstractWidget* FindWidgetUnderCursor (AbstractWindow* context);

    AbstractWidget* RecursiveDispatchHoverEvent (AbstractWidget* widget,
                                                 AbstractWindow* context);

    void OnFocusedWidgetDestroyed (AbstractWidget* widget);

    void OnHoverWidgetDestroyed (AbstractWidget* widget);

    inline bool cursor_on_border () const
    {
      return node_flag_ & NodeCursorOnBorder;
    }

    inline void set_cursor_on_border (bool cursor_on_border)
    {
      if (cursor_on_border) {
        SETBIT(node_flag_, NodeCursorOnBorder);
      } else {
        CLRBIT(node_flag_, NodeCursorOnBorder);
      }
    }

    uint32_t node_flag_;

    float round_radius_;

    Point last_position_;

    Size last_size_;

    Point cursor_point_;

    AbstractWidget* focused_widget_;

    AbstractWidget* hovered_widget_;

    int cursor_position_;

    bool focused_;

    bool pressed_;

    boost::scoped_ptr<Cpp::ConnectionScope> events_;

    boost::scoped_ptr<Cpp::Event<AbstractNode*> > destroyed_;

  };

}
