#!/bin/bash
#
# A simple script to create a unit test from template
#

SCRIPT_FILE=`readlink -f $0`
SCRIPT_PATH=`dirname $SCRIPT_FILE`
CODE_PATH=`dirname $SCRIPT_PATH`
CODE_PATH="$(dirname $SCRIPT_PATH)/unit"

function copy_template () {

    if [ $# -eq 0 ]
    then
        return
    fi

    pushd $1 1>/dev/null || return

    # create CMakeLists.txt
    cat << EOF >> CMakeLists.txt
# CMake file for BlendInt project
#

file(GLOB test_SRC "*.cpp")

add_executable(test$1 \${test_SRC})

set(EXECUTABLE_OUTPUT_PATH \${PROJECT_BINARY_DIR}/bin)
target_link_libraries(test$1 BlendInt Common gtest \${LIBS})
EOF

    cat << EOF >> main.cpp
/*
 * Unit test for $1
 */

#include <gtest/gtest.h>
#include <BlendInt/Gui/Context.hpp>

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
EOF

	local hpp_header=`echo "$1TEST1" | tr '[a-z]' '[A-Z]'` 
    cat << EOF >> $1Test1.hpp
#ifndef _${hpp_header}_HPP_
#define _${hpp_header}_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class $1Test1: public testing::Test
{
public:
	$1Test1 ();
	virtual ~$1Test1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _${hpp_header}_HPP_
EOF

	cat << EOF >> $1Test1.cpp
#include "$1Test1.hpp"
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Gui/$1.hpp>

using namespace BlendInt;

$1Test1::$1Test1()
: testing::Test()
{
	// TODO: add constructor code
}

$1Test1::~$1Test1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F($1Test1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("$1 - Foo1", 640, 480);

    UnitTestContext* context = Manage (new UnitTestContext);
	SetContext(context);
	context->Resize(640, 480);
	
    // TODO: add test code here

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
EOF

    popd 1>/dev/null
}

function update_cmakelist_in_unit () {

    if [ $# -eq 0 ]
    then
        return
    fi

    pushd $CODE_PATH 1>/dev/null || return

    TARGET_LINE=""
    TARGET_LINE=`eval sed -n \'/^ *add_subdirectory.*$1\/p\' CMakeLists.txt`
    if [ "$TARGET_LINE"x != ""x ]
    then
        echo "folder: $1 is already appended in unit/CMakeLists.txt"
        popd 1>/dev/null
        return
    fi

    echo "add_subdirectory($1)" >> CMakeLists.txt
    echo "folder: $1 is appended in unit/CMakeLists.txt"

    popd 1>/dev/null
}

function create_single () {
    
    if [ $# -eq 0 ]
    then
        return
    fi

    pushd $CODE_PATH 1>/dev/null || return
    
    if [ -d $1 ]
    then
        echo $1 is already created in $CODE_PATH
        return
    fi

    echo "create test fixture $1 in $CODE_PATH"
    mkdir $1
  
    copy_template $1

    update_cmakelist_in_unit $1

    popd 1>/dev/null
}

for i in $@
do
    create_single $i
done

