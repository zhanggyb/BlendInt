/**
 * StudioCursor.cpp
 */

#include <assert.h>
#include <boost/filesystem.hpp>

#include "StudioCursor.hpp"

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Image.hpp>

#include <BlendIntConfig.hpp>

void StudioCursor::SetCursor (int cursor_type)
{
	using namespace BlendInt;

	if(cursor_type < 0 || cursor_type >= CursorShapeLast) {
		return;
	}

	switch (cursor_type) {

		case ArrowCursor: {
			glfwSetCursor(window_, arrow_);
			break;
		}

		case CrossCursor: {
			glfwSetCursor(window_, cross_);
			break;
		}

		case SplitVCursor: {
			glfwSetCursor(window_, split_v_);
			break;
		}

		case SplitHCursor: {
			glfwSetCursor(window_, split_h_);
			break;
		}

		case SizeFDiagCursor: {
			glfwSetCursor(window_, top_left_corner_);
			break;
		}

		case SizeBDiagCursor: {
			glfwSetCursor(window_, top_right_corner_);
			break;
		}

		default: {
			glfwSetCursor(window_, arrow_);
			break;
		}
	}
}

void StudioCursor::Initialize ()
{
	using namespace BI;
	namespace fs = boost::filesystem;

	fs::path cursors_path(BLENDINT_INSTALL_PREFIX"/share/BlendInt/datafiles/cursors");

	if(!fs::exists(cursors_path)) {
		cursors_path = fs::path(BLENDINT_PROJECT_SOURCE_DIR"/release/datafiles/cursors");
	}

	if(!fs::exists(cursors_path))
		return;

	GLFWimage cursor;
	Image img;

	std::string filepath;

	filepath = cursors_path.string() + "/" + "left_ptr.png";
	if(img.Read(filepath)) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		arrow_ = glfwCreateCursor(&cursor, 9, 5);
		assert(arrow_ != nullptr);
	} else {
		DBG_PRINT_MSG("%s", "Fail to load cursor");
	}

	filepath = cursors_path.string() + "/" + "hand2.png";
	if(img.Read(filepath)) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		cross_ = glfwCreateCursor(&cursor, 9, 4);
		assert(cross_ != nullptr);
	} else {
		DBG_PRINT_MSG("%s", "Fail to load cursor");
	}

	filepath = cursors_path.string() + "/" + "sb_v_double_arrow.png";
	if(img.Read(filepath)) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		split_v_ = glfwCreateCursor(&cursor, 11, 10);
		assert(split_v_ != nullptr);
	} else {
		DBG_PRINT_MSG("%s", "Fail to load cursor");
	}

	filepath = cursors_path.string() + "/" + "sb_h_double_arrow.png";
	if(img.Read(filepath)) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		split_h_ = glfwCreateCursor(&cursor, 11, 11);
		assert(split_h_ != nullptr);
	} else {
		DBG_PRINT_MSG("%s", "Fail to load cursor");
	}

	filepath = cursors_path.string() + "/" + "top_left_corner.png";
	if(img.Read(filepath)) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		top_left_corner_ = glfwCreateCursor(&cursor, 11, 11);
		assert(top_left_corner_ != nullptr);
	} else {
		DBG_PRINT_MSG("%s", "Fail to load cursor");
	}

	filepath = cursors_path.string() + "/" + "top_right_corner.png";
	if(img.Read(filepath)) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		top_right_corner_ = glfwCreateCursor(&cursor, 12, 11);
		assert(top_right_corner_ != nullptr);
	} else {
		DBG_PRINT_MSG("%s", "Fail to load cursor");
	}

}

void StudioCursor::Release ()
{
	glfwDestroyCursor(arrow_);
	glfwDestroyCursor(cross_);
	glfwDestroyCursor(split_v_);
	glfwDestroyCursor(split_h_);
	glfwDestroyCursor(top_left_corner_);
	glfwDestroyCursor(top_right_corner_);
}
