// ������� : 15
/*
������������� ������� ���������
����� ������ ������� �������
��������������� � ����������� ��������� ������� � ������ ������ ����
*/
#pragma once

#include <windows.h>
#include <windowsx.h>
#include "Scene2D.h"
#include "Scene3D.h"
#include "Matrix.h"
#include "AffineTransform.h"
#include "Model2D.h"
#include "Model3D.h"
LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// �������� ���������
{
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� �������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon( nullptr, IDI_APPLICATION );
	wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH ) ( 6 );
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = ( LPCSTR )"MainWindowClass"; // ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass( &wc );					        // ����������� ������ wc

	HWND hWnd = CreateWindow
	(												// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"MainWindowClass",					// ��� �������� ������
		(LPCSTR)"Plot2D Viewer",					// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		300,100,800,600,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr
	);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while ( GetMessage( &msg, nullptr, 0, 0 ) )				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg ); 					// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

Model3D SpaceModel( "vertices3d.txt", "faces.txt" );
Scene3D SpaceScene( L, R, B, T, SpaceModel );

float TranslationSpeed	= 0.1f;
float RotationSpeed		= 1.f;
float ScailingSpeed		= 1.f;
int PointToRotateAround = 4;

int FirstLinePoint		= 8;
int SecondLinePoint		= 9;

double MouseCoordX, MouseCoordY;

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_PAINT:
		{
			HDC dc = GetDC( hWnd );
			SpaceScene.Clear( dc );
			SpaceScene.Render( dc, false );

			ReleaseDC( hWnd,dc );
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}

		case WM_SIZE:
		{
			HDC dc = GetDC( hWnd );
			SpaceScene.SetResolution( dc );
			ReleaseDC( hWnd, dc );
			InvalidateRect(hWnd,nullptr,false);
			return 0;
		}
		
		case WM_LBUTTONDOWN :
		{
			SpaceScene.GetModel()->StopAllActions();
		}

		case WM_KEYDOWN :
		{
			switch ( wParam )
			{
				case 0x51: // Q turn left
				{
					//Scene.GetModel()->Apply( Rotation( RotationSpeed * M_PI / 60.0 ) );
					break;
				}
				case 0x45 :
				{
					//Scene.GetModel()->Apply( Rotation( RotationSpeed * ( -M_PI ) / 60.0 ) );
					break;
				}
				case 0x41 : // A move left
				{
					//Scene.GetModel()->Apply( Translation( -TranslationSpeed, 0 ) );
					break;
				}
				case 0x44 : // D move right
				{
					//Scene.GetModel()->Apply( Translation( TranslationSpeed, 0 ) );
					break;
				}
				case 0x57 : // W move up
				{
					//Scene.GetModel()->Apply( Translation( 0, TranslationSpeed ) );
					break;
				}
				case 0x53 : // S move down
				{
					//Scene.GetModel()->Apply( Translation( 0, -TranslationSpeed ) );
					break;
				}
				case 0x5A : // Z scale up
				{
					//Scene.GetModel()->Apply( Scailing( ScailingSpeed * 1.1, ScailingSpeed * 1.1 ) );
					break;
				}
				case 0x58 : // X scale down
				{
					//Scene.GetModel()->Apply( Scailing( ScailingSpeed * 0.9, ScailingSpeed * 0.9 ) );
					break;
				}
				case 0x46 : // F map X
				{
					//Scene.GetModel()->Apply( Mapping( true ) );
					break;
				}
				case 0x47 : // G start dragging
				{
					MouseCoordX = GET_X_LPARAM( lParam );
					MouseCoordY = GET_Y_LPARAM( lParam );

					SpaceScene.GetModel()->SetDragged( true );

					break;
				}
				case 0x52 : // R scale up around vertex
				{
					//float CoordX = Scene.GetModel()->GetVertexX( PointToRotateAround );
					//float CoordY = Scene.GetModel()->GetVertexY( PointToRotateAround );

					//Scene.GetModel()->Apply( ScaleAroundPoint( CoordX, CoordY, ScailingSpeed, true ) );

					break;
				}
				case 0x54 : // T scale down around vertex
				{
					//float CoordX = Scene.GetModel()->GetVertexX( PointToRotateAround );
					//float CoordY = Scene.GetModel()->GetVertexY( PointToRotateAround );
					
					//Scene.GetModel()->Apply( ScaleAroundPoint( CoordX, CoordY, ScailingSpeed, false ) );

					break;
				}
				case 0x43 : // C to map around line
				{
					//float FirstX = Scene.GetModel()->GetVertexX( FirstLinePoint );
					//float FirstY = Scene.GetModel()->GetVertexY( FirstLinePoint );

					//float SecondX = Scene.GetModel()->GetVertexX( SecondLinePoint );
					//float SecondY = Scene.GetModel()->GetVertexY( SecondLinePoint );

					//Scene.GetModel()->Apply( MapAroundLine( FirstX, FirstY, SecondX, SecondY ) );

					break;
				}
			}

			InvalidateRect( hWnd, nullptr, false );
			return 0;
		}

		case WM_MOUSEWHEEL :
		{
			POINT MousePosition;

			MousePosition.x = GET_X_LPARAM( lParam );
			MousePosition.y = GET_Y_LPARAM( lParam );

			ScreenToClient( hWnd, &MousePosition );

			//Scene.Scale( MousePosition, GET_WHEEL_DELTA_WPARAM( wParam ) );
			
			InvalidateRect( hWnd, nullptr, false );

			return 0;
		}

		case WM_MOUSEMOVE :
		{
			if( SpaceScene.GetModel()->isDragged() )
			{
				double dx = GET_X_LPARAM( lParam ) - MouseCoordX;
				double dy = GET_Y_LPARAM( lParam ) - MouseCoordY;

				Vector3D MoveVector( dx, dy, 0 );
				MoveVector.Normalize();
				MoveVector.Y = -MoveVector.Y;
				MoveVector = MoveVector * TranslationSpeed;

				SpaceScene.GetModel()->Apply( Translation( MoveVector ) );

				

				MouseCoordX = GET_X_LPARAM( lParam );
				MouseCoordY = GET_Y_LPARAM( lParam );
				
				InvalidateRect( hWnd, nullptr, false );
			}
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}
	}
	return 0;
}
