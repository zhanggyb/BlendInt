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
