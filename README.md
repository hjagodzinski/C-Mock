C Mock - Google Mock Extension
==============================

Overview
--------
C Mock is [Google Mock] [1] extension allowing for mocking C functions.

This is not either a patch to nor fork of Google Mock. This is just a set of headers providing a way of using tools for mock methods with mock C functions in tests.

Requirements
------------
* Google Mock (1.6 or newer)
* GNU/Linux platform that Google Mock supports

Guide
-----
C Mock requires no prior build. As already mentioned it is just a set of header files. What it really gives you are three macros:

* MOCK\_FUNCTION\_\*
* EXPECT\_FUNCTION\_CALL
* ON\_FUNCTION\_CALL

These macros do what theirs' method counterparts do MOCK\_METHOD\_\*, EXPECT\_CALL and ON\_CALL, respectively. There are small differences though.

MOCK\_FUNCTION\_\* in fact stands for series of macros for defining C function mocks i.e. MOCK\_FUNCTION2 - does it resemble you something? It takes three arguments mock class name, function name and function prototype. For instance following line of code defines mock class FooFunctionMock for function int foo(int, int):

    FUNCTION_MOCK2(FooFunctionMock, foo, int(int, int));

EXPECT\_FUNCTION\_CALL and ON\_FUNCTION\_CALL do exactly what theirs' method equivalents. Both take two arguments mock class instance and arguments you expect - there is no need to repeat function name since we already know it at this point. For instance, suppose we expect foo function to be called once with arguments 1 and 2, and want it to return 3:

    FooFunctionMock mock;
    EXPECT_FUNCTION_CALL(mock, (1, 2)).WillOnce(::testing::Return(3));

Function is mocked as long as its corresponding mock class instance exists. This means function is mocked only when required. Still there might be cases you want call real function.

    {
	    {
		    FooFunctionMock mock;
		    /* ... */
		    foo(1, 2); // call mock
	    }

	    foo(1, 2); // call real function
    }

C Mock uses internally some tricks specific to GNU/Linux platforms and a test building requires a few additional steps.

Firstly, all functions you want to mock must be in dynamic libraries. If it includes your project-specific functions you must put them to dynamic library. In such a circumstances it seems reasonable to build all code under test as dynamic library. Selecting only those parts that you are going to mock might be tedious and cumbersome.

Secondly, you must pass following flags to linker when building test executable:

* *-rdynamic* - add all symbols to dynamic symbol table
* *-Wl,--no-as-needed* - link with library event though during static linking there are no dependencies to it
* *-ldl* - dynamic linking loader library

When building code under test as dynamic library it is handy to specify soname as absolute path name. Then when test executable is run no addition environment setup is required for dynamic linking loader to locate your library (i.e. setting LD\_LIBRARY\_PATH).

Example
-------
See example source directory for example.

If your platform is supported, the following commands should succeed:

    cd example
    make
    ./example

[1]: http://code.google.com/p/googlemock/ "Google Mock"
