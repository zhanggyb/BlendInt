#ifndef _WORKSPACETEST1_HPP_
#define _WORKSPACETEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class WorkspaceTest1: public testing::Test
{
public:
	WorkspaceTest1 ();
	virtual ~WorkspaceTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _WORKSPACETEST1_HPP_
