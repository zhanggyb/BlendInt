/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <BlendInt/Stock/Cursor.hpp>

namespace BlendInt {

	Cursor* Cursor::instance = 0;

	bool Cursor::Initialize ()
	{
		instance = new Cursor;

		instance->RegisterCursorType(new CursorType);

		return true;
	}

	void Cursor::Release ()
	{
		if(instance) {
			delete instance;
			instance = 0;
		}
	}

	Cursor::Cursor ()
	: cursor_type_(0)
	{

	}

	Cursor::~Cursor ()
	{
		if(cursor_type_) {
			delete cursor_type_;
			cursor_type_ = 0;
		}
	}

	void Cursor::RegisterCursorType (CursorType* cursor_type)
	{
		if(cursor_type == 0) return;
		
		if(cursor_type_) {
			delete cursor_type_;
			cursor_type_ = 0;
		}

		cursor_type_ = cursor_type;
	}

	void Cursor::SetCursor (int cursor_type)
	{
		cursor_type_->SetCursor(cursor_type);
	}

	void Cursor::PushCursor ()
	{
		if(cursor_type_) {
			cursor_stack_.push(cursor_type_->current_cursor());
		}
	}

	void Cursor::PushCursor (int cursor_type)
	{
		cursor_stack_.push(cursor_type);
	}

	void Cursor::PopCursor ()
	{
		/*
		int cursor = ArrowCursor;

		if(!cursor_stack_.empty()) {
			cursor = cursor_stack_.top();
			cursor_stack_.pop();
		}

		return cursor;
		*/
		if(cursor_type_) {
			int cursor = ArrowCursor;

			if(!cursor_stack_.empty()) {
				cursor = cursor_stack_.top();
				cursor_stack_.pop();
			}

			cursor_type_->SetCursor(cursor);
		}
	}

}