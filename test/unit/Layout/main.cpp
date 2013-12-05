/*
 * Unit test for Layout
 */

#include "HLayoutTest1.hpp"
#include <BlendInt/Interface.hpp>

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
