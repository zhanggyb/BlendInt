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

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	Frame::Frame ()
	: AbstractFrame()
	{
	}

	Frame::Frame (int width, int height)
	: AbstractFrame(width, height)
	{
	}

	Frame::~Frame()
	{
	}

	AbstractView* Frame::GetFocusedView () const
	{
		return nullptr;
	}

    void Frame::SetRoundType (int type)
    {
        if((frame_flag_ & 0x0F) == (type & 0x0F)) return;
        
        PerformRoundTypeUpdate(type & 0x0F);
    }
    
    void Frame::SetRoundRadius(float radius)
    {
        if(round_radius_ == radius) return;
        
        PerformRoundRadiusUpdate(radius);
    }
    
	bool Frame::PreDraw(AbstractWindow* context)
	{
		return visiable();
	}

	ResponseType Frame::Draw (AbstractWindow* context)
	{
		DrawSubViewsOnce(context);

		return subs_count() ? Ignore : Finish;
	}

	void Frame::PostDraw(AbstractWindow* context)
	{

	}

	void Frame::PerformFocusOn(AbstractWindow* context)
	{
	}

	void Frame::PerformFocusOff (AbstractWindow* context)
	{

	}

	void Frame::PerformHoverIn(AbstractWindow* context)
	{
	}

	void Frame::PerformHoverOut(AbstractWindow* context)
	{
	}

    ResponseType Frame::PerformContextMenuPress (AbstractWindow* context)
    {
        return Ignore;
    }
    
    ResponseType Frame::PerformContextMenuRelease (AbstractWindow* context)
    {
        return Ignore;
    }

	ResponseType Frame::PerformKeyPress(AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType Frame::PerformMousePress(AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType Frame::PerformMouseRelease(AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType Frame::PerformMouseMove(AbstractWindow* context)
	{
		return Ignore;
	}

	ResponseType Frame::DispatchHoverEvent(AbstractWindow* context)
	{
		return Ignore;
	}

    void Frame::GenerateRoundedVertices(std::vector<GLfloat>* inner,
                                        std::vector<GLfloat>* outer)
    {
        GenerateVertices(size(),
                         default_border_width() * AbstractWindow::theme->pixel(),
                         round_type(),
                         round_radius_,
                         inner,
                         outer);
    }
    
    void Frame::GenerateRoundedVertices(Orientation shadedir,
                                        short shadetop,
                                        short shadedown,
                                        std::vector<GLfloat>* inner,
                                        std::vector<GLfloat>* outer)
    {
        GenerateVertices(size(),
                         default_border_width() * AbstractWindow::theme->pixel(),
                         round_type(),
                         round_radius_,
                         shadedir,
                         shadetop,
                         shadedown,
                         inner,
                         outer);
    }
    
    void Frame::EnableViewBuffer()
    {
        set_buffered(true);
        if(!buffer_) {
            buffer_.reset(new ViewBuffer(size().width(), size().height()));
        }
    }
    
    void Frame::DisableViewBuffer()
    {
        set_buffered(false);
        buffer_.destroy();
    }
    
    void Frame::PerformRoundTypeUpdate (int round_type)
    {
        set_round_type(round_type);
    }
    
    void Frame::PerformRoundRadiusUpdate(float radius)
    {
        round_radius_ = radius;
    }

}
