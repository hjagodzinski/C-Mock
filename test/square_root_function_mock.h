#ifndef CMOCK_TEST_SQUARE_ROOT_FUNCTION_MOCK_H_
#define CMOCK_TEST_SQUARE_ROOT_FUNCTION_MOCK_H_

#include "cmock/cmock.h"

#include "math.h"

DECLARE_FUNCTION_MOCK1(SqrtFunctionMock, square_root, unsigned int(unsigned int));

#endif /* CMOCK_TEST_SQUARE_ROOT_FUNCTION_MOCK_H_ */
