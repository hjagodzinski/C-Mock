#include "foo.h"

int foo(int a1, int a2)
{
	return a1 * a2;
}

int add(int a1, int a2)
{
	return a1 + a2;
}

int sub(int a1, int a2)
{
	return a1 - a2;
}

int count(const char *s, char c)
{
	if (*s=='\0') return 0;
	return (*s==c) + count(s+1, c);
}
