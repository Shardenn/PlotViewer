#pragma once

#include "Camera2D.h"
#include "Vector3D.h"
#include "Matrix.h"

class Camera3D : public Camera2D
{
private :
	Vector3D Ov, Top, N;
	double D;

	bool bIsRotating;
	bool bIsMoving;

	Matrix<> WorldToView;
	Matrix<> ViewToProject;
	Matrix<> WorldToProject;

public:

	Camera3D( double L, double R, double B, double T ) : 
		Camera2D( L, R, B, T ), 
		bIsRotating( false )
	{
		SetOv( Vector3D( 0, 0, 0 ) );
		SetTop( Vector3D( 0, 1, 0 ) );
		SetN( Vector3D( 0, 0, 1 ) );
		D = 10;
	}

	//void Axes( HDC dc ) override;
	void				SetOv	( Vector3D VectorToSet );
	void				SetTop	( Vector3D VectorToSet );
	void				SetN	( Vector3D VectorN );
	void				SetD	( double newD );
	void				SetIsRotating( bool bSet );
	void				SetIsMoving( bool bSet );

	void				UpdateCamera();
	
	bool				IsRotating()							{ return bIsRotating; }
	bool				IsMoving()								{ return bIsMoving; }
	Vector3D			GetTop()								{ return Top; }
	Vector3D			GetN()									{ return N; }
	Vector3D			GetOv()									{ return Ov; }
	double				GetD()									{ return D; }

	Matrix<>			GetWorldToProject()						{ return WorldToProject; }

	const double RotationAngleX = 0.04;
	const double RotationAngleY = 0.04;
	const double MovingSpeed	= 0.35;
	const double ZoomingRate	= 0.5;
};

void Camera3D::SetOv( Vector3D newVector )
{
	Ov = newVector;
}

void Camera3D::SetTop( Vector3D newVector )
{
	Top = newVector;
}

void Camera3D::SetN( Vector3D newVector )
{
	N = newVector;
}

void Camera3D::SetD( double newD )
{
	D = newD;
}

void Camera3D::SetIsRotating( bool bSet )
{
	bIsRotating = bSet;
}

void Camera3D::SetIsMoving( bool bSet )
{
	bIsMoving = bSet;
}

void Camera3D::UpdateCamera()
{
	Vector3D i = ( Top ^ N );
	i.Normalize();

	Vector3D k = N;
	k.Normalize();

	Vector3D j = k ^ i;


	double a [ ] = { i.X, i.Y, i.Z, -( i.X * Ov.X + i.Y * Ov.Y + i.Z * Ov.Z ),
					j.X, j.Y, j.Z, -( j.X * Ov.X + j.Y * Ov.Y + j.Z * Ov.Z ),
					k.X, k.Y, k.Z, -( k.X * Ov.X + k.Y * Ov.Y + k.Z * Ov.Z ),
					0, 0, 0, 1
	};

	WorldToView = Matrix<>( 4, 4, a );

	double b [ ] = { 1,0,0,0,
					0,1,0,0,
					0,0,(-1/D ),1 };

	ViewToProject = Matrix<>( 3, 4, b );

	WorldToProject = ViewToProject * WorldToView;
}