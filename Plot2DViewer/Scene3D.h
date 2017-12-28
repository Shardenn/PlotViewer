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