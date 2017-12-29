#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"
#include "Data.h"
#include "Model2D.h"

class Scene2D : public Camera2D
{
private:
	using Func = double( * )( double );
	

public:
	Model2D m_Model;
	Scene2D( double L, double R, double B, double T, Model2D Model ) :
		Camera2D( L, R, B, T )
	{
		m_Model = Model;
	}
	
	Model2D *GetModel()
	{
		return &m_Model;
	}

	void Render( HDC dc, bool axes = true )
	{
		if ( axes )
			Axes( dc );

		Matrix<int> Edges = m_Model.GetEdges();

		for ( int i = 1; i <= m_Model.GetEdges().GetRowsCount(); i++ )
		{
			MoveTo( m_Model.GetVertexX( Edges( i, 1 ) ), m_Model.GetVertexY( Edges( i, 1 ) ) );
			LineTo( dc, m_Model.GetVertexX( Edges( i, 2 ) ), m_Model.GetVertexY( Edges( i, 2 ) ) );
		}
	}

	void Plot(HDC dc, Func f, bool axes = true)
	{
		if ( axes )
			Axes( dc );


		MoveTo( L, f( L ) );

		double
			Step = ( R - L ) / W,
			x = L + Step/2;

		for ( ; x <= R; x += Step )
		{
			LineTo( dc, x, f( x ) );
		}
	}
};

#endif SCENE_2D_H
