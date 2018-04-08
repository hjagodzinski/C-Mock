#include <cmock/cmock.h>

#include "foo_mock_class.h"

using namespace ::testing;

/**
 * Function foo is mocked as long as FooFunctionMock instance exists.
 * Once mock function is destroyed, call directs to real function.
 */
TEST(FunctionClassMockersTest, FunctionIsMockedAsLongAsMockInstanceExists) {

	{
		FooMocker mock;

		EXPECT_CALL(mock, add(1, 1)).WillOnce(Return(11));
		ASSERT_EQ(11, add(1, 1));

		EXPECT_CALL(mock, sub(1, 1)).WillOnce(Return(7));
		ASSERT_EQ(7, sub(1, 1));
	}

	ASSERT_EQ(2, add(1, 1));
	ASSERT_EQ(0, sub(1, 1));
}

