#pragma once

#include "Camera2D.h"
#include "Vector3D.h"
#include "Matrix.h"

class Camera3D : public Camera2D
{
private :
	Vector3D Ov, Top, N;
	double D;

	Matrix<> WorldToView;
	Matrix<> ViewToProject;
	Matrix<> WorldToProject;

	Camera3D( double L, double R, double B, double T ) : Camera2D( L, R, B, T )
	{
		SetOv( Vector3D( 0, 0, 0 ) );
		SetTop( Vector3D( 0, 1, 0 ) );
		SetN( Vector3D( 0, 0, 1 ) );
		D = 10;
	}

public:
	void SetOv	( Vector3D VectorToSet );
	void SetTop	( Vector3D VectorToSet );
	void SetN	( Vector3D VectorN );

	void UpdateCamera();
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

void Camera3D::UpdateCamera()
{

}