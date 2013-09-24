/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_EVENT_HPP_
#define _BILO_EVENT_HPP_

namespace BILO {

	class Drawable;

	class Event
	{
	public:

		Event()
		: m_accepted(false), m_ignored(false), m_object(0)
		{
		}

		void accept (Drawable* object)
		{
			m_accepted = true;
			m_object = object;
		}

		/**
		 * @brief ignore the current event loop
		 */
		void ignore (Drawable* object)
		{
			m_ignored = true;
			m_object = object;
		}

		bool accepted () const
		{
			return m_accepted;
		}

		bool ignored () const
		{
			return m_ignored;
		}

		Drawable* object ()
		{
			return m_object;
		}

	protected:

		bool m_accepted;

		bool m_ignored;

		/**
		 * @brief Drawable object who react to this event
		 */
		Drawable* m_object;
	};

} /* namespace BILO */

#endif	/* _BILO_EVENT_HPP_ */
