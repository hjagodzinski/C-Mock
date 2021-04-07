C Mock - Google Mock Extension
==============================

[![Build Status](https://travis-ci.org/hjagodzinski/C-Mock.svg?branch=master)](https://travis-ci.org/hjagodzinski/C-Mock)

Overview
--------

C Mock is [Google Mock][1]'s extension allowing a function mocking.

This is neither a patch to nor fork of Google Mock. This is just a set of headers providing a way to use tools for mock methods with mock functions in tests.

C Mock is not intended to promote a bad design. Its goal is to aid the developers test their code.

Before use of C Mock the following reading is recommended:

* [My code calls a static/global function. Can I mock it?][2]
* [Defeat "Static Cling"][3]

Requirements
------------

* Google Test (for 1.10 support use release [v0.3.1](https://github.com/hjagodzinski/C-Mock/releases/tag/v0.3.1))
* GNU/Linux environment that Google Test supports

Guide
-----

C Mock requires no prior build, it is just a set of header files you include in your code.

### Using CMockMocker class ###

C Mock comes with *CMockMocker* class and the generic macro CMOCK\_MOCK\_FUNCTION.

#### Creating mock ####

C Mock has no knowledge whether a mocked function is declared with a name mangling - whether this is a pure C function or a C++ function. Therefore C Mock does not redeclare mocked function. Original function prototype declaration should be used (i.e. use of original function header file).

Suppose you want to mock *int divide(int, int)* and *int substract(int, int)* functions declared in *math.h* header file. To do that you can create a single mock class for both of them called *MathMocker*:

* *math_mocker.h*:

```cpp
#include <cmock/cmock.h>

#include "math.h"

class MathMocker : public CMockMocker<MathMocker>
{
public:
    MOCK_METHOD(int, divide, (int, int));
    MOCK_METHOD(int, substract, (int, int));
};
```

* *math_mocker.cc*:

```cpp
#include "math_mocker.h"

CMOCK_MOCK_FUNCTION(MathMocker, int, divide, (int, int));
CMOCK_MOCK_FUNCTION(MathMocker, int, substract, (int, int));
```

#### Specifying expectations ####

To specify the expectations you use Google Mock's macros as you normally would do. The functions are mocked as long as its corresponding mocker class instance exists. This allows to easily control when the functions are mocked. If a mocker class instance does not exist, the real function is called.

```cpp
{
    MathMocker mock;

    EXPECT_CALL(mock, divide(1, 1)).WillOnce(Return(11));
    ASSERT_EQ(11, divide(1, 1)); // calling the mock

    EXPECT_CALL(mock, substract(1, 2)).WillOnce(Return(12));
    ASSERT_EQ(12, substract(1, 2)); // calling the mock
}

ASSERT_EQ(1, divide(1, 1)); // calling the real function
ASSERT_EQ(-1, substract(1, 2)); // calling the real function
```

Before the generic `CMOCK_MOCK_FUNCTION` macro was introduced, function mocks where created using macros `CMOCK_MOCK_FUNCTIONn`. These macros are still supported, though migration to the new `CMOCK_MOCK_FUNCTION` is recommended. For instance `CMOCK_MOCK_FUNCTION(MathMocker, int, divide, (int, int))` is equivalent to `CMOCK_MOCK_FUNCTION2(MathMocker, divide, int(int, int))`.

### Using macros (deprecated) ###

C Mock comes with four macros:

* `DECLARE_FUNCTION_MOCKn` and `IMPLEMENT_FUNCTION_MOCKn`
* `EXPECT_FUNCTION_CALL`
* `ON_FUNCTION_CALL`

These macros do what theirs' method counterparts do `MOCK_METHODn`, `EXPECT_CALL` and `ON_CALL`, respectively. There are small differences though.

#### Creating mock ####

In fact both `DECLARE_FUNCTION_MOCKn` and `IMPLEMENT_FUNCTION_MOCKn` stand for a series of macros for defining and implementing a function mock, respectively. These macros take three arguments: a mock class name, a function name and a function prototype.

C Mock internally redefines a function being mocked. Because only one implementation of a function might exist in an executable, a splitting of a declaration and implementation is necessary. Especially, if mocking a certain function takes place in a more than one compilation unit. Therefore declaration should be put in a header file whereas implementation in a source file.

C Mock has no knowledge whether a mocked function is declared with a name mangling - whether this is a pure C function or a C++ function. Therefore C Mock does not redeclare mocked function. Original function prototype declaration should be used (i.e. use of original function header file).

Suppose you want to mock *int add(int, int)* function declared in *math.h* header file. To do that you can create *AddFunctionMock* mock class:

* *add_function_mock.h*

    ```cpp
    #include <cmock/cmock.h>

    #include "math.h" // use original function declaration

    DECLARE_FUNCTION_MOCK2(AddFunctionMock, add, int(int, int));
    ```

* *add_function_mock.cc*

    ```cpp
    IMPLEMENT_FUNCTION_MOCK2(AddFunctionMock, add, int(int, int));
    ```

#### Specifying expectations ####

`EXPECT_FUNCTION_CALL` and `ON_FUNCTION_CALL` do exactly what theirs' method equivalents. Both take two arguments: a mock class instance and the arguments you expect - there is no need to repeat function name since it is already known at this point. Suppose we expect *add* function to be called once with arguments *1* and *2*, and want it to return *12*:

```cpp
AddFunctionMock mock;
EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(::testing::Return(12));
```

A function is mocked as long as its corresponding mock class instance exists. This allows to control when a function is mocked.

```cpp
{
    AddFunctionMock mock;
    add(1, 2); // calling the mock
}

add(1, 2); // calling the real function
```

Still you might want to use a mocked function's real implementation. Each mock class exports static *real* class field which holds pointer to a real function.

```cpp
AddFunctionMock mock;
EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(::testing::Invoke(AddFunctionMock::real));
foo(1, 2); // calling the real function
```

### Building ###

C Mock uses specific GNU/Linux features internally and a test building requires a few additional steps.

Firstly, all functions you want to mock must be compiled into a dynamic library. If it includes your project-specific functions you must put them into a dynamic library as well. In such circumstances it seems reasonable to build all code under test as a dynamic library. Selecting only those parts that you are going to mock might be tedious and cumbersome.

Secondly, you must pass the following options to a linker when building a test executable:

* *-rdynamic* - adds all symbols to dynamic symbol table
* *-Wl,--no-as-needed* - forces to link with library during static linking when there are no dependencies to it
* *-ldl* - links with dynamic linking loader library

C Mock comes with *cmock-config* tool to hide all these details away from you. Run

```
cmock-config --cflags [path to Google Test]
```

and

```
cmock-config --libs [path to Google Test]
```

to get the compilations and linker options, respectively.

Since [it is not recommended to install a pre-compiled version of Google Test][4] many distributions don't provide pre-compiled Google Test anymore. You need to download and compile Google Test manually as described in [Google Test][1]. Optional second command argument is a path to a directory contaning downloaded and built Google Test.

Let's say you built a code under test into *libfoo.so* and put a test code in *bar.cc*. To build your test executable you would run:

```
g++ `cmock-config --cflags` -c bar.cc -o bar.o
g++ `cmock-config --libs` -pthread -lfoo bar.o -o bar # Google Test requires -pthread
```

When building code under test as a dynamic library it is handy to specify *soname* as an absolute path name. Then when test executable is run no additional environment setup is required for the dynamic linking loader to locate your library (i.e. setting LD\_LIBRARY\_PATH).

Installation
------------

To install run:

```
make install
```

To uninstall run:

```
make uninstall
```

By default installation *PREFIX* is */usr/local*. You can change it as follows:

```
make install PREFIX=/usr
```

Test
----

If your environment is supported and Google Test is installed, the following commands should succeed:

```
make
make test
```

Optionally you can provide a directory containing downloaded and built Google Test by setting _GTEST\_DIR_:

```
GTEST_DIR=/path/to/googletest make
```

Tests are quite simple and are a good source of usage examples.

References
----------
* [Google Mock][1]
* [My code calls a static/global function. Can I mock it?][2]
* [Defeat "Static Cling"][3]
* [Why is it not recommended to install a pre-compiled copy of Google Test (for example, into /usr/local)][4]

[1]: https://github.com/google/googletest "Google Test"
[2]: https://github.com/google/googletest/blob/3cfb4117f7e56f8a7075d83f3e59551dc9e9f328/googlemock/docs/gmock_faq.md#my-code-calls-a-staticglobal-function-can-i-mock-it "My code calls a static/global function. Can I mock it?"
[3]: https://googletesting.blogspot.com/2008/06/defeat-static-cling.html "Defeat \"Static Cling\""
[4]: https://github.com/google/googletest/blob/36066cfecf79267bdf46ff82ca6c3b052f8f633c/googletest/docs/faq.md#why-is-it-not-recommended-to-install-a-pre-compiled-copy-of-google-test-for-example-into-usrlocal "Why is it not recommended to install a pre-compiled copy of Google Test (for example, into /usr/local)"
