#pragma once

#include "Camera3D.h"
#include "Model3D.h"
#include <set>

class Scene3D : public Camera3D
{
	Model3D m_Models [ 10 ];
	int m_ModelsCount;
	Model3D m_SelectedModel;
	Model3D m_AxesModel;

public:
	Scene3D( double L, double R, double B, double T, Model3D Models[], int ModelsCount ) :
		Camera3D( L, R, B, T ),
		m_AxesModel( "AxesVertices.txt", "AxesFaces.txt" ),
		m_ModelsCount( ModelsCount + 1 )
	{
		/* Initialize array of models */
		for( int i = 0; i < ModelsCount; i++ )
		{
			m_Models[ i ] = Models[ i ];
		}
		m_Models[ ModelsCount ] = m_AxesModel;

		m_SelectedModel = m_Models[ 0 ];

		//Model3D AxesModel( "AxesVertices.txt", "AxesFaces.txt" );
		//Models.insert( AxesModel );
	}

	void RenderAll( HDC dc );
	void Render( HDC dc );

	void RotateScene( double, double );
	void MoveScene( double, double );
	void ZoomScene( int );

	Vector3D GetScreenVectorFromMouseMovement( double dx, double dy );

	Model3D * GetAllModels() { return m_Models; }

	Model3D * GetModel()
	{
		return &m_SelectedModel;
	}
};

void Scene3D::Render( HDC dc )
{	
	UpdateCamera();

	m_SelectedModel.Project( GetWorldToProject() );

	Matrix<int> edges = m_SelectedModel.GetEdges();
	for( int i = 1; i <= edges.GetRowsCount(); i++ )
	{
		double a = edges( i, 1 );
		double b = edges( i, 2 );

		double ProjectedX = m_SelectedModel.GetProjectedX( a );
		double ProjectedY = m_SelectedModel.GetProjectedY( a );

		MoveTo( ProjectedX, ProjectedY );

		ProjectedX = m_SelectedModel.GetProjectedX( b );
		ProjectedY = m_SelectedModel.GetProjectedY( b );

		LineTo( dc, ProjectedX, ProjectedY );
	}
	
}

void Scene3D::RenderAll( HDC dc )
{
	UpdateCamera();
	
	for( int l = 0; l < m_ModelsCount; l++ )
	{
		Model3D CurrentlyDrawnModel = m_Models[ l ];

		CurrentlyDrawnModel.Project( GetWorldToProject() );

		Matrix<int> edges = CurrentlyDrawnModel.GetEdges();
		for( int i = 1; i <= edges.GetRowsCount(); i++ )
		{
			double a = edges( i, 1 );
			double b = edges( i, 2 );

			double ProjectedX = CurrentlyDrawnModel.GetProjectedX( a );
			double ProjectedY = CurrentlyDrawnModel.GetProjectedY( a );

			MoveTo( ProjectedX, ProjectedY );

			ProjectedX = CurrentlyDrawnModel.GetProjectedX( b );
			ProjectedY = CurrentlyDrawnModel.GetProjectedY( b );

			LineTo( dc, ProjectedX, ProjectedY );
		}
	}
}

void Scene3D::RotateScene( double dX, double dY )
{
	if( !Vector3D::NormalizeVector2D( dX, dY ) )
		return;

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

void Scene3D::MoveScene( double dX, double dY )
{
	Vector3D Movement = GetScreenVectorFromMouseMovement( dX, dY );

	SetOv( GetOv() - ( Movement /* * abs( dX ) + VerticalMovement * abs( dY )*/ ) * MovingSpeed );
}

void Scene3D::ZoomScene( int WheelDelta )
{
	short K = WheelDelta > 0 ? -1 : 1;

	SetOv( GetOv() + GetN() * K );
	SetD( GetD() + K * GetN().Length() );
}

/*
* Returns vector that belongs to plane, perpendicular to camera sight
* dx dy - mouse movement vector in 2d plane ( on table )
*/
Vector3D Scene3D::GetScreenVectorFromMouseMovement( double dx, double dy )
{
	if( !Vector3D::NormalizeVector2D( dx, dy ) )
		return Vector3D( 0 );

	double phiX = Vector3D::CosBetween( Vector3D( 1, 0, 0 ), Vector3D( dx, -dy, 0 ) );
	double phiY = Vector3D::CosBetween( Vector3D( 0, 1, 0 ), Vector3D( dx, -dy, 0 ) );

	Vector3D RightVector = GetTop() ^ GetN();

	Vector3D HorizontalMovement = RightVector / phiX;
	Vector3D VerticalMovement = GetTop() / phiY;
	
	VerticalMovement.Normalize();
	HorizontalMovement.Normalize();
	
	return HorizontalMovement * abs( dx ) + VerticalMovement * abs( dy );
}
