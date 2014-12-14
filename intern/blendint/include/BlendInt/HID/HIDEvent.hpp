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

#ifndef _BLENDINT_INPUTEVENT_HPP_
#define _BLENDINT_INPUTEVENT_HPP_

#include <BlendInt/HID/Input.hpp>

namespace BlendInt {

	class Context;
	class AbstractFrame;

	/**
	 * @brief Events for input devices including keyboard, mouse
	 *
	 * @ingroup device_events
	 */
	class HIDEvent
	{
	public:
		
		/**
		 * @brief Default Constructor
		 * @param modifiers Bitwise conbination of KeyModifier
		 */
		HIDEvent(int modifiers = ModifierNone)
		: context_(0),
		  frame_(0),
		  modifiers_(modifiers)
		{
		}

		HIDEvent(const HIDEvent& orig)
		: context_(orig.context_),
		  frame_(orig.frame_),
		  modifiers_(orig.modifiers_)
		{

		}

		virtual ~HIDEvent()
		{
		}

		Context* context () const
		{
			return context_;
		}

		AbstractFrame* frame () const
		{
			return frame_;
		}

		int modifiers () const {return modifiers_;}

		void set_modifiers (int mods) {modifiers_ = mods;}

	protected:

		friend class Context;
		friend class AbstractFrame;

		void set_context(Context* context)
		{
			context_ = context;
		}

		void set_frame (AbstractFrame* frame)
		{
			frame_ = frame;
		}

		Context* context_;
		AbstractFrame* frame_;

		int modifiers_;

	};

} /* namespace BlendInt */

#endif	/* _BIL_INPUTEVENT_H_ */
