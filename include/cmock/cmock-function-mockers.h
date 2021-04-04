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
// This file implements DECLARE_FUNCTION_MOCKn() and IMPLEMENT_FUNCTION_MOCKn()
// macros of various arities.

#ifndef CMOCK_INCLUDE_CMOCK_CMOCK_FUNCTION_MOCKERS_H_
#define CMOCK_INCLUDE_CMOCK_CMOCK_FUNCTION_MOCKERS_H_

#include <dlfcn.h>

#include <sstream>
#include <stdexcept>

#include "cmock/cmock-internal.h"

#define CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(_ClassName, _FunctionName, _N, _Signature) \
    class _ClassName \
    { \
        typedef CMOCK_INTERNAL_RETURN_TYPE(_Signature) (*func_type)(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)); \
\
        public: \
            static func_type real; \
\
            _ClassName(); \
            ~_ClassName(); \
\
            CMOCK_INTERNAL_RETURN_TYPE(_Signature) operator()(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)); \
            ::testing::MockSpec<GMOCK_PP_REMOVE_PARENS(_Signature)> cmock_func(GMOCK_PP_REPEAT(GMOCK_INTERNAL_MATCHER_PARAMETER, _Signature, _N)); \
\
        private: \
            static func_type lookup(); \
            static CMOCK_INTERNAL_RETURN_TYPE(_Signature) call(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)); \
\
            static _ClassName *instance; \
\
            ::testing::FunctionMocker<GMOCK_PP_REMOVE_PARENS(_Signature)> mocker; \
\
            friend CMOCK_INTERNAL_RETURN_TYPE(_Signature) _FunctionName(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)); \
    };

#define CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(_ClassName, _FunctionName, _N, _Signature) \
    _ClassName::_ClassName() { \
        instance = this; \
    } \
\
    _ClassName::~_ClassName() { \
        instance = NULL; \
    } \
\
    CMOCK_INTERNAL_RETURN_TYPE(_Signature) _ClassName::operator()(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)) { \
        mocker.SetOwnerAndName(this, #_FunctionName); \
        return mocker.Invoke(GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, _Signature, _N)); \
    } \
\
    ::testing::MockSpec<GMOCK_PP_REMOVE_PARENS(_Signature)> _ClassName::cmock_func(GMOCK_PP_REPEAT(GMOCK_INTERNAL_MATCHER_PARAMETER, _Signature, _N)) { \
        mocker.RegisterOwner(this); \
        return mocker.With(GMOCK_PP_REPEAT(GMOCK_INTERNAL_MATCHER_ARGUMENT, , _N)); \
    } \
\
    _ClassName::func_type _ClassName::lookup() { \
        return (_ClassName::func_type)dlsym(RTLD_NEXT, #_FunctionName); \
    } \
\
    CMOCK_INTERNAL_RETURN_TYPE(_Signature) _ClassName::call(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)) { \
        if (instance != NULL) { \
            return (*instance)(GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, _Signature, _N)); \
        } \
\
        if (real == NULL) { \
            std::ostringstream msg; \
            msg << "Error: Function " << #_FunctionName; \
            msg << " not found. Neither mock nor real function is present."; \
            throw std::logic_error(msg.str()); \
        } \
        return real(GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, _Signature, _N)); \
    } \
\
    _ClassName *_ClassName::instance = NULL; \
    _ClassName::func_type _ClassName::real = lookup(); \
\
    CMOCK_INTERNAL_RETURN_TYPE(_Signature) _FunctionName(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, _Signature, _N)) { \
        return _ClassName::call(GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, _Signature, _N)); \
    }

#define DECLARE_FUNCTION_MOCK0(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 0, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK0(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 0, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK1(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 1, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK1(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 1, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK2(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 2, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK2(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 2, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK3(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 3, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK3(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 3, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK4(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 4, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK4(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 4, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK5(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 5, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK5(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 5, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK6(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 6, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK6(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 6, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK7(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 7, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK7(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 7, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK8(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 8, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK8(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 8, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK9(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 9, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK9(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 9, (::testing::internal::identity_t<__VA_ARGS__>))

#define DECLARE_FUNCTION_MOCK10(c, n, ...) \
    CMOCK_INTERNAL_DECLARE_FUNCTION_MOCKN(c, n, 10, (::testing::internal::identity_t<__VA_ARGS__>))

#define IMPLEMENT_FUNCTION_MOCK10(c, n, ...) \
    CMOCK_INTERNAL_IMPLEMENT_FUNCTION_MOCKN(c, n, 10, (::testing::internal::identity_t<__VA_ARGS__>))

#endif // CMOCK_INCLUDE_CMOCK_CMOCK_FUNCTION_MOCKERS_H_
