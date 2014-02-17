/*
 * Form.cpp
 *
 *  Created on: 2013年11月19日
 *      Author: zhanggyb
 */



#include <BlendInt/AbstractResizableForm.hpp>


namespace BlendInt {

	void AbstractResizableForm::Resize (unsigned int width, unsigned int height)
	{
		if(size().width() == width && size().height() == height) return;

		Size new_size (width, height);

		Update(FormSize, &new_size);

		set_size(width, height);
	}

	void AbstractResizableForm::Resize (const Size& size)
	{
		if(AbstractResizableForm::size() == size) return;

		Update(FormSize, &size);

		set_size(size);
	}

}
