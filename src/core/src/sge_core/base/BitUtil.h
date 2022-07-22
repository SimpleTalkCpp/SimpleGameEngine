#pragma once

namespace sge {

struct BitUtil {
	BitUtil() = delete;
public:
	template<class T>	constexpr	static bool	hasAny	(T  value, T bits)			{ return (value & bits) != T(0);    }
	template<class T>	constexpr	static bool	has		(T  value, T bits)			{ return (value & bits) == bits; }	
	template<class T>	constexpr	static void	set		(T& value, T bits, bool b)	{ b ? set(value, bits) : unset(value, bits); }
	template<class T>	constexpr	static void	set		(T& value, T bits)			{ value |=  bits; }
	template<class T>	constexpr	static void	unset	(T& value, T bits)			{ value &= ~bits; }
	template<class T>	constexpr	static void	toggle	(T& value, T bits)			{ value ^=  bits; }
	template<class T>	constexpr	static int	count1	(T  value);
	template<class T>	constexpr	static int	count0	(T  value) { return count1(~value); }
	template<class T>	constexpr	static int	highest	(T  value);
	template<class T>	constexpr	static int	lowest	(T  value);
};

template<class T> constexpr
int BitUtil::highest(T value) {
	int result = -1;
	int n = sizeof(T) * 8;
	T m = T(1 << (n - 1));
	for (int i = n - 1; i >= 0; i--) {
		if (hasAny(value, m))
			return i;
		m >>= 1;
	}
	return result;
}

template<class T> constexpr
int BitUtil::lowest(T value) {
	int result = -1;
	int n = sizeof(T) * 8;
	T m = T(1);
	for (int i = 0; i < n; i++) {
		if (hasAny(value, m))
			return i;
		m <<= 1;
	}
	return result;
}

template<class T> constexpr
int BitUtil::count1(T value) {
	int result = 0;
	int n = sizeof(T) * 8;
	T m = T(1);
	for (int i = 0; i < n; i++) {
		if (hasAny(value, m)) result++;
		m <<= 1;
	}
	return result;
}

}