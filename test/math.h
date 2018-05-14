#ifndef CMOCK_TEST_MATH_H_
#define CMOCK_TEST_MATH_H_

#ifdef __cplusplus
extern "C" {
#endif

int add(int a1, int a2);
int divide(int a1, int a2);
int substract(int a1, int a2);

/* These function aren't implemented, but can be mocked */
int negate(int n);
unsigned int square_root(unsigned int n);

#ifdef __cplusplus
}
#endif

#endif /* CMOCK_TEST_MATH_H_ */
