#include <cmock/cmock.h>

#include "foo_mock.h"

using namespace ::testing;

/**
 * Function foo is mocked as long as FooFunctionMock instance exists.
 * Once mock function is destroyed, call directs to real function.
 */
TEST(FunctionMockersTest, FunctionIsMockedAsLongAsMockInstanceExists) {

	{
		FooFunctionMock mock;

		EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(Return(3));
		ASSERT_EQ(3, foo(1, 2));
	}

	ASSERT_EQ(2, foo(1, 2));
}

/**
 * real static mock class field holds pointer to real function.
 */
TEST(FunctionMockersTest, FunctionMockExportsRealFunctionPointer) {
	EXPECT_EQ(2, FooFunctionMock::real(1, 2));
}
