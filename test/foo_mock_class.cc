#include "foo_mock_class.h"

CMOCK_REDIRECT_TO_MOCK2(FooMocker, add, int(int, int));
CMOCK_REDIRECT_TO_MOCK2(FooMocker, sub, int(int, int));

CMOCK_REDIRECT_TO_MOCK2(CountMocker, count, int(const char *, char));
