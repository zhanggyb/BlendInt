/* Test case for FontFace */

#ifndef _SHADER_TEST_H
#define _SHADER_TEST_H

#include <BIL/Widget.hpp>
#include <BIL/Program.hpp>

using namespace BIL;
using namespace BIL::GL;
using namespace std;

class ShaderWidget: public Widget
{
public:

	ShaderWidget (Traceable* parent = NULL);

	virtual ~ShaderWidget ();

protected:

	virtual void update () {}

	virtual void render ();

private:

	Program program_;
	GLuint vbo_triangle_;
};

#endif  /* _SHADER_TEST_H */
