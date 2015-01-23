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

#include <BlendInt/Gui/NodeView.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	NodeView::NodeView()
	: Widget(),
	  curve_(0)
	{
		set_size (400, 300);

		curve_ = new CubicBezierCurve;
		curve_->Unpack();
	}

	NodeView::~NodeView ()
	{
		delete curve_;
	}

	bool NodeView::AddNode (AbstractNode* node)
	{
		return true;
	}

	void NodeView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void NodeView::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);
		RequestRedraw();
	}

	void NodeView::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);
		RequestRedraw();
	}

	ResponseType NodeView::Draw(const Context* context)
	{
		curve_->Draw();

		return Finish;
	}

}
