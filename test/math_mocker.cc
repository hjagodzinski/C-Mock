#include "math_mocker.h"

CMOCK_MOCK_FUNCTION2(MathMocker, divide, int(int, int));
CMOCK_MOCK_FUNCTION2(MathMocker, substract, int(int, int));
CMOCK_MOCK_FUNCTION1(MathMocker, negate, int(int));
