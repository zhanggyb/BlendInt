#ifndef _INFO_WIDGET_HPP_
#define _INFO_WIDGET_HPP_

#include <BlendInt/Widget.hpp>
#include <BlendInt/String.hpp>
#include <BlendInt/MenuItem.hpp>

class InfoWidget: public BlendInt::Widget
{
public:
	
	InfoWidget ();
	
	virtual ~InfoWidget ();

	void onShowText (BlendInt::MenuItem* item);
	
protected:
	
	virtual void Draw ();
	
private:
	
	BlendInt::String m_text;
};
	
#endif	// _INFO_WIDGET_HPP_