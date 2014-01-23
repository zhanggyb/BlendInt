/*
 * Unit test for Window
 */

#include <gtest/gtest.h>
#include <BlendInt/Interface.hpp>

// Begin test code
#include <sys/select.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// End test code

int main (int argc, char* argv[])
{
    /*
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
    */

// Begin test code

    char tmpfile[20];
    strncpy(tmpfile, "/tmp/testXXXXXX", 20);
    int tmp_fd = mkstemp(tmpfile);

    if(tmp_fd < 0) {
        printf("Cannot create temp file\n");
        exit(1);
    }

    sleep(100);

    unlink(tmpfile);
    close (tmp_fd);
    return 0;

// End test code
}
