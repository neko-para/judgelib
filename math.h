#ifndef _MATH_H_
#define _MATH_H_

#ifdef __cplusplus
extern "C" {
#endif

double cos(double x);
double sin(double x);
double tan(double x);
double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double cosh(double x);
double sinh(double x);
double tanh(double x);
double exp(double x);
double frexp(double x, int* exp);
double ldexp(double x, int exp);
double log(double x);
double log10(double x);
double modf(double x, double* iptr);
double pow(double x, double y);
double sqrt(double x);
double ceil(double x);
double floor(double x);
double fmod(double x, double y);
double fabs(double x);

#ifdef __cplusplus
}

double abs(double x);
float abs(float x);
long double abs(long double x);
#endif

#endif