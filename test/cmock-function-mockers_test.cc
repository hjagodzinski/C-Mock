#include <cmock/cmock.h>

#include "add_function_mock.h"
#include "square_root_function_mock.h"

using namespace ::testing;

/**
 * Function add is mocked as long as AddFunctionMock instance exists.
 * Once mock function is destroyed, a call directs to a real function.
 */
TEST(FunctionMockersTest, FunctionIsMockedAsLongAsMockInstanceExists) {

	{
		AddFunctionMock mock;

		EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(Return(12));
		ASSERT_EQ(12, add(1, 2));
	}

	ASSERT_EQ(3, add(1, 2));
}

/**
 * real static mock class field holds pointer to a real function.
 */
TEST(FunctionMockersTest, FunctionMockExportsRealFunctionPointer) {
	EXPECT_EQ(3, AddFunctionMock::real(1, 2));
}

/**
 * Function square_root doesn't exist, but can be mocked
 */
TEST(FunctionMockersTest, FunctionDoesntExist)
{
	{
		SqrtFunctionMock mock;

		EXPECT_FUNCTION_CALL(mock, (9)).WillOnce(Return(3));

		ASSERT_EQ(3, square_root(9));
	}

	EXPECT_THROW(square_root(9), std::logic_error);
}
