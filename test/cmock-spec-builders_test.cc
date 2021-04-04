#include <cmock/cmock.h>

#include "math_mockers.h"

TEST(SpecBuildersTest, ExpectFunctionCallCompiles) {
  AddFunctionMock mock;
  EXPECT_FUNCTION_CALL(mock, (1, 2)).Times(0);
}

TEST(SpecBuildersTest, OnFunctionCallCompiles) {
  AddFunctionMock mock;
  ON_FUNCTION_CALL(mock, (1, 2));
}
