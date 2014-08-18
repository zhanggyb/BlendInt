#
#

FIND_PACKAGE(Doxygen)
if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please
    install it correctly")
endif()

configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
add_custom_target(Docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
    SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)

install(DIRECTORY "${PROJECT_BINARY_DIR}/Documentation/html" DESTINATION
    share/doc/BlendInt)
