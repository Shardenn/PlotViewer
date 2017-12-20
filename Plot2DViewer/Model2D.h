#pragma once

#ifndef MODEL_2D_H
#define MODEL_2D_H

#include <string>
#include <fstream>
#include "Matrix.h"
#include "AffineTransform.h"

class Model2D
{
private:
	Matrix<>	m_Vertices;
	int			m_vRows, m_vColumns;
	Matrix<int> m_Edges;
	int			m_eRows, m_eColumns;
	Matrix<>	m_CumulativeAT;
	Matrix<>	m_InitialVertices;
public:
	Model2D() :
		//m_Vertices(),
		//m_Edges(),
		m_CumulativeAT( Identity() ),
		m_InitialVertices( m_Vertices )
	{}
	
	Model2D( string VerticesName, string EdgesName ) :
		m_CumulativeAT( Identity() )
	{
		ifstream VerticesFlow( VerticesName );

		VerticesFlow >> m_vRows >> m_vColumns;
		m_Vertices.ReallocateCells( m_vRows, m_vColumns );
		VerticesFlow >> m_Vertices;

		ifstream  EdgesFlow( EdgesName );

		EdgesFlow >> m_eRows >> m_eColumns;
		m_Edges.ReallocateCells( m_eRows, m_eColumns );
		EdgesFlow >> m_Edges;

		m_InitialVertices = m_Vertices;
	}

	Model2D( const Matrix<> Vertices, const Matrix<int> Edges ) :
		m_Vertices( Vertices ),
		m_Edges( Edges )
	{}

	Model2D *operator=( const Model2D Model )
	{
		m_Edges = Model.m_Edges;
		m_InitialVertices = Model.m_InitialVertices;
		m_Vertices = Model.m_Vertices;
		m_CumulativeAT = Model.m_CumulativeAT;

		return this;
	}

	Matrix<> GetVertices()
	{
		return m_Vertices;
	}

	Matrix<int> GetEdges()
	{
		return m_Edges;
	}

	double GetVertexX( int Num )
	{
		return m_Vertices( 1, Num ) / m_Vertices( 3, Num );
	}

	double GetVertexY( int Num )
	{
		return m_Vertices( 2, Num ) / m_Vertices( 3, Num );
	}

	void Apply( Matrix<> M )
	{
		m_CumulativeAT = M * m_CumulativeAT;

		m_Vertices = m_CumulativeAT * m_InitialVertices;
	}
};
#endif MODEL_2D_H