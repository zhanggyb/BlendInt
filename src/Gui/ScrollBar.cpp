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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ScrollBar.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	ScrollBar::ScrollBar (Orientation orientation)
	: AbstractSlider(orientation),
	  m_vao(0),
	  m_line_width(0),
	  m_pressed(false)
	{
		m_bar.Resize(14, 14);

		if (orientation == Vertical) {
			set_size(18, 200);
			set_expand_y(true);

			m_line_start.set_x(18 / 2);
			m_line_start.set_y(7);
		} else {
			set_size(200, 18);
			set_expand_x(true);

			m_line_start.set_x(7);
			m_line_start.set_y(18 / 2);
		}

		m_line_width = 200 - 7 * 2;

		InitOnce();
	}

	ScrollBar::~ScrollBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool ScrollBar::Update (const UpdateRequest& request)
	{
		if (request.id() == Predefined) {

			switch (request.type()) {
				case FormPosition: {
					// don't care position change
					return true;
				}

				case FormSize: {
					const Size* size_p =
									static_cast<const Size*>(request.data());

					int switch_radius = std::min(m_bar.size().width(),
									m_bar.size().height()) / 2;

					float slot_radius;
					Orientation slot_orient;
					if (orientation() == Vertical) {
						m_line_start.set_x(size_p->width() / 2);
						m_line_start.set_y(switch_radius);
						m_line_width = size_p->height() - switch_radius * 2;
						slot_radius = m_bar.size().width() / 2.f;
						slot_orient = Horizontal;
					} else {
						m_line_start.set_x(switch_radius);
						m_line_start.set_y(size_p->height() / 2);
						m_line_width = size_p->width() - switch_radius * 2;
						slot_radius = m_bar.size().height() / 2.f;
						slot_orient = Vertical;
					}

					const Color& color = themes()->scroll.outline;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					glBindVertexArray(m_vao);
					GenerateShadedFormBuffers(
									*size_p,
									RoundAll,
									slot_radius,
									color,
									shadetop,
									shadedown,
									slot_orient,
									m_slot_inner_buffer.get(),
									m_slot_outline_buffer.get());
					glBindVertexArray(0);


					return true;
				}

				case SliderPropertyValue: {

					return true;
				}

				case SliderPropertyOrientation: {
					//const Orientation* orient_p =
						//			static_cast<const Orientation*>(request.data());

					// TODO:

					//Refresh();

					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	void ScrollBar::Draw (RedrawEvent* event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event->pv_matrix(), pos);

		glm::mat4 local_mvp;

		if (orientation() == Horizontal) {
			// m_line_start.x() == switch_radius
			local_mvp = glm::translate(mvp,
							glm::vec3(0.f,
											(float) (m_line_start.y()
															- m_line_start.x()),
											0.f));
		} else {
			// m_line_start.y() == switch_radius
			local_mvp = glm::translate(mvp,
							glm::vec3(
											(float) (m_line_start.x()
															- m_line_start.y()),
											0.f, 0.f));
		}

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_form_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(local_mvp));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		DrawShadedTriangleFan(0, 1, m_slot_inner_buffer.get());

		glDisableVertexAttribArray(1);

		glm::vec4 color;
		color.r = themes()->scroll.outline.r() / 255.f;
		color.g = themes()->scroll.outline.g() / 255.f;
		color.b = themes()->scroll.outline.b() / 255.f;
		color.a = themes()->scroll.outline.a() / WIDGET_AA_JITTER /255.f;

		program->SetVertexAttrib4fv("color", glm::value_ptr(color));

		DrawTriangleStrip(program, local_mvp, 0, m_slot_outline_buffer.get());

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		if (orientation() == Horizontal) {
			// m_line_start.x() == switch_radius
			local_mvp = glm::translate(mvp,
							glm::vec3(get_position(),
											(float) (m_line_start.y()
															- m_line_start.x()),
											0.0));
		} else {
			// m_line_start.y() == switch_radius
			local_mvp = glm::translate(mvp,
							glm::vec3(
											(float) (m_line_start.x()
															- m_line_start.y()),
											get_position(), 0.0));
		}

		m_bar.Draw(local_mvp);

		event->accept(this);
	}

	void ScrollBar::MouseMoveEvent (MouseEvent* event)
	{
		if (m_pressed) {

			int new_value = value();

			// DO not fire if cursor is out of range, otherwise too many events
			if (GetNewValue(event->position(), &new_value)) {
				set_value(new_value);
				fire_slider_moved_event(value());
				Refresh();
			}

			event->accept(this);

		} else {
			if (CursorOnSlideIcon(event->position())) {
				m_bar.set_highlight(true);

				Refresh();
				event->accept(this);
			} else {
				m_bar.set_highlight(false);
				Refresh();
				event->ignore(this);
			}
		}
	}

	void ScrollBar::MousePressEvent (MouseEvent* event)
	{
		if (CursorOnSlideIcon(event->position())) {
			m_pressed = true;
			fire_slider_pressed();
			event->accept(this);
		} else {
			event->ignore(this);
		}
	}

	void ScrollBar::MouseReleaseEvent (MouseEvent* event)
	{
		if (m_pressed) {
			m_pressed = false;

			if (CursorOnSlideIcon(event->position())) {
				fire_slider_released();
			}

			Refresh();
		}

		event->accept(this);
	}

	void ScrollBar::InitOnce ()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_slot_inner_buffer.reset(new GLArrayBuffer);
		m_slot_outline_buffer.reset(new GLArrayBuffer);

		Size slot_size = m_bar.size();
		float slot_radius;
		Orientation slot_orient;
		if(orientation() == Horizontal) {
			slot_size.set_width(size().width());
			slot_radius = m_bar.size().height() / 2.f;
			slot_orient = Vertical;
		} else {
			slot_size.set_height(size().height());
			slot_radius = m_bar.size().width() / 2.f;
			slot_orient = Horizontal;
		}

		const Color& color = themes()->scroll.inner;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		GenerateShadedFormBuffers(
						slot_size,
						RoundAll,
						slot_radius,
						color,
						shadetop,
						shadedown,
						slot_orient,
						m_slot_inner_buffer.get(),
						m_slot_outline_buffer.get()
						);

		glBindVertexArray(0);
	}

	int ScrollBar::GetSpace ()
	{
		int space = 0;

		if (orientation() == Horizontal) {
			space = size().width() - m_line_start.x() * 2;// m_line_start.x() is the radius of m_switch
		} else {
			space = size().height() - m_line_start.y() * 2;	// m_line_start.y() is the radius of m_switch
		}

		return space;
	}

	bool ScrollBar::CursorOnSlideIcon (const Point& cursor)
	{
		bool ret = false;

		glm::vec2 icon_center;	// slide switch center position

		if (orientation() == Horizontal) {
			icon_center.x = position().x() + m_line_start.x() + get_position();
			icon_center.y = position().y() + m_line_start.y();
		} else {
			icon_center.x = position().x() + m_line_start.x();
			icon_center.y = position().y() + m_line_start.y() + get_position();
		}

		glm::vec2 cursor_pos(cursor.x(), cursor.y());
		float distance = glm::distance(icon_center, cursor_pos);

		if (orientation() == Horizontal && distance <= m_line_start.x()) {
			ret = true;
		} else if (orientation() == Vertical && distance <= m_line_start.y()) {
			ret = true;
		} else {
			ret = false;
		}

		return ret;
	}

	bool ScrollBar::GetNewValue (const Point& cursor, int* vout)
	{
		bool ret = false;

		int offset = 0;
		if (orientation() == Horizontal) {
			offset = cursor.x() - position().x() - m_line_start.x();

		} else {
			offset = cursor.y() - position().y() - m_line_start.y();
		}

		if (offset < 0) {
			*vout = minimum();
		} else if (offset > m_line_width) {
			*vout = maximum();
		} else {
			*vout = (offset * (maximum() - minimum())) / m_line_width;
			ret = true;
		}

		return ret;
	}

}
