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

#ifndef _BLENDINT_GUI_ABSTRACTFORM_HPP_
#define _BLENDINT_GUI_ABSTRACTFORM_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <vector>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Types.hpp>

#include <BlendInt/Core/Object.hpp>

#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	class Jitter: public std::vector<glm::vec2>
	{
		public:
			Jitter ()
				: std::vector<glm::vec2>()
			{
				resize(8);

				this->operator [](0) = glm::vec2(0.468813, -0.481430);
				this->operator [](1) = glm::vec2(-0.155755, -0.352820);
				this->operator [](2) = glm::vec2(0.219306, -0.238501);
				this->operator [](3) = glm::vec2(-0.393286,-0.110949);
				this->operator [](4) = glm::vec2(-0.024699, 0.013908);
				this->operator [](5) = glm::vec2(0.343805, 0.147431);
				this->operator [](6) = glm::vec2(-0.272855, 0.269918);
				this->operator [](7) = glm::vec2(0.095909, 0.388710);
			}

			~Jitter ()
			{
				clear();
			}
	};

	class UpdateRequest
	{
	public:

		UpdateRequest (int type, const void* data) :
						m_type(type), m_data(data)
		{

		}

		~UpdateRequest ()
		{

		}

		int type () const
		{
			return m_type;
		}

		const void* data () const
		{
			return m_data;
		}

	private:

		UpdateRequest();

		int m_type;
		const void* m_data;
	};

	enum FormRequestType {
		FormSize,
		FormRoundType,
		FormRoundRadius
	};

	/**
	 * @brief Abstract form class
	 *
	 * An AbstractForm is the base class for all GUI elements in 2 dimension.
	 *
	 * The default size is 0 at (0, 0) and the subclass must provide a public
	 * method to resize the form, this method usually need to call Update for redraw
	 * the form.
	 *
	 * @sa Update()
	 */
	class AbstractForm: public Object
	{
	public:

#ifdef DEBUG
		friend class Interface;
#endif

		AbstractForm();

		virtual ~AbstractForm();

		explicit AbstractForm(const AbstractForm& orig);

		AbstractForm& operator = (const AbstractForm& orig)
		{
			m_size = orig.size();
			return *this;
		}

		void Resize (int width, int height);

		void Resize (const Size& size);

		const Size& size () const
		{
			return m_size;
		}

		virtual void Draw (const glm::mat4& mvp, int short gamma = 0) = 0;

		static void SetDefaultBorderWidth (int border);

		static int DefaultBorderWidth ();

		static const Jitter kJit;

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request) = 0;

		/**
		 * @brief preset the size of the form
		 * @param width
		 * @param height
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (int width, int height)
		{
			m_size.set_width(width);
			m_size.set_height(height);
		}

		/**
		 * @brief preset the size of the form
		 * @param size
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (const Size& size)
		{
			m_size = size;
		}

		/**
		 * @brief draw vertices without buffer
		 */
		static void DrawOutlineArray (const float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2], int num);

		static void DrawTriangleFan (const GLint attrib, const GLArrayBuffer* buffer);

		static void DrawTriangleFan (const RefPtr<GLSLProgram>& program, const glm::mat4& mvp, const GLint attrib, GLArrayBuffer* buffer
						);

		static void DrawShadedTriangleFan (const GLint coord, const GLint color, GLArrayBuffer* buffer);

		static void DrawTriangleStrip (const GLint attrib, GLArrayBuffer* buffer);

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

	private:

		static int default_border_width;

		Size m_size;

	};

}

#endif /* _BLENDINT_ABSTRACTFORM_HPP_ */
