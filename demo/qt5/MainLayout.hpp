/**
 * Main Layout
 */

#ifndef _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_
#define _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_

#include <BlendInt/Gui/VLayout.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>

namespace BI = BlendInt;

class MainLayout: public BI::VLayout
{
public:

	MainLayout ();
	
	virtual ~MainLayout ();

private:

	void InitOnce ();

	BI::Viewport3D* m_view3d;
	BI::Button* m_button;
};

#endif
