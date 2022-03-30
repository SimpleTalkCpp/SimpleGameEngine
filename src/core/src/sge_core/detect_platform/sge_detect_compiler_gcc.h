#pragma once

#if ! (SGE_COMPILER_CLANG | SGE_COMPILER_GCC)
	#error
#endif

#if __cplusplus >= 199711L
	#define SGE_CPLUSPLUS_03	1
#endif

#if __cplusplus >= 201103L 
	#define SGE_CPLUSPLUS_11	1
#endif

#if __cplusplus >= 201402L
	#define SGE_CPLUSPLUS_14	1
#endif

#if __cplusplus >= 201703L
	#define SGE_CPLUSPLUS_17	1
#endif

#if __cplusplus >= 202002L
	#define SGE_CPLUSPLUS_20	1
#endif



#if SGE_COMPILER_CLANG
	#define SGE_TYPE_HAS_SIZE_T		0  //linux 
	#define SGE_TYPE_HAS_SSIZE_T		0  //linux 

	#define SGE_OPTIMIZE_OFF			_Pragma("clang optimize off")
#else
	#define SGE_TYPE_HAS_SIZE_T		0  //linux 
	#define SGE_TYPE_HAS_SSIZE_T		0  //linux 

	#define SGE_OPTIMIZE_OFF			_Pragma("GCC optimize(\"O0\")")
#endif

#if __GNUC__ >= 4
	#define	SGE_DLL_EXPORT			__attribute__ ((visibility ("default")))
	#define SGE_DLL_IMPORT			// __attribute__ ((visibility ("hidden")))
#else
	#define	SGE_DLL_EXPORT
	#define SGE_DLL_IMPORT
#endif


// #define nullptr	NULL

#define	SGE_FUNC_NAME_SZ				__FUNCTION__
#define SGE_FUNC_FULLNAME_SZ			__PRETTY_FUNCTION__

#define SGE_DEPRECATED( f )		f __attribute__( (deprecated) )

#define SGE_COMPILER_VER __cplusplus

#if SGE_CPLUSPLUS_17
	#define SGE_FALLTHROUGH		// [[fallthrough]]
	#define SGE_NODISCARD		[[nodiscard]]
#else
	#define SGE_FALLTHROUGH
	#define SGE_NODISCARD
#endif

//#define SGE_ALIGN(N)				__attribute__((aligned(N)))
//#define SGE_ALIGN(N)				alignas(N) //c++11

#if SGE_GEN_CONFIG_Debug
	#define	SGE_INLINE      inline
#else
	#define	SGE_INLINE		inline //__attribute__( (always_inline) )
#endif

//#define SGE_THREAD_LOCAL	__thread
#define SGE_THREAD_LOCAL thread_local //C++11


#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64) || defined(__amd64__)
	#define SGE_CPU_X86_64      1

#elif defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
	#define SGE_CPU_X86         1

#elif defined(__POWERPC__)
	#define SGE_CPU_POWERPC     1

#elif defined(__aarch64__)
	#define SGE_CPU_ARM64 1

	#if defined(__ARMEL__) || defined(__AARCH64EL__)
		#define SGE_CPU_ENDIAN_LITTLE	1
	#elif defined(__ARMEB__) || defined(__AARCH64EB__)
		#define SGE_CPU_ENDIAN_BIG		1
	#else
		#error unknown ARM CPU endian
	#endif

#elif defined(__arm__)
	#define SGE_CPU_ARM 1

	#if defined(__ARMEL__) || defined(__AARCH64EL__)
		#define SGE_CPU_ENDIAN_LITTLE	1
	#elif defined(__ARMEB__) || defined(__AARCH64EB__)
		#define SGE_CPU_ENDIAN_BIG		1
	#else
		#error unknown ARM CPU endian
	#endif
#endif

#if	defined(__ARM_NEON__) || defined(__ARM_NEON)
	#define SGE_CPU_FEATURE_ARM_NEON
#endif

#if __SSE__
	#define SGE_CPU_FEATURE_SSE1			1
#endif

#if __SSE2__
	#define SGE_CPU_FEATURE_SSE2			1
	#include <xmmintrin.h>
#endif

#if __SSE3__
	#define SGE_CPU_FEATURE_SSE3			1
#endif

#if __SSSE3__
	#define SGE_CPU_FEATURE_SSSE3		1
#endif

#if __SSE4_1__
	#define SGE_CPU_FEATURE_SSE4_1		1
#endif

#if __SSE4_2__
	#define SGE_CPU_FEATURE_SSE4_2		1
#endif

//os

#if _WIN64
	#define SGE_OS_WIN64     1
	#define SGE_OS_WINDOWS	1

#elif _WIN32
	#define SGE_OS_WIN32     1
	#define SGE_OS_WINDOWS	1

#elif __ANDROID__
	#define SGE_OS_ANDROID	1

#elif __linux
	#define SGE_OS_LINUX        1

#elif __FreeBSD__
	#define SGE_OS_FREEBSD		1

#elif __APPLE__ && __MACH__
	#include <TargetConditionals.h>
	#if (TARGET_OF_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define SGE_OS_IOS		1
	#else
		#define SGE_OS_MACOSX	1
	#endif

#elif __sun
	#define SGE_OS_SOLARIS		1

#elif __CYGWIN__
    #define SGE_OS_CYGWIN        1

#elif __MINGW32__
	#define SGE_OS_MINGW			1
#endif

