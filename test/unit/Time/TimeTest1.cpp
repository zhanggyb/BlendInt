#include "TimeTest1.hpp"
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Core/Time.hpp>

using namespace BlendInt;

TimeTest1::TimeTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TimeTest1::~TimeTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TimeTest1, Foo1)
{
	uint64_t time = Time::GetMicroSeconds();

	DBG_PRINT_MSG("current time (in microseconds): %ld", time);

	Time::SaveCurrent();

	sleep(1);

	time = Time::GetIntervalOfSeconds();
	
	DBG_PRINT_MSG("time interval (seconds): %ld", time);

	sleep(1);

	time = Time::GetIntervalOfMilliseconds();

	DBG_PRINT_MSG("time interval (milliseconds): %ld", time);

	sleep(1);

	time = Time::GetIntervalOfMicroseconds();

	DBG_PRINT_MSG("time interval (microseconds): %ld", time);

	ASSERT_TRUE(true);
}
