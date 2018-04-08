#include <cmock/cmock.h>

#include "add_function_mock.h"

TEST(SpecBuildersTest, ExpectFunctionCallCompiles) {
  AddFunctionMock mock;
  EXPECT_FUNCTION_CALL(mock, (1, 2)).Times(0);
}

TEST(SpecBuildersTest, OnFunctionCallCompiles) {
  AddFunctionMock mock;
  ON_FUNCTION_CALL(mock, (1, 2));
}
