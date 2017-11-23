#ifndef _MATH_H_
#define _MATH_H_

#define NAN (0.0f/0.0f)
#define HUGE_VAL (1.0/0.0)

#ifdef __cplusplus
#define _MATH_FUNC_(name) \
	extern "C" double name(double x); \
	extern "C" float name##f(float x); \
	extern "C" long double name##l(long double x); \
	inline float name(float x) { \
		return name##f(x); \
	} \
	inline long double name(long double x) { \
		return name##l(x); \
	}
#define _MATH_FUNC2_(name) \
	extern "C" double name(double x, double y); \
	extern "C" float name##f(float x, float y); \
	extern "C" long double name##l(long double x, long double y); \
	inline float name(float x, float y) { \
		return name##f(x, y); \
	} \
	inline long double name(long double x, long double y) { \
		return name##l(x, y); \
	}
#define _MATH_FUNC2P_(name) \
	extern "C" double name(double x, double* y); \
	extern "C" float name##f(float x, float* y); \
	extern "C" long double name##l(long double x, long double* y); \
	inline float name(float x, float* y) { \
		return name##f(x, y); \
	} \
	inline long double name(long double x, long double* y) { \
		return name##l(x, y); \
	}
#define _MATH_FUNC_EXTRA_(name, type) \
	extern "C" double name(double x, type y); \
	extern "C" float name##f(float x,type y); \
	extern "C" long double name##l(long double x, type y); \
	inline float name(float x, type y) { \
		return name##f(x, y); \
	} \
	inline long double name(long double x, type y) { \
		return name##l(x, y); \
	}
#else
#define _MATH_FUNC_(name) \
	double name(double x); \
	float name##f(float x); \
	long double name##l(long double x);
#define _MATH_FUNC2_(name) \
	double name(double x, double y); \
	float name##f(float x, float y); \
	long double name##l(long double x, long double y);
#define _MATH_FUNC2P_(name) \
	double name(double x, double* y); \
	float name##f(float x, float* y); \
	long double name##l(long double x, long double* y);
#define _MATH_FUNC_EXTRA_(name, type) \
	double name(double x, type y); \
	float name##f(float x,type y); \
	long double name##l(long double x, type y);
#endif

_MATH_FUNC_(cos)
_MATH_FUNC_(sin)
_MATH_FUNC_(tan)
_MATH_FUNC_(acos)
_MATH_FUNC_(asin)
_MATH_FUNC_(atan)
_MATH_FUNC2_(atan2)
_MATH_FUNC_(cosh)
_MATH_FUNC_(sinh)
_MATH_FUNC_(tanh)
_MATH_FUNC_(exp)
_MATH_FUNC_EXTRA_(frexp, int*)
_MATH_FUNC_EXTRA_(ldexp, int)
_MATH_FUNC_(log)
_MATH_FUNC_(log10)
_MATH_FUNC2P_(modf)
_MATH_FUNC2_(pow)
_MATH_FUNC_(sqrt)
_MATH_FUNC_(ceil)
_MATH_FUNC_(floor)
_MATH_FUNC_(round)
_MATH_FUNC2_(fmod)
_MATH_FUNC_(fabs)

#endif