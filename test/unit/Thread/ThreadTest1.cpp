#include "ThreadTest1.hpp"
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Core/Thread.hpp>

using namespace BlendInt;

ThreadTest1::ThreadTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ThreadTest1::~ThreadTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */

class Runnable1: public AbstractRunnable
{
public:

	Runnable1()
	: AbstractRunnable(),
	  count_(0)
	{
	}

	virtual ~Runnable1()
	{}

	virtual void* Run ()
	{
		for(int i = 0; i < 5; i++) {
			fprintf(stdout, "count: %d\n", count_);
			sleep(1);
			count_++;
		}

		return 0;
	}

private:

	int count_;
};

TEST_F(ThreadTest1, Foo1)
{
	RefPtr<Runnable1> runnable(new Runnable1);

	Thread thread(runnable, false);
	thread.Start();

	sleep(10);
	thread.Join();
	DBG_PRINT_MSG("%s", "in main thread, end");

	ASSERT_TRUE(true);
}
