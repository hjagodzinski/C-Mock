#ifndef CMOCK_TEST_FOO_MOCK_H_
#define CMOCK_TEST_FOO_MOCK_H_

#include "cmock/cmock.h"

#include "foo.h"

DECLARE_FUNCTION_MOCK2(FooFunctionMock, foo, int(int, int));

#endif /* CMOCK_TEST_FOO_MOCK_H_ */
