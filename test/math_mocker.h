#ifndef CMOCK_TEST_MATH_MOCKER_H_
#define CMOCK_TEST_MATH_MOCKER_H_

#include "cmock/cmock.h"

#include "math.h"

class MathMocker : public CMockMocker<MathMocker>
{
public:
    MOCK_METHOD2(divide, int(int, int));
    MOCK_METHOD2(substract, int(int, int));
    MOCK_METHOD1(negate, int(int));
};

#endif /* CMOCK_TEST_MATH_MOCKER_H_ */
