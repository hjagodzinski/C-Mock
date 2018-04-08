C Mock - Google Mock Extension
==============================

[![Build Status](https://travis-ci.org/hjagodzinski/C-Mock.svg?branch=master)](https://travis-ci.org/hjagodzinski/C-Mock)

Overview
--------

C Mock is [Google Mock][1]'s extension allowing a function mocking.

This is neither a patch to nor fork of Google Mock. This is just a set of headers providing a way to use tools for mock methods with mock functions in tests.

C Mock is not intended to promote a bad design. Its goal is to aid the developers test their code.

Before use of C Mock following reading is recommended:

* [My code calls a static/global function. Can I mock it?][2]
* [Defeat "Static Cling"][3]

Requirements
------------

* Google Mock (1.6 or newer)
* GNU/Linux platform that Google Mock supports

Guide
-----

C Mock requires no prior build, it is just a set of header files. What it really gives you is four macros:

* DECLARE\_FUNCTION\_MOCK\*
* IMPLEMENT\_FUNCTION\_MOCK\*
* EXPECT\_FUNCTION\_CALL
* ON\_FUNCTION\_CALL

These macros do what theirs' method counterparts do MOCK\_METHOD\_\*, EXPECT\_CALL and ON\_CALL, respectively. There are small differences though.

### Creating mock ###

Both DECLARE\_FUNCTION\_MOCK\* and IMPLEMENT\_FUNCTION\_MOCK\* in fact stand for a series of macros for defining and implementing C function mocks, respectively. These macros take three arguments: a mock class name, a function name and a function prototype.

C Mock internally redefines a function being mocked. Because only one implementation of a function might exist in executable, splitting of declaration and implementation is necessary. Especially, if mocking a certain function takes place in a more than one compilation unit. Therefore declaration should be put in a header file whereas implementation in a source file.

C Mock does has no knowledge whether a mocked function is declared with name mangling - whether this is a pure C function or a C++ function. Therefore C Mock does not redeclare mocked function. Original function prototype declaration should be used (i.e. use of original function header file).

Suppose you want to mock *int foo(int, int)* function declared in *foo.h* header file and name mock class *FooFunctionMock*. You could create two files, one header with declaration and one source file with implementation:

* *foo_mock.h*

            #include <cmock/cmock.h>

            #include "foo.h" // use original function declaration

            DECLARE_FUNCTION_MOCK2(FooFunctionMock, foo, int(int, int));

* *foo_mock.cc*

            IMPLEMENT_FUNCTION_MOCK2(FooFunctionMock, foo, int(int, int));

### Specifying expectations ###

EXPECT\_FUNCTION\_CALL and ON\_FUNCTION\_CALL do exactly what theirs' method equivalents. Both take two arguments: a mock class instance and the arguments you expect - there is no need to repeat function name since it is already known at this point. Suppose we expect *foo* function to be called once with arguments *1* and *2*, and want it to return *3*:

        FooFunctionMock mock;
        EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(::testing::Return(3));

Function is mocked as long as its corresponding mock class instance exists. This allows to control when a function is mocked.

        {
            {
                FooFunctionMock mock;
                /* ... */
                foo(1, 2); // call mock
            }

            foo(1, 2); // call real function
        }

Still you might want to use a mocked function real implementation. Each mock class exports static *real* class field which holds pointer to a real function.

        FooFunctionMock mock;
        EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(::testing::Invoke(FooFunctionMock::real));
        foo(1, 2); // call real function

### Building ###

C Mock uses internally some tricks specific to GNU/Linux platform and a test building requires a few additional steps.

Firstly, all functions you want to mock must be in a dynamic library. If it includes your project-specific functions you must put them into a dynamic library. In such a circumstances it seems reasonable to build all code under test as a dynamic library. Selecting only those parts that you are going to mock might be tedious and cumbersome.

Secondly, you must pass the following options to a linker when building a test executable:

* *-rdynamic* - adds all symbols to dynamic symbol table
* *-Wl,--no-as-needed* - links with library when during static linking there are no dependencies to it
* *-ldl* - links with dynamic linking loader library

C Mock comes with *cmock-config* tool to hide all these details away from you. Run

        cmock-config --cflags

and

        cmock-config --libs [path to libgmock [path to libgtest]]

to get the compilations and linker options, respectively.

Note: Since it is [not recommended to install a pre-compiled version of Google Test][4] many distributions don't provide such libs anymore. You need to download and compile those libs as described in [Google Test][1].
For the linker to find libgmock and libgtest you can pass the paths to those libs to the cmock-config script.
If you omit the path to libgtest it defaults to "pathToLibgmock/libgtest".

Let's say you built a code under test into *libfoo.so* and put a test code in *bar.cc*. To build your test executable you would run:

        g++ `cmock-config --cflags` -c bar.cc -o bar.o
        g++ `cmock-config --libs` -pthread -lfoo bar.o -o bar # Google Test requires -pthread

When building code under test as a dynamic library it is handy to specify *soname* as an absolute path name. Then when test executable is run no additional environment setup is required for the dynamic linking loader to locate your library (i.e. setting LD\_LIBRARY\_PATH).

Installation
------------

To install run:

        make install

To uninstall run:

        make uninstall

By default installation *PREFIX* is */usr/local*. You can change it as follows:

        make install PREFIX=/usr

Test
----

If your platform is supported and Google Test is installed, the following commands should succeed:

        make
        make test

Tests are quite simple and are a good source of usage examples.

References
----------
* [Google Mock][1]
* [My code calls a static/global function. Can I mock it?][2]
* [Defeat "Static Cling"][3]
* [Why is it not recommended to install a pre-compiled copy of Google Test (for example, into /usr/local)][4]

[1]: https://github.com/google/googletest "Google Test"
[2]: https://github.com/google/googlemock/blob/master/googlemock/docs/FrequentlyAskedQuestions.md#my-code-calls-a-staticglobal-function--can-i-mock-it "My code calls a static/global function. Can I mock it?"
[3]: https://googletesting.blogspot.com/2008/06/defeat-static-cling.html "Defeat "Static Cling"
[4]: https://github.com/google/googletest/blob/master/googletest/docs/FAQ.md#why-is-it-not-recommended-to-install-a-pre-compiled-copy-of-google-test-for-example-into-usrlocal "Why is it not recommended to install a pre-compiled copy of Google Test (for example, into /usr/local)"
