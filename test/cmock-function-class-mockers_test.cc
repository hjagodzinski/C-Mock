#include <cmock/cmock.h>

#include "math_mocker.h"

using namespace ::testing;

/**
 * Functions add and substract are mocked as long as MathMocker instance exists.
 * Once mock function is destroyed, call directs to real function.
 */
TEST(FunctionClassMockersTest, FunctionsAreMockedAsLongAsMockerInstanceExists) {

	{
		MathMocker mock;

		EXPECT_CALL(mock, divide(1, 1)).WillOnce(Return(11));
		ASSERT_EQ(11, divide(1, 1));

		EXPECT_CALL(mock, substract(1, 2)).WillOnce(Return(12));
		ASSERT_EQ(12, substract(1, 2));
	}

	ASSERT_EQ(1, divide(1, 1));
	ASSERT_EQ(-1, substract(1, 2));
}

TEST(FunctionClassMockersTest, ExceptionIfNotFound) {

	{
		MathMocker mock;

		EXPECT_CALL(mock, negate(3)).WillOnce(Return(-3));
		ASSERT_EQ(-3, negate(3));
	}

	EXPECT_THROW(negate(3), std::logic_error);
}

