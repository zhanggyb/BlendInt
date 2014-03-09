#ifndef _INFO_WIDGET_HPP_
#define _INFO_WIDGET_HPP_

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/MenuItem.hpp>

class InfoWidget: public BlendInt::Widget
{
public:
	
	InfoWidget ();
	
	virtual ~InfoWidget ();

	void onShowText (BlendInt::MenuItem* item);
	
protected:
	
	virtual void Draw (BlendInt::RedrawEvent* event);
	
private:
	
	BlendInt::String m_text;
};
	
#endif	// _INFO_WIDGET_HPP_
