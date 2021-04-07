// Copyright 2021, Hubert Jagodziński
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: hubert.jagodzinski@gmail.com (Hubert Jagodziński)

// C Mock - Google Mock's extension allowing a function mocking.
//
// This file implements CMockMocker class, the generic macros CMOCK_MOCK_METHOD & CMOCK_MOCK_FUNCTION
// and old-style CMOCK_MOCK_FUNCTIONn() macros of various arities.

#ifndef CMOCK_INCLUDE_CMOCK_CMOCK_FUNCTION_CLASS_MOCKERS_H_
#define CMOCK_INCLUDE_CMOCK_CMOCK_FUNCTION_CLASS_MOCKERS_H_

#include <dlfcn.h>

#include <sstream>
#include <stdexcept>

#include "cmock/cmock-internal.h"

// Allows finding an instance of a class:
//   class Foo : public CMockMocker<Foo> { ... }
//   Foo *p = CMockMocker<Foo>::cmock_get_instance();
template<typename T>
class CMockMocker
{
public:
	CMockMocker()
	{
		instance = (T *)this;
	}

	~CMockMocker()
	{
		instance = (T *)NULL;
	}

	static T *cmock_get_instance() { return instance; }

private:
	static T *instance;
};

template<typename T>
T *CMockMocker<T>::instance = NULL;

// Find the real implementation of a mocked function
static inline void *
cmock_lookup(const char *fname)
{
    return dlsym(RTLD_NEXT, fname);
}

#define CMOCK_MOCK_METHOD(_Ret, _FunctionName, _Args) \
    MOCK_METHOD(_Ret, _FunctionName, _Args); \
\
    typedef _Ret (*_FunctionName##_type)(GMOCK_PP_REPEAT(CMOCK_INTERNAL_NO_PARAMETER_NAME, (GMOCK_INTERNAL_SIGNATURE(_Ret, _Args)), GMOCK_PP_NARG0 _Args)); \
    static _FunctionName##_type cmock_real_##_FunctionName;

#define CMOCK_INTERNAL_IMPLEMENT_FUNCTION(_ClassName, _FunctionName, _RealFunctionPtr, _N, _Signature) \
    CMOCK_INTERNAL_RETURN_TYPE(_Signature) _FunctionName(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)) { \
        _ClassName *mock = _ClassName::cmock_get_instance(); \
        if (mock != nullptr) { \
            return mock->_FunctionName(GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, _Signature, _N)); \
        } \
\
        if (_RealFunctionPtr == nullptr) { \
            std::ostringstream msg; \
            msg << "Error: Function " << #_FunctionName; \
            msg << " not found. Neither mock nor real function is present."; \
            throw std::logic_error(msg.str()); \
        } \
        return _RealFunctionPtr(GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, _Signature, _N)); \
    }

#define CMOCK_REAL_FUNCTION(_ClassName, _FunctionName) \
    _ClassName::cmock_real_##_FunctionName

#define CMOCK_MOCK_FUNCTION(_ClassName,  _Ret, _FunctionName, _Args) \
    _ClassName::_FunctionName##_type CMOCK_REAL_FUNCTION(_ClassName, _FunctionName) = (_ClassName::_FunctionName##_type)cmock_lookup(#_FunctionName); \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION(_ClassName, _FunctionName, CMOCK_REAL_FUNCTION(_ClassName, _FunctionName), GMOCK_PP_NARG0 _Args, (GMOCK_INTERNAL_SIGNATURE(_Ret, _Args)))

#define CMOCK_INTERNAL_MOCK_FUNCTIONN(_ClassName, _FunctionName, _N, _Signature) \
    typedef CMOCK_INTERNAL_RETURN_TYPE(_Signature) (*_ClassName##_##_FunctionName##_type)(GMOCK_PP_REPEAT(CMOCK_INTERNAL_NO_PARAMETER_NAME, _Signature, _N)); \
    static _ClassName##_##_FunctionName##_type cmock_real_##_FunctionName = (_ClassName##_##_FunctionName##_type)cmock_lookup(#_FunctionName); \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION(_ClassName, _FunctionName, cmock_real_##_FunctionName, _N, _Signature)

#define CMOCK_MOCK_FUNCTION0(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 0, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION1(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 1, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION2(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 2, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION3(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 3, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION4(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 4, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION5(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 5, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION6(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 6, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION7(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 7, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION8(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 8, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION9(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 9, (::testing::internal::identity_t<__VA_ARGS__>))

#define CMOCK_MOCK_FUNCTION10(c, n, ...) \
    CMOCK_INTERNAL_MOCK_FUNCTIONN(c, n, 10, (::testing::internal::identity_t<__VA_ARGS__>))

#endif // CMOCK_INCLUDE_CMOCK_CMOCK_FUNCTION_CLASS_MOCKERS_H_
