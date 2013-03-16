C Mock - Google Mock Extension
==============================

Overview
--------

C Mock is [Google Mock][1] extension allowing for mocking functions.

This is not either a patch to nor fork of Google Mock. This is just a set of headers providing a way of using tools for mock methods with mock functions in tests.

C Mock is not intended to promote bad design. Its goal is to help people test their code.

Before use of C Mock following reading is recommended:

* [My code calls a static/global function. Can I mock it?][2]
* [Defeat "Static Cling"][3]

Requirements
------------

* Google Mock (1.6 or newer)
* GNU/Linux platform that Google Mock supports

Guide
-----

C Mock requires no prior build. As already mentioned it is just a set of header files. What it really gives you are four macros:

* DECLARE\_FUNCTION\_MOCK\*
* IMPLEMENT\_FUNCTION\_MOCK\*
* EXPECT\_FUNCTION\_CALL
* ON\_FUNCTION\_CALL

These macros do what theirs' method counterparts do MOCK\_METHOD\_\*, EXPECT\_CALL and ON\_CALL, respectively. There are small differences though.

### Creating mock ###

Both DECLARE\_FUNCTION\_MOCK\* and IMPLEMENT\_FUNCTION\_MOCK\* in fact stand for a series of macros for defining and implementing C function mocks, respectively. These macros take three arguments: mock class name, function name and function prototype.

C Mock internally redefines function being mocked. Because only one implementation of a function might exist in executable, splitting of declaration and implementation is necessary. Especially, if mocking of a certain function is happening in a more than one compilation unit. Therefore declaration should be put in a header file whereas implementation in a source file.

C Mock does not know whether mocked function is declared with name mangling - whether this is a pure C function or a C++ function. Therefore C Mock does not redeclare mocked function. Original function prototype declaration should be used (i.e. use of original function header file).

Suppose you want to mock *int foo(int, int)* function declared in *foo.h* header file and name mock class *FooFunctionMock*. You could create two files, one header with declaration and one source file with implementation:

* *foo_mock.h*

            #include "foo.h" // use original function declaration

            DECLARE_FUNCTION_MOCK2(FooFunctionMock, foo, int(int, int));

* *foo_mock.cc*

            IMPLEMENT_FUNCTION_MOCK2(FooFunctionMock, foo, int(int, int));

### Specifying expectations ###

EXPECT\_FUNCTION\_CALL and ON\_FUNCTION\_CALL do exactly what theirs' method equivalents. Both take two arguments: mock class instance and arguments you expect - there is no need to repeat function name since it is already known at this point. Suppose we expect *foo* function to be called once with arguments *1* and *2*, and want it to return *3*:

        FooFunctionMock mock;
        EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(::testing::Return(3));

Function is mocked as long as its corresponding mock class instance exists. This means function is mocked only when required.

        {
            {
                FooFunctionMock mock;
                /* ... */
                foo(1, 2); // call mock
            }

            foo(1, 2); // call real function
        }

Event though you mock function, you might want to use its real implementation. Each mock class exports static *real* class field which holds pointer to real function.

        FooFunctionMock mock;
        EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(::testing::Invoke(FooFunctionMock::real));
        foo(1, 2); // call real function

### Building ###

C Mock uses internally some tricks specific to GNU/Linux platform and a test building requires a few additional steps.

Firstly, all functions you want to mock must be in dynamic libraries. If it includes your project-specific functions you must put them to dynamic library. In such a circumstances it seems reasonable to build all code under test as a dynamic library. Selecting only those parts that you are going to mock might be tedious and cumbersome.

Secondly, you must pass following options to linker when building test executable:

* *-rdynamic* - add all symbols to dynamic symbol table
* *-Wl,--no-as-needed* - link with library event though during static linking there are no dependencies to it
* *-ldl* - dynamic linking loader library

C Mock comes with *cmock-config* tool to hide all these details away from you. Run

        cmock-config --cflags

and

        cmock-config --libs

to get compilations and linker options, respectively.

Let's say you built a code under test into *libfoo.so* and put a test code in *bar.cc*. To build your test executable you would run:

        g++ `cmock-config --cflags` -c bar.cc -o bar.o
        g++ `cmock-config --libs` -pthread -lfoo -lgmock -lgtest bar.o -o bar # Google Test requires -pthread

When building code under test as a dynamic library it is handy to specify *soname* as an absolute path name. Then when test executable is run no additional environment setup is required for dynamic linking loader to locate your library (i.e. setting LD\_LIBRARY\_PATH).

Installation
------------

To install run:

        make install

To uninstall run:

        make uninstall

By default instalation *PREFIX* is */usr/local*. Change it in command line if required.

Test
----

If your platform is supported, the following commands should succeed:

        make
        make test

Tests are quite simple and are good source of example.

References
----------
* [Google Mock][1]
* [My code calls a static/global function. Can I mock it?][2]
* [Defeat "Static Cling"][3]

[1]: http://code.google.com/p/googlemock/ "Google Mock"
[2]: https://code.google.com/p/googlemock/wiki/FrequentlyAskedQuestions#My_code_calls_a_static/global_function.__Can_I_mock_it? "My code calls a static/global function. Can I mock it?"
[3]: http://googletesting.blogspot.com/2008/06/defeat-static-cling.html "Defeat "Static Cling""
