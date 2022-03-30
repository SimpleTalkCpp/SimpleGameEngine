#pragma once

#pragma warning(disable: 4668) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'

#if !SGE_COMPILER_VC	
	#error
#endif

#if _MSVC_LANG >= 199711L
	#define SGE_CPLUSPLUS_03	1
#endif

#if _MSVC_LANG >= 201103L
	#define SGE_CPLUSPLUS_11	1
#endif

#if _MSVC_LANG >= 201402L
	#define SGE_CPLUSPLUS_14	1
#endif

#if _MSVC_LANG >= 201703L
	#define SGE_CPLUSPLUS_17	1
#endif

#if _MSVC_LANG >= 202002L
	#define SGE_CPLUSPLUS_20	1
#endif

#define SGE_TYPE_HAS_SIZE_T		0
#define SGE_TYPE_HAS_SSIZE_T	0
#define SGE_TYPE_HAS_LONGLONG	0

#define SGE_DLL_EXPORT			_declspec(dllexport)
#define SGE_DLL_IMPORT			_declspec(dllimport)

#define	SGE_FUNC_NAME_SZ			__FUNCTION__
#define SGE_FUNC_FULLNAME_SZ		__FUNCSIG__

#define SGE_DEPRECATED			__declspec(deprecated)

#define SGE_COMPILER_VER _MSVC_LANG

#if SGE_CPLUSPLUS_17
	#define SGE_FALLTHROUGH	//	[[fallthrough]]
	#define SGE_NODISCARD		[[nodiscard]]
#else
	#define SGE_FALLTHROUGH
	#define SGE_NODISCARD
#endif

//#define SGE_ALIGN(N)				__declspec(align(N)) 
//#define SGE_ALIGN(N)				alignas(N) //c++11

#define SGE_OPTIMIZE_OFF			__pragma(optimize("", off))

#if 0 // SGE_GEN_CONFIG_Debug
	#define	SGE_INLINE		inline
#else
	#define	SGE_INLINE		__forceinline
#endif

#define SGE_THREAD_LOCAL			__declspec( thread )
//#define SGE_THREAD_LOCAL thread_local //C++11

#if _MSC_VER < 1600
	#define	nullptr	NULL
#endif

//cpu
#if _M_X64
	#define SGE_CPU_X86_64	1
	#define SGE_CPU_FEATURE_SSE1		1
	#define SGE_CPU_FEATURE_SSE2		1
	#define SGE_CPU_FEATURE_SSE3		1

#elif _M_PPC
	#define SGE_CPU_PowerPC     1
#else
	#define SGE_CPU_x86         1
#endif

//os

#if _WIN64
	#define SGE_OS_WIN64     1
	#define SGE_OS_WINDOWS	1
#elif _WIN32
	#define SGE_OS_WIN32     1
	#define SGE_OS_WINDOWS	1
#elif _WINCE
	#define SGE_OS_WinCE     1
	#define SGE_OS_WINDOWS	1
#endif
