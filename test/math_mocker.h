#ifndef CMOCK_TEST_MATH_MOCKER_H_
#define CMOCK_TEST_MATH_MOCKER_H_

#include "cmock/cmock.h"

#include "math.h"

class MathMocker : public CMockMocker<MathMocker>
{
public:
    MOCK_METHOD(int, divide, (int, int));
    MOCK_METHOD(int, substract, (int, int));
    MOCK_METHOD(int, negate, (int));
};

#endif /* CMOCK_TEST_MATH_MOCKER_H_ */
