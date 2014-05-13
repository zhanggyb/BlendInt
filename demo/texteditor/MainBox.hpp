/*
 * MainBox.hpp
 *
 *  Created on: 2014年5月13日
 *      Author: zhanggyb
 */

#ifndef MAINBOX_HPP_
#define MAINBOX_HPP_

#include <BlendInt/Gui/VBox.hpp>

#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ToolButton.hpp>

namespace BI = BlendInt;

class MainBox: public BI::VBox
{
public:

	MainBox ();

	virtual ~MainBox ();

private:

	void InitOnce ();

	void OnOpenClick ();

	void OnResize (AbstractWidget* context, int type);

	BI::MenuBar* m_menubar;
	BI::ToolBar* m_toolbar;

	BI::ToolButton* m_open;
};

#endif /* MAINBOX_HPP_ */
