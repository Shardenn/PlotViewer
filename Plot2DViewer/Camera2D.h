#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{

protected:
	double L, R, B, T;					// ћировые координаты границ рабочей области окна

	int W, H;							// –азрешение рабочей области окна

	int WorldToScreenX( double X )
	{
		return ( int ) ( ( X - L ) / ( R - L ) * W );
	}

	int WorldToScreenY( double Y )
	{
		return ( int ) ( ( T - Y ) / ( T - B ) * H );
	}

	double ScreenToWorldX( int X )
	{
		return L + ( R - L ) * ( X + 0.5 ) / W;
	}

	double ScreenToWorldY( int Y )
	{
		return T - ( T - B ) * ( Y + 0.5 ) / H;
	}

private:
	double posX, posY;

	bool bDragging = false;
	double prevMouseX, prevMouseY;

public:
	Camera2D( double L, double R, double B, double T ) : L( L ), R( R ), B( B ), T( T )
	{}

	/* Clears the screen */
	void Clear( HDC dc )
	{
		RECT rectangle;
		GetClientRect( WindowFromDC( dc ), &rectangle );
		Rectangle( dc, rectangle.left, rectangle.top, rectangle.right, rectangle.bottom );
	}

	void SetResolution( HDC dc )
	{
		RECT rectangle;
		GetClientRect( WindowFromDC( dc ), &rectangle );

		W = rectangle.right - rectangle.left + 1;
		H = rectangle.bottom - rectangle.top + 1;

		double newL = ( L + R ) / 2 - ( T - B ) / 2 * W / H;
		double newR = ( L + R ) / 2 + ( T - B ) / 2 * W / H;

		L = newL;
		R = newR;
	}

	void MoveTo( double X, double Y )
	{
		posX = WorldToScreenX( X );
		posY = WorldToScreenY( Y );
	}

	void LineTo( HDC dc, double X, double Y )
	{
		::MoveToEx( dc, (int)posX, (int)posY, nullptr );
		MoveTo( X, Y );

		::LineTo( dc, (int)posX, (int)posY );
	}

	virtual void Axes( HDC dc )
	{
		MoveTo( L, 0.0 );
		LineTo( dc, R, 0.0 );

		MoveTo( 0.0, T );
		LineTo( dc, 0.0, B );
	}

	void StartDragging( int X, int Y )
	{
		prevMouseX = X;
		prevMouseY = Y;

		bDragging = true;
	}

	void Drag( int X, int Y )
	{
		L -= ( int ) ( ScreenToWorldX( X ) - ScreenToWorldX( prevMouseX ) );
		R -= ( int ) ( ScreenToWorldX( X ) - ScreenToWorldX( prevMouseX ) );

		B -= ( int ) ( ScreenToWorldY( Y ) - ScreenToWorldY( prevMouseY ) );
		T -= ( int ) ( ScreenToWorldY( Y ) - ScreenToWorldY( prevMouseY ) );

		prevMouseX = X;
		prevMouseY = Y;
	}

	bool IsDragging()
	{
		return bDragging;
	}

	void StopDragging()
	{
		bDragging = false;
	}

	void Scale( POINT MousePosition, float Koeff )
	{
		Koeff = Koeff > 0 ? 1.1f : 0.9f;

		double Xw = ScreenToWorldX( MousePosition.x );
		double Yw = ScreenToWorldY( MousePosition.y );

		L = Xw - ( Xw - L ) / Koeff;
		R = Xw - ( Xw - R ) / Koeff;
		B = Yw - ( Yw - B ) / Koeff;
		T = Yw - ( Yw - T ) / Koeff;
	}
};

#endif CAMERA_2D_H
