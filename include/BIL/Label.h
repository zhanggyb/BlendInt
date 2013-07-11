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
#include <BIL/Color.h>
#include <BIL/Font.h>

using namespace std;

namespace BIL {

	class Label: public BIL::Controller
	{
	public:

		Label (BasicObject *parent = NULL);

		Label (const string& label, BasicObject * parent = NULL);

		virtual ~Label ();

		void setColor (const Color& color);

		void setFont (const Font& font);

	protected:

		virtual void render (void);

	private:

		string _label;

		Font _font;

		Color _color;	/** Text color */

		Color _background;	/** background color */

	};

} /* namespace BIL */
#endif /* LABEL_H_ */
