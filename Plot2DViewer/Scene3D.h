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
};

void Scene3D::Render( HDC dc, bool axes )
{
	if ( axes )
		Axes( dc );

	UpdateCamera();

	Model.Project( GetWorldToProject() );

	for ( int i = 0; i < Model.GetEdges().GetRowsCount(); i++ )
	{
		MoveTo( Model.GetProjectedX( Model.GetEdges()( i, 1 ) ), Model.GetProjectedY( Model.GetEdges()( i, 1 ) ) );
		LineTo( dc, Model.GetProjectedX( Model.GetEdges()( i, 2 ) ), Model.GetProjectedY( Model.GetEdges()( i, 2 ) ) );
	}
}