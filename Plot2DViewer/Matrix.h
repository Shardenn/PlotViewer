#pragma once

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

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
				{}	// Конструктор по умолчанию
				Matrix( const Matrix& );					// Конструктор копирования
				Matrix( int, int );							// Конструктор нулевой матрицы
				Matrix( int, int, Cell* );						// Конструктор матрицы из списка
				~Matrix();								// Деструктор

				int GetRowsCount()
				{
					return m_Rows;
				}

				void ReallocateCells( int NewRows, int NewColumns )
				{
					FreeCells();

					AllocateCells( NewRows, NewColumns );

					for ( int i = 0; i < m_Rows; i++ )
						for ( int j = 0; j < m_Columns; j++ )
							m_cells [ i ] [ j ] = 0;

					m_Rows = NewRows;
					m_Columns = NewColumns;
				}

				Cell &operator()( int i, int j )
				{
					return m_cells [ i - 1 ] [ j - 1 ];
				}

				Matrix& operator = ( const Matrix& );		// Перегрузка оператора присваивания
				Matrix  operator + ( const Matrix& );		// Сложение матриц
				Matrix  operator - ( const Matrix& );		// Вычитание матриц
				Matrix  operator * ( const Matrix& );		// Умножение матриц

				friend istream& operator >> <> ( istream&, Matrix& );			// Перегрузка оператора >> для ввода матрицы
				friend ostream& operator << <> ( ostream&, const Matrix& );	// Перегрузка оператора << для вывода матрицы
};


template <typename Cell>
Matrix<Cell>::Matrix( const Matrix<Cell>& M )
{
	AllocateCells( M.m_Rows, M.m_Columns );

	for ( int i = 0; i < m_Rows; i++ )
		for ( int j = 0; j < m_Columns; j++ )
			m_cells [ i ] [ j ] = M.m_cells [ i ] [ j ];
}

template <typename Cell>
Matrix<Cell>::Matrix( int Rows, int Columns )
{
	AllocateCells( Rows, Columns );

	for ( int i = 0; i < Rows; i++ )
		for ( int j = 0; j < Columns; j++ )
			m_cells [ i ] [ j ] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix( int Rows, int Columns, Cell* list )
{
	AllocateCells( Rows, Columns );

	for ( int i = 0; i < Rows; i++ )
		for ( int j = 0; j < Columns; j++ )
			m_cells [ i ] [ j ] = list [ i*Columns + j ];
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=( const Matrix& M )
{
	if ( m_Rows != M.m_Rows || m_Columns != M.m_Columns )
	{
		FreeCells();
		AllocateCells( M.m_Rows, M.m_Columns );
	}

	for ( int i = 0; i < m_Rows; i++ )
		for ( int j = 0; j < m_Columns; j++ )
			m_cells [ i ] [ j ] = M.m_cells [ i ] [ j ];

	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+( const Matrix& M )
{
	if ( m_Rows != M.m_Rows || m_Columns != M.m_Columns )
		return Matrix();

	Matrix<Cell> Result( *this );

	for ( int i = 0; i < m_Rows; i++ )
		for ( int j = 0; j < m_Columns; j++ )
			Result.m_cells [ i ] [ j ] += M.m_cells [ i ] [ j ];

	return Result;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-( const Matrix& M )
{
	if ( m_Rows != M.m_Rows || m_Columns != M.m_Columns )
		return Matrix();

	Matrix<Cell> Result( *this );

	for ( int i = 0; i < m_Rows; i++ )
		for ( int j = 0; j < m_Columns; j++ )
			Result.m_cells [ i ] [ j ] -= M.m_cells [ i ] [ j ];

	return Result;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*( const Matrix& M )
{
	if ( m_Columns != M.m_Rows )
		return Matrix();

	Matrix<Cell> Result( m_Rows, M.m_Columns );

	for ( int i = 0; i < m_Rows; i++ )
		for ( int j = 0; j < M.m_Columns; j++ )
			for ( int l = 0; l < m_Columns; l++ )
				Result.m_cells [ i ] [ j ] += m_cells [ i ] [ l ] * M.m_cells [ l ] [ j ];

	return Result;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells( int Rows, int Columns )
{
	m_cells = new Cell* [ Rows ];

	for ( int i = 0; i < Rows; i++ )
		m_cells [ i ] = new Cell [ Columns ];

	m_Rows = Rows;
	m_Columns = Columns;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for ( int i = 0; i < m_Rows; i++ )
		delete m_cells [ i ];

	delete m_cells;

	m_Rows = 0;
	m_Columns = 0;
}

template <typename Cell>
inline istream& operator >> <>( istream& fi, Matrix<Cell>& M )
{
	for ( int i = 0; i < M.m_Rows; i++ )
		for ( int j = 0; j < M.m_Columns; j++ )
			fi >> M.m_cells [ i ] [ j ];
	return fi;
}

template <typename Cell>
ostream& operator << <>( ostream& fo, const Matrix<Cell>& M )
{
	for ( int i = 0; i < M.m_Rows; i++ )
	{
		fo << "  ";
		for ( int j = 0; j < M.m_Columns; j++ )
			fo << M.m_cells [ i ] [ j ] << " \t";
		fo << endl;
	}
	return fo;
}

#endif MATRIX_H
