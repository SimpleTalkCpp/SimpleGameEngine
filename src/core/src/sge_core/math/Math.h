#pragma once

namespace sge {

namespace Math {
	template<class T> constexpr T max(const T& a, const T& b) { return a > b ? a : b; }
	template<class T> constexpr T min(const T& a, const T& b) { return a < b ? a : b; }

	template<class T> constexpr T byteToK(const T& v) { return v / 1024; }
	template<class T> constexpr T byteToM(const T& v) { return v / (1024 * 1024); }
	template<class T> constexpr T byteToG(const T& v) { return v / (1024 * 1024 * 1024); }

	struct _Helper {
		template<class T>
		static constexpr T alignTo_uint(T n, T a) {
			static_assert(std::is_unsigned<T>::value, "");
			T r = n % a;
			return r ? (n + a - r) : n;
		//	return (n + a - 1) / a * a;
		}
	};

	constexpr size_t alignTo(size_t n, size_t a) { return _Helper::alignTo_uint(n, a); }

#if SGE_OS_MACOSX
	SGE_INLINE void sincos( float  rad, float  & out_sin, float  & out_cos ) { ::__sincosf(rad, &out_sin, &out_cos); }
	SGE_INLINE void sincos( double rad, double & out_sin, double & out_cos ) { ::__sincos (rad, &out_sin, &out_cos); }
#elif SGE_OS_LINUX
	SGE_INLINE void sincos( float  rad, float  & out_sin, float  & out_cos ) { ::sincosf(rad, &out_sin, &out_cos); }
	SGE_INLINE void sincos( double rad, double & out_sin, double & out_cos ) { ::sincos (rad, &out_sin, &out_cos); }
#else
	SGE_INLINE void sincos( float  rad, float  & out_sin, float  & out_cos ) { out_sin = ::sinf(rad); out_cos = ::cosf(rad); }
	SGE_INLINE void sincos( double rad, double & out_sin, double & out_cos ) { out_sin = ::sin (rad); out_cos = ::cos (rad); }
#endif

	template<class T> T abs(const T& v) { return v < 0 ? -v : v; }

	template<class T> constexpr T epsilon();
	template<>	constexpr int	epsilon<int >() { return 0; }
	template<>	constexpr f32	epsilon<f32 >() { return FLT_EPSILON; }
	template<>	constexpr f64	epsilon<f64 >() { return DBL_EPSILON; }
	template<>	constexpr f128	epsilon<f128>() { return LDBL_EPSILON; }

	template<class T, class EP = T> SGE_INLINE bool equals (const T& a, const T& b, const EP& ep = epsilon<T>()) { return abs(a-b) <= ep; }
	template<class T, class EP = T> SGE_INLINE bool equals0(const T& a,             const EP& ep = epsilon<T>()) { return abs( a ) <= ep; }

}

}