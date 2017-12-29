#pragma once

#include "Camera3D.h"
#include "Model3D.h"

class Scene3D : public Camera3D
{
	Model3D Model;
public:
	Scene3D( double L, double R, double B, double T, Model3D sModel ) : Camera3D( L, R, B, T )
	{
		Model = sModel;
	}

	void Render( HDC dc, bool axes = true );
	//void Axes( HDC dc ) override;

	void RotateScene( double, double );

	Model3D * GetModel()				{ return &Model; }
};

void Scene3D::Render( HDC dc, bool axes )
{
	if ( axes )
		Axes( dc );

	UpdateCamera();

	Model.Project( GetWorldToProject() );

	Matrix<int> edges = Model.GetEdges();
 	for ( int i = 1; i <= edges.GetRowsCount(); i++ )
	{
		double a = edges( i, 1 );
		double b = edges( i, 2 );

		double ProjectedX = Model.GetProjectedX( a );
		double ProjectedY = Model.GetProjectedY( a );

		MoveTo( ProjectedX, ProjectedY );

		ProjectedX = Model.GetProjectedX( b );
		ProjectedY = Model.GetProjectedY( b );

		LineTo( dc, ProjectedX, ProjectedY );
	}
}
/*
inline void Scene3D::Axes( HDC dc )
{
	MoveTo( 0, -20 );
	LineTo( dc, 0, 20 );

	MoveTo( -20, 0 );
	LineTo( dc, 20, 0 );
}
*/
void Scene3D::RotateScene( double dX, double dY )
{
	if ( abs( dX ) < std::numeric_limits<double>::epsilon() && abs( dY ) < std::numeric_limits<double>::epsilon() )
		return;

	double BufX = dX;
	double BufY = dY;
	double mSquareRoot = sqrt( BufX * BufX + BufY * BufY );

	dX /= mSquareRoot;
	dY /= mSquareRoot;

	Matrix<> NewNMatrix;
	Vector3D NewN;
	Vector3D NewTop;
	Vector3D VectorToRotateAround = GetTop() ^ GetN();

	NewTop = RotationAroundVector( VectorToRotateAround * ( dY > 0 ? -1.f : 1.f ), RotationAngleY * dY ) * GetTop();
	NewN = RotationAroundVector( VectorToRotateAround * ( dY > 0 ? -1.f : 1.f ), RotationAngleY * dY ) * GetN();
	
	NewN = RotationAroundVector( NewTop * ( dX > 0 ? -1.f : 1.f ), RotationAngleX * dX ) * NewN;

	NewTop.Normalize();
	NewN.Normalize();

	SetTop( NewTop );
	SetN( NewN );
	
	
}