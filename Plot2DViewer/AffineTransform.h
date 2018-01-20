#pragma once

#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include "Vector3D.h"

Matrix<> Translation( double x, double y )
{
	double T [ 9 ] = {
		1, 0, x,
		0, 1, y,
		0, 0, 1 };
	return Matrix<>( 3, 3, T );
}

Matrix<> Translation( double x, double y, double z )
{
	double T [ 16 ] = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};

	return Matrix<>( 4, 4, T );
}

Matrix<> Translation( Vector3D V )
{
	double T [ 16 ] = {
		1, 0, 0, V.X,
		0, 1, 0, V.Y,
		0, 0, 1, V.Z,
		0, 0, 0, 1
	};

	return Matrix<>( 4, 4, T );
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

Matrix<> Identity3D()
{
	double T [ 16 ] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return Matrix<>( 4, 4, T );
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

Matrix<> RotationX( double t )
{
	double T [ 16 ] = {
		1, 0, 0, 0,
		0, cos( t ), -sin( t ), 0,
		0, sin( t ), cos( t ), 0,
		0, 0, 0, 1
	};

	return Matrix<>( 4, 4, T );
}

Matrix<> RotationY( double t )
{
	double T [ 16 ] = {
		cos( t ), 0, sin( t ), 0,
		0, 1, 0, 0,
		-sin( t ), 0, cos( t ), 0,
		0, 0, 0, 1
	};

	return Matrix<>( 4, 4, T );
}

Matrix<> RotationZ( double t )
{
	double T [ 16 ] = {
		cos( t ), -sin( t ), 0, 0,
		sin( t ), cos( t ), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return Matrix<>( 4, 4, T );
}

Matrix<> RotationAroundVector( Vector3D U, float Angle )
{
	float C = cos( Angle );
	float S = sqrt( 1 - C * C );
	float OneC = 1 - C;

	double T [ ] = {
		C + U.X * U.X * OneC , U.X * U.Y * OneC - U.Z * S , U.X * U.Z * OneC + U.Y * S,
		U.Y * U.X  * OneC + U.Z * S, C + U.Y * U.Y * OneC, U.Y * U.Z * OneC - U.X * S,
		U.Z * U.X * OneC - U.Y * S, U.Z * U.Y * OneC + U.X * S, C + U.Z * U.Z * OneC
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

Matrix<> Scailing( double Kx, double Ky, double Kz )
{
	double T [ 16 ] = {
		Kx, 0, 0, 0,
		0, Ky, 0, 0,
		0, 0, Kz, 0,
		0, 0, 0, 1
	};

	return Matrix<>( 4, 4, T );
}

Matrix<> Mapping( bool isXAxis )
{
	if ( isXAxis ) return Scailing( -1, 1 );
	if ( !isXAxis ) return Scailing( 1, -1 );
	return Scailing( 1, 1 );
}

Matrix<> ScaleAroundPoint( float CoordX, float CoordY, float ScaleSpeed, bool bScaleUp )
{
	float ScaleKoeff = bScaleUp ? 1.1f : 0.9f;
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
}

Matrix<> RotatePerpendicularToSight( float Angle, Vector3D LinePoint, Vector3D LineVector )
{
	Vector3D ProjectedVector = Vector3D( LineVector.X, 0, LineVector.Z );
	/* What angle do we need to rotate in order to move vector in plane which contains X axis */
	auto YCosAngle = Vector3D::CosBetween( ProjectedVector, Vector3D( 1, 0, 0 ) );
	auto NewX = ProjectedVector.Length();
	auto ZCosAngle = Vector3D::CosBetween( Vector3D( 1, 0, 0 ), Vector3D( NewX, LineVector.Y, 0 ) );

	return Translation( LinePoint ) *
		RotationY( -YCosAngle ) *
		RotationZ( ZCosAngle ) *
		RotationX( Angle ) *
		RotationZ( -ZCosAngle ) *
		RotationY( YCosAngle )*
		Translation( LinePoint * (-1) );
}

#endif AFFINE_TRANSFORM_H
