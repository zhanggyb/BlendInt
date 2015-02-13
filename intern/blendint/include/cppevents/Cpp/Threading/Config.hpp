// Copyright (c) 2010 Nickolas Pohilets
//
// This file is a part of the CppEvents library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __CPP__THREADING__CONFIG__HPP
#define __CPP__THREADING__CONFIG__HPP

#ifdef __UNIX__
#define CPP_THREADING_PLATFORM_DIR POSIX
#else
#ifdef __WIN__
// Directory for platform-specific files (set by CMake)
#define CPP_THREADING_PLATFORM_DIR WinApi
#endif
#endif

#ifndef CPP_THREADING_PLATFORM_DIR
#error "Unsupported target platform!"
#endif

// Construction of the platform-specific paths
#define CPP_THREADING_PLATFORM_PATH_HELPER_0(Path) #Path
#define CPP_THREADING_PLATFORM_PATH_HELPER_1(Dir, Path) CPP_THREADING_PLATFORM_PATH_HELPER_0(Dir/Path)
#define CPP_THREADING_PLATFORM_PATH_HELPER_2(Dir, Path) CPP_THREADING_PLATFORM_PATH_HELPER_1(Dir, Path)
#define CPP_THREADING_PLATFORM_PATH(Path) CPP_THREADING_PLATFORM_PATH_HELPER_2(CPP_THREADING_PLATFORM_DIR, Path)

#endif //__CPP__THREADING__CONFIG__HPP
