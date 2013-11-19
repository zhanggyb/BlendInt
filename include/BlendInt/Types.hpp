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

#ifndef _BLENDINT_TYPES_HPP_
#define _BLENDINT_TYPES_HPP_

#include <string>	// typedef String

/**
 * A macro to disallow the copy constructor and operator= functions
 * This should be used in the private: declarations for a class
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	private: \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#ifdef DEBUG
#include <stdio.h>
#define PRINT_DEBUG_MSG(msg) \
	fprintf(stderr, "%s: %d, %s -- %s\n", __FILE__, __LINE__, __func__, msg)
#endif

/**
 * macro to define a class property that will effect the opengl drawing,
 * change this property should call Update()
 */
#define DRAWABLE_PROPERTY

/** macro to declare a member function is an event */
#define EVENT_CALLER

/** macro to declare a member function is an delegate */
#define DELEGATE


#define WIDGET_AA_JITTER 8

/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

namespace BlendInt {

	enum HorizontalAlignment {
		AlignTop = 0x0001,
		AlignBottom = 0x0002,
	  AlignHorizontalCenter = 0x0004
	};

	enum VerticalAlignment {
		AlignLeft = 0x0010,
		AlignRight = 0x0020,
	  AlignVerticalCenter = 0x0040
	};

	enum Alignment {
		AlignNone = 0x0000,
		AlignCenter = AlignHorizontalCenter | AlignVerticalCenter
	};

	/**
	 * flags to set which corners will become rounded:
	 *
	 * 1------2
	 * |      		|
	 * 8------4
	 */
//	enum CornerType
//	{
//		CornerNone = 0,
//		CornerTopLeft = (1 << 0),
//		CornerTopRight = (1 << 1),
//		CornerBottomRight = (1 << 2),
//		CornerBottomLeft = (1 << 3),
//		/* just for convenience */
//		CornerAll = (CornerTopLeft | CornerTopRight
//		        | CornerBottomRight | CornerBottomLeft)
//	};

	enum ScrollState
	{
		SCROLL_PRESSED = (1 << 0),
		SCROLL_ARROW = (1 << 1),
		SCROLL_NO_OUTLINE = (1 << 2)
	};

	enum Orientation
	{
		Horizontal = 0x1, Vertical = 0x2
	};


	/**
	 * flags to set which corners will become rounded:
	 *
	 * 1------2
	 * |      		|
	 * 8------4
	 */
	enum RoundType {
		RoundNone = 0,
		RoundTopLeft = (1 << 0),
		RoundTopRight = (1 << 1),
		RoundBottomRight = (1 << 2),
		RoundBottomLeft = (1 << 3),
		/* just for convenience */
		RoundAll = (RoundTopLeft | RoundTopRight
				| RoundBottomRight | RoundBottomLeft),
		UI_RB_ALPHA = RoundAll + 1
	};


	enum FormPropertyType {
		FormSize,
		FormPosition,
		FormPreferredSize,
		FormMinimalSize,
		FormMaximalSize,
		WidgetLayer,
		WidgetVisibility,
		WidgetBorderWidth,
		FormRoundType,
		FormRoundRadius,
		FrameMargin,
		LayoutPropertyAlignment,
		LayoutPropertySpace,
		LayoutPropertyMargin,
		LayoutPropertyItem,

		SliderPropertyOrientation,
		SliderPropertyValue,
		SliderPropertyMinimum,
		SliderPropertyMaximum,
		SliderPropertyStep,

		FormPropertyLast
	};

}

#endif	/* _BIL_TYPES_H_ */
