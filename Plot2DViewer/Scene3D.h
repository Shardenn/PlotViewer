#pragma once

#include "Camera3D.h"
#include "Model3D.h"
#include <set>

class Scene3D : public Camera3D
{
	//Model3D m_Models [ 10 ] = {};
	set<Model3D> m_Models;

	Model3D m_AxesModel;

public:
	Scene3D( double L, double R, double B, double T, set<Model3D> Models ) :
		Camera3D( L, R, B, T ),
		m_AxesModel( "AxesVertices.txt", "AxesFaces.txt" )
	{
		for( auto CurModel : Models )
		{
			m_Models.insert( CurModel );
		}
	}

	void RenderAll( HDC dc );

	void RotateScene( double, double );
	void MoveScene( double, double );
	void ZoomScene( int );

	set<Model3D> * GetAllModels() { return &m_Models; }

	Model3D * GetModel()
	{
		for( auto Model : m_Models )
		{
			return &Model;
		}
	}
};

void Scene3D::RenderAll( HDC dc )
{
	UpdateCamera();

	for( auto CurModel : m_Models )
	{
		CurModel.Project( GetWorldToProject() );

		Matrix<int> edges = CurModel.GetEdges();
		for( int i = 1; i <= edges.GetRowsCount(); i++ )
		{
			double ProjectedX = CurModel.GetProjectedX( edges( i, 1 ) );
			double ProjectedY = CurModel.GetProjectedY( edges( i, 1 ) );

			MoveTo( ProjectedX, ProjectedY );

			ProjectedX = CurModel.GetProjectedX( edges( i, 2 ) );
			ProjectedY = CurModel.GetProjectedY( edges( i, 2 ) );

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
	if( !Vector3D::NormalizeVector2D( dX, dY ) )
		return;
	
	double phiX = Vector3D::CosBetween( Vector3D( 1, 0, 0 ), Vector3D( dX, -dY, 0 ) );
	double phiY = Vector3D::CosBetween( Vector3D( 0, 1, 0 ), Vector3D( dX, -dY, 0 ) );

	Vector3D RightVector = GetTop() ^ GetN();

	Vector3D SideMovement = RightVector / phiX;
	Vector3D VerticalMovement = GetTop() / phiY;

	SideMovement.Normalize();
	VerticalMovement.Normalize();

	SetOv( GetOv() - ( SideMovement * abs( dX ) + VerticalMovement * abs( dY ) ) * MovingSpeed );
}

void Scene3D::ZoomScene( int WheelDelta )
{
	short K = WheelDelta > 0 ? -1 : 1;

	SetOv( GetOv() + GetN() * K );
	SetD( GetD() + K * GetN().Length() );
}
