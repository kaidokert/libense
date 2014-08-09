//===------------------------- string.cpp ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#define _LIBCPP_EXTERN_TEMPLATE(...) extern template __VA_ARGS__;

#include "string"
#include "cerrno"
#include "limits"
#include "stdexcept"
#ifdef _LIBCPP_MSVCRT
#include "support/win32/support.h"
#endif // _LIBCPP_MSVCRT
#include <stdio.h>

_LIBCPP_BEGIN_NAMESPACE_STD

template class __basic_string_common<true>;

template class basic_string<char>;

template
    string
    operator+<char, char_traits<char>, allocator<char> >(char const*, string const&);

_LIBCPP_END_NAMESPACE_STD
