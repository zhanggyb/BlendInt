#include "AbstractFormTest1.hpp"
#include <BlendInt/Gui/AbstractForm.hpp>

using namespace BlendInt;

AbstractFormTest1::AbstractFormTest1()
: testing::Test()
{
	// TODO: add constructor code
}

AbstractFormTest1::~AbstractFormTest1()
{
	// TODO: add destructor code
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result: 
 */
TEST_F(AbstractFormTest1, GenerateRectVertices)
{
	Init ();

    GLFWwindow* win = CreateWindow("AbstractForm - GenerateRectVertices", 640, 480);

    // TODO: add test code here
	
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundNone, 5.f, &inner, &outer);

    std::cout << "inner size: " << inner.size() << std::endl;
    for(size_t i = 0; i < inner.size(); i += 2)
    {
        std::cout << inner[i] << " " << inner[i + 1] << std::endl;
    }

    std::cout << "outer size: " << outer.size() << std::endl;
    for(size_t i = 0; i < outer.size(); i += 2)
    {
        std::cout << outer[i] << " " << outer[i + 1] << std::endl;
    }

    std::cout << "total verts: " << sum.total << std::endl;

    std::vector<GLfloat> strip;
    AbstractForm::GenerateTriangleStripVertices(inner, outer, sum.total, &strip);

    std::cout << "strip size: " << strip.size() << std::endl;
    for(size_t i = 0; i < strip.size(); i += 2)
    {
    	std::cout << strip[i] << " " << strip[i + 1] << std::endl;
    }


    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
