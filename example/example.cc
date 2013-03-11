#include <cmock/cmock.h>

#include "foo.h"

using namespace testing;

MOCK_FUNCTION2(FooFunctionMock, foo, int(int, int));

/**
 * Function foo is mocked as long as FooFunctionMock instance exists.
 * When the test ends it is automatically destroyed.
 */
TEST(FooTest, CallMockedFoo)
{
	FooFunctionMock mock;
	EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(Return(3));
	ASSERT_EQ(3, foo(1, 2));
}

/**
 * During the test no FooFunctionMock is created,
 * so call to real function is made.
 */
TEST(FooTest, CallRealFoo)
{
	ASSERT_EQ(2, foo(1, 2));
}
