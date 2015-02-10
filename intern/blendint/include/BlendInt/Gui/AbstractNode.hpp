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

#include <BlendInt/Gui/AbstractView.hpp>

namespace BlendInt {

	/**
	 * @brief Base class of node and nodeset
	 */
	class AbstractNode: public AbstractView
	{
	public:

		AbstractNode ()
		: AbstractView(),
		  node_flag_(0),
		  round_radius_(5.f)
		{

		}

		virtual ~AbstractNode ()
		{

		}

		void SetRoundRadius (float radius);

		void SetRoundType (int type);

		inline uint32_t round_type () const
		{
			return node_flag_ & 0x0F;
		}

		inline float round_radius () const
		{
			return round_radius_;
		}

	protected:

		virtual bool PreDraw (AbstractWindow* context);

		// virtual Response Draw (Profile& profile);

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

		void GenerateRoundedVertices (
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		void GenerateRoundedVertices (
				Orientation shadedir,
				short shadetop,
				short shadedown,
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		virtual void PerformRoundTypeUpdate (int round);

		virtual void PerformRoundRadiusUpdate (float radius);

		inline void set_round_type (int type)
		{
			node_flag_ = (node_flag_ & 0xFFF0) + (type & 0x0F);
		}

		inline void set_round_radius (float radius)
		{
			round_radius_ = radius;
		}

	private:

		enum NodeFlagIndex {

			NodeRoundTopLeft = (1 << 0),

			NodeRoundTopRight = (1 << 1),

			NodeRoundBottomRight = (1 << 2),

			NodeRoundBottomLeft = (1 << 3),

		};

		uint32_t node_flag_;

		float round_radius_;

	};

}
