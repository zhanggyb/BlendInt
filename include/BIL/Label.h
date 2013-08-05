/*
 * Label.h
 *
 *  Created on: 2013年7月11日
 *      Author: zhanggyb
 */

#ifndef _BIL_LABEL_H_
#define _BIL_LABEL_H_

#include <string>

#include <BIL/Controller.h>
#include <BIL/TextBuffer.h>
#include <BIL/Font.h>

using namespace std;

namespace BIL {

	class Label: public BIL::Controller
	{
	public:

		Label (const wstring& label, Drawable * parent = NULL);

		virtual ~Label ();

		void setLabel (const wstring& label);

		void setTextColor (const RGBAf& fg, const RGBAf& bg)
		{

		}

		void setFont (const Font& font)
		{

		}

		virtual void render (void);

	private:

		TextBuffer _label;

		Coord2f _origin;		/* where start to draw text */

	};

} /* namespace BIL */
#endif /* LABEL_H_ */
