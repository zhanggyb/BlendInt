#ifndef _INFO_WIDGET_HPP_
#define _INFO_WIDGET_HPP_

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/MenuItem.hpp>

namespace BI=BlendInt;

class InfoWidget: public BlendInt::Widget
{
public:
	
	InfoWidget ();
	
	virtual ~InfoWidget ();

	void onShowText (BlendInt::MenuItem* item);
	
protected:
	
	virtual BI::ResponseType Draw (const BI::Profile& profile);
	
private:
	
	BlendInt::String m_text;
};
	
#endif	// _INFO_WIDGET_HPP_
