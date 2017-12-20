#pragma once

#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H


Matrix<> Translation( double x, double y )
{
	double T [ 9 ] = {
		1, 0, x,
		0, 1, y,
		0, 0, 1 };
	return Matrix<>( 3, 3, T );
}

Matrix<> Identity()
{
	double T [ 9 ] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};

	return Matrix<>( 3, 3, T );
}

Matrix<> Rotation( double X )
{
	double T [ 9 ] = {
		cos( X ), -sin( X ), 0,
		sin( X ),  cos( X ), 0,
		0,			0,		 1
	};

	return Matrix<>( 3, 3, T );
}

Matrix<> Rotation( double C, double S )
{
	double K = sqrt( pow( C, 2 ) + pow( S, 2 ) );

	double T [ 9 ] = {
		C/K, -S/K, 0,
		S/K,  C/K, 0,
		0,	0, 1
	};

	return Matrix<>( 3, 3, T );
}

Matrix<> Scailing( double Kx, double Ky )
{
	double T [ 9 ] = {
		Kx, 0, 0,
		0, Ky, 0,
		0, 0,  1
	};

	return Matrix<>( 3, 3, T );
}

Matrix<> Mapping( bool isXAxis )
{
	if ( isXAxis ) return Scailing( -1, 1 );
	if ( !isXAxis ) return Scailing( 1, -1 );
}

Matrix<> ScaleAroundPoint( float CoordX, float CoordY, float ScaleSpeed, bool bScaleUp )
{
	float ScaleKoeff = bScaleUp ? 1.1 : 0.9;
	return Translation( CoordX, CoordY ) * 
		Scailing( ScaleKoeff * ScaleSpeed, ScaleKoeff * ScaleSpeed )  *
		Translation( -CoordX, -CoordY );
}

Matrix<> MapAroundLine( float FirstX, float FirstY, float SecondX, float SecondY )
{
	return Translation( FirstX, FirstY ) *
		Rotation( SecondX-FirstX, SecondY-FirstY) *
		Mapping( false ) *
		Rotation( SecondX-FirstX, FirstY-SecondY) *
		Translation( -FirstX, -FirstY);
	/*
		Rotation( FirstY - SecondYFirstX - SecondX,  ) *
		Mapping( true ) *
		Rotation( SecondY - FirstY, FirstX - SecondX ) *
		Translation( -FirstX, -FirstY );
		*/
}
// Здесь требуется реализовать функции, возвращающие матрицы базовых АП:
// Identity() - тождественное АП;
// Rotation(t) - поворот на угол t;
// Rotation(c, s) - поворот на угол, косинус и синус которого пропорциональны величинам c и s;
// Scaling(kx, ky) - масштабирование;
// Mapping (различные виды отражений) - по желанию, для создания матриц отражения можно использовать функцию Scaling.

// В приведённом примере используется конструктор матрицы из списка.
// Приветствуются идеи о других способах создания матриц.
// Например, вызов функции, создающей единичную матрицу,
// с последующим переопределением некоторых её коэффициентов.

#endif AFFINE_TRANSFORM_H
