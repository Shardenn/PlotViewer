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

LRESULT _stdcall WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );						// �������� ������� ���������
int _stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )		// �������� ���������
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
	wc.lpszMenuName = nullptr;							// ���� � ������� ������ �����������
	wc.lpszClassName = ( LPCSTR )"MainWindowClass"; // ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass( &wc );					        // ����������� ������ wc

	HWND hWnd = CreateWindow
	(												// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		( LPCSTR )"MainWindowClass",					// ��� �������� ������
	  ( LPCSTR )"Plot2D Viewer",					// ��������� ����
	  WS_OVERLAPPEDWINDOW,						// ����� ����
	  300, 100, 800, 600,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
	  nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while( GetMessage( &msg, nullptr, 0, 0 ) )				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg ); 					// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

float TranslationSpeed = 0.5f;
float RotationSpeed = 1.f;
float ScailingSpeed = 1.f;

int FirstLinePoint = 8;
int SecondLinePoint = 9;

double MouseCoordX, MouseCoordY;

Model3D SpaceModel( "vertices3d.txt", "faces.txt" );
set<Model3D> ModelsToCreate = { SpaceModel };
Scene3D SpaceScene( L, R, B, T, ModelsToCreate );

LRESULT _stdcall WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	//Model3D AxesModel( "AxesVertices.txt", "AxesFaces.txt" );
	switch( msg )
	{
		case WM_PAINT:
		{
			HDC dc = GetDC( hWnd );
			SpaceScene.Clear( dc );
			SpaceScene.RenderAll( dc );

			ReleaseDC( hWnd, dc );
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}

		case WM_SIZE:
		{
			HDC dc = GetDC( hWnd );
			SpaceScene.SetResolution( dc );
			ReleaseDC( hWnd, dc );
			InvalidateRect( hWnd, nullptr, false );
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			SpaceScene.GetModel()->StopAllActions();
		}

		case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case 0x51: // Q turn left
				{

					break;
				}
				case 0x45:
				{

					break;
				}
				case 0x41: // A move left
				{

					break;
				}
				case 0x44: // D move right
				{

					break;
				}
				case 0x57: // W move up
				{

					break;
				}
				case 0x53: // S move down
				{

					break;
				}
				case 0x5A: // Z scale up
				{

					break;
				}
				case 0x58: // X scale down
				{

					break;
				}
				case 0x46: // F map X
				{

					break;
				}
				case 0x47: // G start dragging
				{
					MouseCoordX = GET_X_LPARAM( lParam );
					MouseCoordY = GET_Y_LPARAM( lParam );

					SpaceScene.GetModel()->SetDragged( true );

					break;
				}
				case 0x52: // R scale up around vertex
				{

					break;
				}
				case 0x54: // T scale down around vertex
				{

					break;
				}
				case 0x43: // C to map around line
				{

					break;
				}
			}

			InvalidateRect( hWnd, nullptr, false );
			return 0;
		}

		case WM_MOUSEWHEEL: // Wheel rotate
		{
			SpaceScene.ZoomScene( GET_WHEEL_DELTA_WPARAM( wParam ) );
	
			InvalidateRect( hWnd, nullptr, false );

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			double dx = GET_X_LPARAM( lParam ) - MouseCoordX;
			double dy = GET_Y_LPARAM( lParam ) - MouseCoordY;

			if( SpaceScene.GetModel()->isDragged() )
			{
				Vector3D MoveVector( dx, dy, 0 );
				MoveVector.Normalize();
				MoveVector.Y = -MoveVector.Y;
				MoveVector = MoveVector * TranslationSpeed;

				SpaceScene.GetModel()->Apply( Translation( MoveVector ) );
			}
			if( SpaceScene.IsRotating() ) // Mouse wheel is pushed => rotate scene
			{
				SpaceScene.RotateScene( dx, dy );
			}
			else if( SpaceScene.IsMoving() )
			{
				SpaceScene.MoveScene( dx, dy );
			}

			MouseCoordX = GET_X_LPARAM( lParam );
			MouseCoordY = GET_Y_LPARAM( lParam );

			InvalidateRect( hWnd, nullptr, false );

			return 0;
		}

		case WM_MBUTTONDOWN: // Wheel push
		{
			MouseCoordX = GET_X_LPARAM( lParam );
			MouseCoordY = GET_Y_LPARAM( lParam );

			if( GetAsyncKeyState( VK_SHIFT ) )
			{
				SpaceScene.SetIsMoving( true );
			}
			else
			{
				SpaceScene.SetIsRotating( true );
			}

			return 0;
		}

		case WM_MBUTTONUP:
		{
			SpaceScene.SetIsRotating( false );
			SpaceScene.SetIsMoving( false );

			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
	}
	return 0;
}
