#pragma once

#include "Matrix.h"
#include "Vector3D.h"
#include <fstream>
#include <set>
class Model3D
{
private:
	Matrix<>	m_Vertices;
	Matrix<>	m_InitialVertices;
	Matrix<>	m_ProjectedVertices;
	Matrix<int> m_Faces;
	Matrix<int> m_Edges;
	int			m_FacesCount;
	Matrix<>	m_CumulativeAT;
public:
	Model3D() :
		m_Vertices(),
		m_Edges(),
		m_Faces(),
		m_ProjectedVertices(),
		m_InitialVertices( m_Vertices ),
		m_CumulativeAT( Identity() )
	{}

	Model3D( const Matrix<> Vertices, const Matrix<int> Faces ) :
		m_Vertices( Vertices ),
		m_Faces( Faces ),
		m_CumulativeAT( Identity() )
	{}

	Model3D( string VerticesName, string FacesName )
	{
		ifstream VerticesFlow( VerticesName );

		int vRows, vColumns;
		VerticesFlow >> vRows >> vColumns;
		m_Vertices.ReallocateCells( vRows, vColumns );
		VerticesFlow >> m_Vertices;

		ifstream FacesFlow( FacesName );
		
		int eRows, eColumns;
		FacesFlow >> eRows >> eColumns;
		m_Faces.ReallocateCells( eRows, eColumns );
		FacesFlow >> m_Faces;

		SetEdges();
	}

	Matrix<> GetVertices() { return m_Vertices; }
	Matrix<int> GetEdges() { return m_Edges; }
	Matrix<int> GetFaces() { return m_Faces; }
	Matrix<> GetProjectedVertices()	{ return m_ProjectedVertices; }
	double GetVertexX( int Num );
	double GetVertexY( int Num );
	double GetVertexZ( int Num );

	double GetProjectedX( int );
	double GetProjectedY( int );

	void SetEdges();

	void Apply( Matrix<> );
	void Project( Matrix <> );
};

double Model3D::GetVertexX( int Num )
{
	return m_Vertices( 1, Num ) / m_Vertices( 4, Num );
}

double Model3D::GetVertexY( int Num )
{
	return m_Vertices( 2, Num ) / m_Vertices( 4, Num );

}

double Model3D::GetVertexZ( int Num )
{
	return m_Vertices( 3, Num ) / m_Vertices( 4, Num );
}

double Model3D::GetProjectedX( int Num )
{
	return m_ProjectedVertices( 1, Num ) / m_ProjectedVertices( 3, Num );
}

double Model3D::GetProjectedY( int Num )
{
	return m_ProjectedVertices( 2, Num ) / m_ProjectedVertices( 3, Num );
}

void Model3D::SetEdges()
{
	/* c+* */
	set <pair<int, int>> EdgesSet;

	for ( int i = 1; i <= m_Faces.GetRowsCount(); i++ )
	{
		for( int j = 1; j <=2; j++ )
			for ( int k = j + 1; k <= 3; k++ )
			{
				int FirstNumber = m_Faces( i, j );
				int SecondNumber = m_Faces( i, k );

				if ( SecondNumber < FirstNumber )
					swap( FirstNumber, SecondNumber );

				/* To avoid nil edges */
				if ( FirstNumber == SecondNumber )
					continue;

				EdgesSet.insert( pair<int, int>( FirstNumber, SecondNumber ) );
			}
	}

	int i = 1;
	m_Edges = Matrix<int>( EdgesSet.size(), 2 );

	for ( auto PairIter : EdgesSet )
	{
		m_Edges( i, 1 ) = PairIter.first;
		m_Edges( i, 2 ) = PairIter.second;

		i++;
	}
	ofstream output( "newEdges.txt" );
	output << m_Edges;
}

void Model3D::Apply( Matrix<> M )
{
	m_CumulativeAT = M * m_CumulativeAT;

	m_Vertices = m_CumulativeAT * m_InitialVertices;
}

void Model3D::Project( Matrix<> M )
{
	m_ProjectedVertices = M * m_Vertices;
}