#ifndef CMOCK_TEST_FOO_MOCK_CLASS_H_
#define CMOCK_TEST_FOO_MOCK_CLASS_H_

#include "cmock/cmock.h"

#include "foo.h"

class FooMocker : public FunctionMocker<FooMocker> 
{
public:
    MOCK_METHOD2(add, int(int, int));
    MOCK_METHOD2(sub, int(int, int));
};

class CountMocker : public FunctionMocker<CountMocker> 
{
public:
    MOCK_METHOD2(count, int(const char *, char));
};

#endif /* CMOCK_TEST_FOO_MOCK_CLASS_H_ */
