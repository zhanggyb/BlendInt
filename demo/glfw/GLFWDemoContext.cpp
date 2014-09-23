/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Image.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/RadioButton.hpp>
#include <BlendInt/Gui/CheckButton.hpp>
#include <BlendInt/Gui/Viewport.hpp>
#include <BlendInt/Gui/ViewportSplitter.hpp>

#include <BlendInt/Stock/Shaders.hpp>

using BI::Stock::Shaders;

GLFWDemoContext::GLFWDemoContext()
: BI::Context()
{
	Initialize ();
}

GLFWDemoContext::~GLFWDemoContext ()
{
}

void GLFWDemoContext::Initialize ()
{
	using namespace BI;

	glm::mat4 matrix(1.f);
	glm::mat4 model;

	Shaders::instance->SetUIProjectionMatrix(matrix);
	Shaders::instance->GetUIProjectionMatrix(model);

//	Shaders::instance->SetUIViewMatrix(matrix);
//	Shaders::instance->GetUIViewMatrix(model);

	if(matrix == model) {
		DBG_PRINT_MSG("%s", "Equal");
	} else {
		DBG_PRINT_MSG("%s", "Not Equal");
	}

	Viewport* vp = Manage(new Viewport);

	AddViewport(vp);

	vp->Resize(1000, 800);
	vp->SetPosition(20, 20);

    FileSelector* fb = Manage(new FileSelector);
    fb->SetPosition(100, 100);
	fb->Resize(800, 600);
	fb->SetRoundType(RoundAll);

	vp->AddContainer(fb);
}
