/**
 * BlendInt demo
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Types.hpp>

#include <BlendInt/Stock/Cursor.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "../Common/GLFWCursor.hpp"
#include "../Common/GLFWWindow.hpp"

void GenerateCrossIconVertices (float min = 0.12f, float max = 0.9f)
{
	glm::vec4 p1(-min, max, 0.f, 1.f);
	glm::vec4 p2(-min, min, 0.f, 1.f);
	glm::vec4 p3(-max, min, 0.f, 1.f);
	glm::vec4 p4(-max, -min, 0.f, 1.f);
	glm::vec4 p5(-min, -min, 0.f, 1.f);
	glm::vec4 p6(-min, -max, 0.f, 1.f);
	glm::vec4 p7(min, -max, 0.f, 1.f);
	glm::vec4 p8(min, -min, 0.f, 1.f);
	glm::vec4 p9(max, -min, 0.f, 1.f);
	glm::vec4 p10(max, min, 0.f, 1.f);
	glm::vec4 p11(min, min, 0.f, 1.f);
	glm::vec4 p12(min, max, 0.f, 1.f);

	glm::mat4 rotate_mat = glm::rotate(glm::mat4(1.f), (glm::mediump_float)(- M_PI / 4.f), glm::vec3(0.f, 0.f, 1.f));

	glm::vec4 r1 = rotate_mat * p1;
	glm::vec4 r2 = rotate_mat * p2;
	glm::vec4 r3 = rotate_mat * p3;
	glm::vec4 r4 = rotate_mat * p4;
	glm::vec4 r5 = rotate_mat * p5;
	glm::vec4 r6 = rotate_mat * p6;
	glm::vec4 r7 = rotate_mat * p7;
	glm::vec4 r8 = rotate_mat * p8;
	glm::vec4 r9 = rotate_mat * p9;
	glm::vec4 r10 = rotate_mat * p10;
	glm::vec4 r11 = rotate_mat * p11;
	glm::vec4 r12 = rotate_mat * p12;

	printf("copy to VertexIcon: \n");
	printf("------------------------- \n");
	printf("{%f, %f},\n", r1.x, r1.y);
	printf("{%f, %f},\n", r2.x, r2.y);
	printf("{%f, %f},\n", r3.x, r3.y);
	printf("{%f, %f},\n", r4.x, r4.y);
	printf("{%f, %f},\n", r5.x, r5.y);
	printf("{%f, %f},\n", r6.x, r6.y);
	printf("{%f, %f},\n", r7.x, r7.y);
	printf("{%f, %f},\n", r8.x, r8.y);
	printf("{%f, %f},\n", r9.x, r9.y);
	printf("{%f, %f},\n", r10.x, r10.y);
	printf("{%f, %f},\n", r11.x, r11.y);
	printf("{%f, %f}\n", r12.x, r12.y);
	printf("------------------------- \n");
}

int main(int argc, char* argv[])
{
	using namespace BlendInt;

	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 1280, 800);

	Cursor::instance->RegisterCursorType (new GLFWCursor(win));

	GLFWDemoContext* context = Manage (new GLFWDemoContext(win));
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	DBG_PRINT_MSG("sizeof short: %ld", sizeof(unsigned short));
	DBG_PRINT_MSG("view size: %ld", sizeof(AbstractView));

	RunLoop(win);
	Terminate();

	return 0;
}

