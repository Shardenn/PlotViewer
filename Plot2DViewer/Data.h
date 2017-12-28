#define _USE_MATH_DEFINES
#include <cmath>
#include "Matrix.h"

double					// Исходные значения параметров L, R, B, T
	L = -20,
	R = 20,
	B = -20,
	T = 20,
	t_min = 1,
	t_max = 20*M_PI;

const int VerticesCount = 3;
const int EdgesCount = 3;
double v[ VerticesCount *3] = {
	1, 2, 5,
	4, 6, 2,
	1, 1, 1
};

Matrix<> ModelVertices(3, VerticesCount, v);
int e [ EdgesCount * 2 ] = {
	1, 2,
	2, 3,
	3, 1
};
Matrix<int> ModelEdges( EdgesCount, 2, e);


double Parabola(double x)
{
	return x*x-2;
}

double Sinusoid(double x)
{
	return sin(x);
}

double r(double t)
{
	return (t*3+pow(t,0.33)*pow(-1,int(t*10)))/20;
}

double phi(double t)
{
	return 3.0/4.0*M_PI*cos(t);
}
