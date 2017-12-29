#pragma once

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "Vector3D.h"

using namespace std;

template <typename Cell = double>
class Matrix
{
private:
	int			m_Rows, m_Columns;
	Cell		**m_cells;
	void		AllocateCells( int, int );
	void		FreeCells();

public:
	Matrix() :
		m_Rows( 0 ),
		m_Columns( 0 ),
		m_cells( nullptr )
	{}
	Matrix( const Matrix& );					
	Matrix( int, int );							
	Matrix( int, int, Cell* );						
	~Matrix();								

	int GetRowsCount() { return m_Rows; }

	int GetColsCount() { return m_Columns; }

	void ReallocateCells( int NewRows, int NewColumns )
	{
		FreeCells();

		AllocateCells( NewRows, NewColumns );

		for( int i = 0; i < m_Rows; i++ )
			for( int j = 0; j < m_Columns; j++ )
				m_cells[ i ][ j ] = 0;

		m_Rows = NewRows;
		m_Columns = NewColumns;
	}

	Cell &operator()( int i, int j )
	{
		return m_cells[ i - 1 ][ j - 1 ];
	}

	Matrix& operator = ( const Matrix& );		// Перегрузка оператора присваивания
	Matrix  operator + ( const Matrix& );		// Сложение матриц
	Matrix  operator - ( const Matrix& );		// Вычитание матриц
	Matrix  operator * ( const Matrix& );		// Умножение матриц
	Vector3D  operator * ( const Vector3D& );

	friend istream& operator >> <> ( istream&, Matrix& );			// Перегрузка оператора >> для ввода матрицы
	friend ostream& operator << <> ( ostream&, const Matrix& );	// Перегрузка оператора << для вывода матрицы
};


template <typename Cell>
Matrix<Cell>::Matrix( const Matrix<Cell>& M )
{
	AllocateCells( M.m_Rows, M.m_Columns );

	for( int i = 0; i < m_Rows; i++ )
		for( int j = 0; j < m_Columns; j++ )
			m_cells[ i ][ j ] = M.m_cells[ i ][ j ];
}

template <typename Cell>
Matrix<Cell>::Matrix( int Rows, int Columns )
{
	AllocateCells( Rows, Columns );

	for( int i = 0; i < Rows; i++ )
		for( int j = 0; j < Columns; j++ )
			m_cells[ i ][ j ] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix( int Rows, int Columns, Cell* list )
{
	AllocateCells( Rows, Columns );

	for( int i = 0; i < Rows; i++ )
		for( int j = 0; j < Columns; j++ )
			m_cells[ i ][ j ] = list[ i*Columns + j ];
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=( const Matrix& M )
{
	if( m_Rows != M.m_Rows || m_Columns != M.m_Columns )
	{
		ReallocateCells( M.m_Rows, M.m_Columns );
	}

	for( int i = 0; i < m_Rows; i++ )
		for( int j = 0; j < m_Columns; j++ )
			m_cells[ i ][ j ] = M.m_cells[ i ][ j ];

	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+( const Matrix& M )
{
	if( m_Rows != M.m_Rows || m_Columns != M.m_Columns )
		return Matrix();

	Matrix<Cell> Result( *this );

	for( int i = 0; i < m_Rows; i++ )
		for( int j = 0; j < m_Columns; j++ )
			Result.m_cells[ i ][ j ] += M.m_cells[ i ][ j ];

	return Result;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-( const Matrix& M )
{
	if( m_Rows != M.m_Rows || m_Columns != M.m_Columns )
		return Matrix();

	Matrix<Cell> Result( *this );

	for( int i = 0; i < m_Rows; i++ )
		for( int j = 0; j < m_Columns; j++ )
			Result.m_cells[ i ][ j ] -= M.m_cells[ i ][ j ];

	return Result;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*( const Matrix& M )
{
	if( m_Columns != M.m_Rows )
		return Matrix();

	Matrix<Cell> Result( m_Rows, M.m_Columns );

	for( int i = 0; i < m_Rows; i++ )
		for( int j = 0; j < M.m_Columns; j++ )
			for( int l = 0; l < m_Columns; l++ )
				Result.m_cells[ i ][ j ] += m_cells[ i ][ l ] * M.m_cells[ l ][ j ];

	return Result;
}

template<typename Cell>
inline Vector3D Matrix<Cell>::operator*( const Vector3D & other )
{
	if( m_Columns != 3 || m_Rows != 3 )
		return Vector3D( 0 );

	Vector3D Result( m_cells[ 0 ][ 0 ] * other.X + m_cells[ 0 ][ 1 ] * other.Y + m_cells[ 0 ][ 2 ] * other.Z,
					 m_cells[ 1 ][ 0 ] * other.X + m_cells[ 1 ][ 1 ] * other.Y + m_cells[ 1 ][ 2 ] * other.Z,
					 m_cells[ 2 ][ 0 ] * other.X + m_cells[ 2 ][ 1 ] * other.Y + m_cells[ 2 ][ 2 ] * other.Z );

	return Result;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells( int Rows, int Columns )
{
	m_cells = new Cell*[ Rows ];

	for( int i = 0; i < Rows; i++ )
		m_cells[ i ] = new Cell[ Columns ];

	m_Rows = Rows;
	m_Columns = Columns;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for( int i = 0; i < m_Rows; i++ )
		delete m_cells[ i ];

	if( m_Rows > 0 )
		delete m_cells;

	m_Rows = 0;
	m_Columns = 0;
}

template <typename Cell>
inline istream& operator >> <>( istream& fi, Matrix<Cell>& M )
{
	for( int i = 0; i < M.m_Rows; i++ )
		for( int j = 0; j < M.m_Columns; j++ )
			fi >> M.m_cells[ i ][ j ];
	return fi;
}

template <typename Cell>
ostream& operator << <>( ostream& fo, const Matrix<Cell>& M )
{
	for( int i = 0; i < M.m_Rows; i++ )
	{
		fo << "  ";
		for( int j = 0; j < M.m_Columns; j++ )
			fo << M.m_cells[ i ][ j ] << " \t";
		fo << endl;
	}
	return fo;
}

#endif MATRIX_H
