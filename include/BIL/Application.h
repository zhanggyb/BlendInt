/*
 * This file is part of BIL (Blender Interface Library).
 * 
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

/*
 * Application.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_APPLICATION_H_
#define _BIL_APPLICATION_H_

#include <string>
#include <BIL/Window.h>

namespace BIL {

    struct GLFWVersion
    {
        int major;
        int minor;
        int rev;
    };

    class Application
    {
    public:
        Application ();

        bool initialize (bool nls = true);

        void terminate (void)
        {
            glfwTerminate();
        }

        /**
         * @brief Get GLFW version
         * @return
         */
        GLFWVersion getVersion (void);

        /**
         *  @brief Get GLFW version
         * @return
         */
        std::string getVersionString (void);

        void setMainWindow (Window *window);

        void run (void);

        virtual ~Application ();

        static std::string glStrVersion;

        static GLfloat glVersion;

    private:

        static void cbError (int error, const char* description);

        Window *_mainWindow;
    };

} /* namespace BIL */
#endif /* APPLICATION_H_ */
