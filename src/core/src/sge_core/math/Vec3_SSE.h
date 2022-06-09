#include "Vec3_Basic.h"

#include <xmmintrin.h>

namespace sge {

template<class T, class M>
struct Vec3_SSE_Data {
	using RegisterType = M;
	using ElementType = T;
	static const size_t kElementCount = 3;

	union {
		struct { T x, y, z, _unused_padding; };
		T data[kElementCount];
		M _m;
	};

	SGE_INLINE Vec3_SSE_Data() = default;
	SGE_INLINE Vec3_SSE_Data(const T& x_, const T& y_, const T& z_);
	SGE_INLINE Vec3_SSE_Data(const Tuple3<T>& v)
		: Vec3_SSE_Data(v.x, v.y, v.z)
	{
		static_assert(sizeof(Vec3_SSE_Data) == sizeof(T) * 4);
	}
};

using Vec3f_SSE_Data = Vec3_SSE_Data<float,  __m128 >; // SSE
using Vec3d_SSE_Data = Vec3_SSE_Data<double, __m256d>; // AVX

template<class T> struct Vec3_SSE_Select  { using Data = Tuple3<T>; };
template<> struct Vec3_SSE_Select<float > { using Data = Vec3f_SSE_Data; };
template<> struct Vec3_SSE_Select<double> { using Data = Vec3d_SSE_Data; };

template<class T> using Vec3_SSE = Vec3_Basic<T, typename Vec3_SSE_Select<T>::Data >;

using Vec3f_SSE = Vec3_SSE<float>;
using Vec3d_SSE = Vec3_SSE<double>;

SGE_INLINE Vec3f_SSE Vec3_SSE_make(__m128  m) { Vec3f_SSE o; o._m = m; return o; }
SGE_INLINE Vec3d_SSE Vec3_SSE_make(__m256d m) { Vec3d_SSE o; o._m = m; return o; }

#if defined(SGE_CPU_ENDIAN_LITTLE)
	template<> SGE_INLINE Vec3f_SSE_Data::Vec3_SSE_Data(const float&  x_, const float&  y_, const float&  z_) : _m(   _mm_set_ps(0, z_, y_, x_)) {}
	template<> SGE_INLINE Vec3d_SSE_Data::Vec3_SSE_Data(const double& x_, const double& y_, const double& z_) : _m(_mm256_set_pd(0, z_, y_, x_)) {}
#elif defined(SGE_CPU_ENDIAN_BIG)
	template<> SGE_INLINE Vec3f_SSE_Data::Vec3_SSE_Data(const float&  x_, const float&  y_, const float&  z_) : _m(   _mm_set_ps(x_, y_, z_, 0)) {}
	template<> SGE_INLINE Vec3d_SSE_Data::Vec3_SSE_Data(const double& x_, const double& y_, const double& z_) : _m(_mm256_set_pd(x_, y_, z_, 0)) {}
#else
	#error
#endif

template<> SGE_INLINE Vec3f_SSE Vec3f_SSE::operator+(const Vec3& r) const { return Vec3_SSE_make(   _mm_add_ps(_m, r._m)); }
template<> SGE_INLINE Vec3f_SSE Vec3f_SSE::operator-(const Vec3& r) const { return Vec3_SSE_make(   _mm_sub_ps(_m, r._m)); }
template<> SGE_INLINE Vec3f_SSE Vec3f_SSE::operator*(const Vec3& r) const { return Vec3_SSE_make(   _mm_mul_ps(_m, r._m)); }
template<> SGE_INLINE Vec3f_SSE Vec3f_SSE::operator/(const Vec3& r) const { return Vec3_SSE_make(   _mm_div_ps(_m, r._m)); }

template<> SGE_INLINE Vec3d_SSE Vec3d_SSE::operator+(const Vec3& r) const { return Vec3_SSE_make(_mm256_add_pd(_m, r._m)); }
template<> SGE_INLINE Vec3d_SSE Vec3d_SSE::operator-(const Vec3& r) const { return Vec3_SSE_make(_mm256_sub_pd(_m, r._m)); }
template<> SGE_INLINE Vec3d_SSE Vec3d_SSE::operator*(const Vec3& r) const { return Vec3_SSE_make(_mm256_mul_pd(_m, r._m)); }
template<> SGE_INLINE Vec3d_SSE Vec3d_SSE::operator/(const Vec3& r) const { return Vec3_SSE_make(_mm256_div_pd(_m, r._m)); }

}