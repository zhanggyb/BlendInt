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

#define BLENDINT_EVENTS_INIT_ONCE_IN_MAIN Cpp::Events::ProcessInit processInit

/**
 * A macro to disallow the copy constructor and operator= functions
 * This should be used in the private: declarations for a class
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	private: \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#ifdef __UNIX__
#include <string.h>
#define FILE_BASE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
#include <string.h>
#define FILE_BASE_FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#ifdef DEBUG
#include <stdio.h>
#define DBG_PRINT_MSG(fmt, args...) \
	do { \
		fprintf(stderr, "%s:%d:%s(): " fmt "\n", FILE_BASE_NAME, __LINE__, __FUNCTION__, args); \
	} while (0)
#else
#define DBG_PRINT_MSG(fmt, args...) ((void)0)
#endif

#ifdef DEBUG
#define DBG_SET_NAME(obj, str) \
	do {\
		obj->set_name(str); \
	} while (0)
#else
#define DBG_SET_NAME(obj, str) ((void)0)
#endif

#define SETBIT(x,y) (x |= y)
#define CLRBIT(x,y) (x &= ~y)

//#define WIDGET_AA_JITTER 8

/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

namespace BlendInt {

	enum HorizontalAlignment
	{
		AlignTop = 0x0001,
		AlignBottom = 0x0002,
		AlignHorizontalCenter = 0x0004
	};

	enum VerticalAlignment
	{
		AlignLeft = 0x0010,
		AlignRight = 0x0020,
		AlignVerticalCenter = 0x0040
	};

	enum Alignment
	{
		AlignNone = 0x0000, AlignCenter = AlignHorizontalCenter
		        | AlignVerticalCenter
	};

	enum SizePolicy
	{
		DefaultSizePolicy = 0x0,
		PreferredWidth = (1 << 0),
		PreferredHeight = (1 << 1),
		CurrentWidth = (1 << 2),
		CurrentHeight = (1 << 3),
		ExpandX = (1 << 4),
		ExpandY = (1 << 5)
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

	enum FocusStyle {
		FocusOnClick,
		FocusOnHover
	};

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
	 *
	 */
	enum ResponseType {
		Ignore = 0,
		Accept = (1 << 0)
	};

	/**
	 * flags to set which corners will become rounded:
	 *
	 * 1------2
	 * |      		|
	 * 8------4
	 */
	enum RoundCornerType {
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

	/*
	enum FormPropertyType {
		FormSize,
		FormPosition,
		WidgetLayer,
		WidgetVisibility,

		SliderPropertyOrientation,
		SliderPropertyValue,
		SliderPropertyMinimum,
		SliderPropertyMaximum,
		SliderPropertyStep,

		FormPropertyLast
	};
	*/

	enum CursorShape {
		ArrowCursor = 0,	//	The standard arrow cursor.
		UpArrowCursor,		//	1	An arrow pointing upwards toward the top of the screen.
		CrossCursor,		//	2	A crosshair cursor, typically used to help the user accurately select a point on the screen.
		WaitCursor,			//	3	An hourglass or watch cursor, usually shown during operations that prevent the user from interacting with the application.
		IBeamCursor,		//	4	A caret or ibeam cursor, indicating that a widget can accept and display text input.
		SizeVerCursor,		//	5	A cursor used for elements that are used to vertically resize top-level windows.
		SizeHorCursor,		//	6	A cursor used for elements that are used to horizontally resize top-level windows.
		SizeBDiagCursor,	//	7	A cursor used for elements that are used to diagonally resize top-level windows at their top-right and bottom-left corners.
		SizeFDiagCursor,	//	8	A cursor used for elements that are used to diagonally resize top-level windows at their top-left and bottom-right corners.
		SizeAllCursor,		//	9	A cursor used for elements that are used to resize top-level windows in any direction.
		BlankCursor,		//	10	A blank/invisible cursor, typically used when the cursor shape needs to be hidden.
		SplitVCursor,		//	11	A cursor used for vertical splitters, indicating that a handle can be dragged horizontally to adjust the use of available space.
		SplitHCursor,		//	12	A cursor used for horizontal splitters, indicating that a handle can be dragged vertically to adjust the use of available space.
		PointingHandCursor,	//	13	A pointing hand cursor that is typically used for clickable elements such as hyperlinks.
		ForbiddenCursor,	//	14	A slashed circle cursor, typically used during drag and drop operations to indicate that dragged content cannot be dropped on particular widgets or inside certain regions.
		OpenHandCursor,		//	17	A cursor representing an open hand, typically used to indicate that the area under the cursor is the visible part of a canvas that the user can click and drag in order to scroll around.
		ClosedHandCursor,	//	18	A cursor representing a closed hand, typically used to indicate that a dragging operation is in progress that involves scrolling.
		WhatsThisCursor,	//	15	An arrow with a question mark, typically used to indicate the presence of What's This? help for a widget.
		BusyCursor,			//	16	An hourglass or watch cursor, usually shown during operations that allow the user to interact with the application while they are performed in the background.
		DragMoveCursor,		//	20	A cursor that is usually used when dragging an item.
		DragCopyCursor,		//	19	A cursor that is usually used when dragging an item to copy it.
		DragLinkCursor,		//	21	A cursor that is usually used when dragging an item to make a link to it.
		BitmapCursor,		//	24
		CursorShapeLast
	};

}

#endif	/* _BIL_TYPES_H_ */
