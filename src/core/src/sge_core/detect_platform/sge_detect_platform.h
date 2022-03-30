#pragma once

/*
SGE_DetectPlatform:
	- headers only
	- detect compiler / OS / CPU
	- may be used by external lib, so please keep it simple
*/

#ifdef __OBJC__ // Objective C
	#define SGE_OBJC		1
#else
	#define SGE_OBJC		0
#endif

//=========== Detect COMPILER ===============
#if defined(__clang__) 
	#define SGE_COMPILER_CLANG	1
	#include "sge_detect_compiler_gcc.h"

#elif defined(__GNUC__)
	#define SGE_COMPILER_GCC		1
	#include "sge_detect_compiler_gcc.h"

#elif defined(_MSC_VER)
	#define SGE_COMPILER_VC		1
	#include "sge_detect_compiler_vc.h"

#endif

#if SGE_COMPILER_VC + SGE_COMPILER_GCC + SGE_COMPILER_CLANG != 1
    #error "Compiler should be specified"
#endif

//======== Detect CPU =============

// check CPU define
#if SGE_CPU_X86_64 + SGE_CPU_X86 + SGE_CPU_POWERPC + SGE_CPU_ARM + SGE_CPU_ARM64 != 1
	#error "CPU should be specified"
#endif

#if SGE_CPU_X86_64
	#define SGE_CPU_LP64				1
	#define SGE_CPU_ENDIAN_LITTLE	1
	#define SGE_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SGE_CPU_X86
	#define SGE_CPU_LP32				1
	#define SGE_CPU_ENDIAN_LITTLE	1
	#define SGE_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SGE_CPU_POWERPC
	#define SGE_CPU_LP32				1
	#define SGE_CPU_ENDIAN_BIG		1
	#define SGE_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SGE_CPU_ARM64
	#define SGE_CPU_LP64				1
	#define SGE_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SGE_CPU_ARM
	#define SGE_CPU_LP32				1
	#define SGE_CPU_MISALIGNED_MEMORY_ACCESSES	1
#endif

#if SGE_CPU_LP32 + SGE_CPU_LP64 != 1
	#error "CPU bits should be specified"
#endif

#if SGE_CPU_LP32
	#define SGE_SIZEOF_POINTER		4
#endif

#if SGE_CPU_LP64
	#define SGE_SIZEOF_POINTER		8
#endif

#ifndef SGE_SIZEOF_POINTER
	#error "Unknown SGE_SIZEOF_POINTER"
#endif

#if SGE_CPU_ENDIAN_BIG + SGE_CPU_ENDIAN_LITTLE != 1
	#error "CPU endian should be specified"
#endif

//======== Detect OS ===============

#if SGE_OS_WIN32 + SGE_OS_WIN64 + SGE_OS_WINCE \
	+ SGE_OS_FREEBSD + SGE_OS_LINUX \
	+ SGE_OS_ANDROID \
	+ SGE_OS_MACOSX  + SGE_OS_IOS \
	+ SGE_OS_CYGWIN  + SGE_OS_MINGW != 1
	#error "OS should be specified"
#endif



//====================================

#ifndef UNICODE
	#define UNICODE
#endif

#ifndef _UNICODE
	#define _UNICODE
#endif
