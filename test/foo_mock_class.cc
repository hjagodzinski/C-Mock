#include "foo_mock_class.h"

IMPLEMENT_FUNCTION_MOCK_N2(FooMocker, add, int(int, int));
IMPLEMENT_FUNCTION_MOCK_N2(FooMocker, sub, int(int, int));

IMPLEMENT_FUNCTION_MOCK_N2(CountMocker, count, int(const char *, char));
