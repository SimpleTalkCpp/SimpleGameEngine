#include "Vec4_Basic.h"

#include <xmmintrin.h>

namespace sge {

template<class T, class M>
struct Vec4_SSE_Data {
	using RegisterType = M;
	using ElementType = T;
	static const size_t kElementCount = 4;

	union {
		struct { T x, y, z, w; };
		T data[kElementCount];
		M _m;
	};

	SGE_INLINE Vec4_SSE_Data() = default;
	SGE_INLINE Vec4_SSE_Data(const T& x_, const T& y_, const T& z_, const T& w_);
	SGE_INLINE Vec4_SSE_Data(const Tuple4<T>& v)
		: Vec4_SSE_Data(v.x, v.y, v.z, v.w)
	{
		static_assert(sizeof(Vec4_SSE_Data) == sizeof(T) * 4);
	}
};

using Vec4f_SSE_Data = Vec4_SSE_Data<float,  __m128 >; // SSE
using Vec4d_SSE_Data = Vec4_SSE_Data<double, __m256d>; // AVX

template<class T> struct Vec4_SSE_Select  { using Data = Tuple4<T>; };
template<> struct Vec4_SSE_Select<float > { using Data = Vec4f_SSE_Data; };
template<> struct Vec4_SSE_Select<double> { using Data = Vec4d_SSE_Data; };

template<class T> using Vec4_SSE = Vec4_Basic<T, typename Vec4_SSE_Select<T>::Data >;

using Vec4f_SSE = Vec4_SSE<float>;
using Vec4d_SSE = Vec4_SSE<double>;

SGE_INLINE Vec4f_SSE Vec4_SSE_make(__m128  m) { Vec4f_SSE o; o._m = m; return o; }
SGE_INLINE Vec4d_SSE Vec4_SSE_make(__m256d m) { Vec4d_SSE o; o._m = m; return o; }

#if defined(SGE_CPU_ENDIAN_LITTLE)
	template<> SGE_INLINE Vec4f_SSE_Data::Vec4_SSE_Data(const float&  x_, const float&  y_, const float&  z_, const float&  w_) : _m(   _mm_set_ps(w_, z_, y_, x_)) {}
	template<> SGE_INLINE Vec4d_SSE_Data::Vec4_SSE_Data(const double& x_, const double& y_, const double& z_, const double& w_) : _m(_mm256_set_pd(w_, z_, y_, x_)) {}
#elif defined(SGE_CPU_ENDIAN_BIG)
	template<> SGE_INLINE Vec4f_SSE_Data::Vec4_SSE_Data(const float&  x_, const float&  y_, const float&  z_, const float&  w_) : _m(   _mm_set_ps(x_, y_, z_, w_)) {}
	template<> SGE_INLINE Vec4d_SSE_Data::Vec4_SSE_Data(const double& x_, const double& y_, const double& z_, const double& w_) : _m(_mm256_set_pd(x_, y_, z_, w_)) {}
#else
	#error
#endif

template<> SGE_INLINE Vec4f_SSE Vec4f_SSE::operator+(const Vec4& r) const { return Vec4_SSE_make(   _mm_add_ps(_m, r._m)); }
template<> SGE_INLINE Vec4f_SSE Vec4f_SSE::operator-(const Vec4& r) const { return Vec4_SSE_make(   _mm_sub_ps(_m, r._m)); }
template<> SGE_INLINE Vec4f_SSE Vec4f_SSE::operator*(const Vec4& r) const { return Vec4_SSE_make(   _mm_mul_ps(_m, r._m)); }
template<> SGE_INLINE Vec4f_SSE Vec4f_SSE::operator/(const Vec4& r) const { return Vec4_SSE_make(   _mm_div_ps(_m, r._m)); }

template<> SGE_INLINE Vec4d_SSE Vec4d_SSE::operator+(const Vec4& r) const { return Vec4_SSE_make(_mm256_add_pd(_m, r._m)); }
template<> SGE_INLINE Vec4d_SSE Vec4d_SSE::operator-(const Vec4& r) const { return Vec4_SSE_make(_mm256_sub_pd(_m, r._m)); }
template<> SGE_INLINE Vec4d_SSE Vec4d_SSE::operator*(const Vec4& r) const { return Vec4_SSE_make(_mm256_mul_pd(_m, r._m)); }
template<> SGE_INLINE Vec4d_SSE Vec4d_SSE::operator/(const Vec4& r) const { return Vec4_SSE_make(_mm256_div_pd(_m, r._m)); }

}