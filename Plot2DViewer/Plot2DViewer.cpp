// Вариант : 15
/*
Эллиптическая система координат
Явный способ задания функции
Масштабирование с сохранением положений верхней и нижней границ окна
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
LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создаётся окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon( nullptr, IDI_APPLICATION );
	wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH ) ( 6 );
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = ( LPCSTR )"MainWindowClass"; // имя оконного класса, используемое при создании экземпляров окна
	RegisterClass( &wc );					        // регистрация класса wc

	HWND hWnd = CreateWindow
	(												// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// имя оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		300,100,800,600,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr,nullptr,hInstance,nullptr
	);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while ( GetMessage( &msg, nullptr, 0, 0 ) )				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg ); 					// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// В основном модуле объявляется только одна глобальная переменная - создаётся объект класса Scene2D
// Все дальнейшие действия осуществляются посредством обращения к методам, реализованным в этом классе
Model2D Model( "vertices.txt", "edges.txt" );
Model3D SpaceModel( "vertices3d.txt", "faces.txt" );
Scene2D Scene( L, R, B, T, Model );
Scene3D SpaceScene( L, R, B, T, SpaceModel );

float TranslationSpeed	= 1.f;
float RotationSpeed		= 1.f;
float ScailingSpeed		= 1.f;
int PointToRotateAround = 4;

int FirstLinePoint		= 8;
int SecondLinePoint		= 9;

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
	switch( msg )
	{
		case WM_PAINT:
		{
			HDC dc = GetDC( hWnd );
			Scene.Clear( dc );				// Вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd
			//Scene.Plot( dc, Sinusoid );		// Вызов реализованного в классе Scene2D метода, отвечающего за отрисовку графика синусоиды
			//Scene.ParametricPlot( dc, F );
			//Scene.Plot( dc, Parabola );

			//Scene.Render( dc );

			SpaceScene.Render( dc );

			ReleaseDC( hWnd,dc );
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}

		case WM_SIZE:
		{
			HDC dc = GetDC( hWnd );
			Scene.SetResolution( dc );
			ReleaseDC( hWnd, dc );
			InvalidateRect(hWnd,nullptr,false);
			return 0;
		}
		
		case WM_LBUTTONDOWN :
		{
			Scene.StartDragging( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;
		}

		case WM_MOUSEMOVE :
		{
			if ( Scene.IsDragging() )
			{
				Scene.Drag( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );

				InvalidateRect( hWnd, NULL, false );
			}
			return 0;
		}

		case WM_LBUTTONUP :
		{
			Scene.StopDragging();
			return 0;
		}

		case WM_KEYDOWN :
		{
			switch ( wParam )
			{
				case 0x51: // Q turn left
				{
					Scene.GetModel()->Apply( Rotation( RotationSpeed * M_PI / 60.0 ) );
					break;
				}
				case 0x45 :
				{
					Scene.GetModel()->Apply( Rotation( RotationSpeed * ( -M_PI ) / 60.0 ) );
					break;
				}
				case 0x41 : // A move left
				{
					Scene.GetModel()->Apply( Translation( -TranslationSpeed, 0 ) );
					break;
				}
				case 0x44 : // D move right
				{
					Scene.GetModel()->Apply( Translation( TranslationSpeed, 0 ) );
					break;
				}
				case 0x57 : // W move up
				{
					Scene.GetModel()->Apply( Translation( 0, TranslationSpeed ) );
					break;
				}
				case 0x53 : // S move down
				{
					Scene.GetModel()->Apply( Translation( 0, -TranslationSpeed ) );
					break;
				}
				case 0x5A : // Z scale up
				{
					Scene.GetModel()->Apply( Scailing( ScailingSpeed * 1.1, ScailingSpeed * 1.1 ) );
					break;
				}
				case 0x58 : // X scale down
				{
					Scene.GetModel()->Apply( Scailing( ScailingSpeed * 0.9, ScailingSpeed * 0.9 ) );
					break;
				}
				case 0x46 : // F map X
				{
					Scene.GetModel()->Apply( Mapping( true ) );
					break;
				}
				case 0x47 : // G map Y
				{
					Scene.GetModel()->Apply( Mapping( false ) );
					break;
				}
				case 0x52 : // R scale up around vertex
				{
					float CoordX = Scene.GetModel()->GetVertexX( PointToRotateAround );
					float CoordY = Scene.GetModel()->GetVertexY( PointToRotateAround );

					Scene.GetModel()->Apply( ScaleAroundPoint( CoordX, CoordY, ScailingSpeed, true ) );

					break;
				}
				case 0x54 : // T scale down around vertex
				{
					float CoordX = Scene.GetModel()->GetVertexX( PointToRotateAround );
					float CoordY = Scene.GetModel()->GetVertexY( PointToRotateAround );
					
					Scene.GetModel()->Apply( ScaleAroundPoint( CoordX, CoordY, ScailingSpeed, false ) );

					break;
				}
				case 0x43 : // C to map around line
				{
					float FirstX = Scene.GetModel()->GetVertexX( FirstLinePoint );
					float FirstY = Scene.GetModel()->GetVertexY( FirstLinePoint );

					float SecondX = Scene.GetModel()->GetVertexX( SecondLinePoint );
					float SecondY = Scene.GetModel()->GetVertexY( SecondLinePoint );

					Scene.GetModel()->Apply( MapAroundLine( FirstX, FirstY, SecondX, SecondY ) );

					break;
				}
			}

			InvalidateRect( hWnd, NULL, false );
			return 0;
		}

		case WM_MOUSEWHEEL :
		{
			POINT MousePosition;

			MousePosition.x = GET_X_LPARAM( lParam );
			MousePosition.y = GET_Y_LPARAM( lParam );

			ScreenToClient( hWnd, &MousePosition );

			Scene.Scale( MousePosition, GET_WHEEL_DELTA_WPARAM( wParam ) );
			
			InvalidateRect( hWnd, NULL, false );

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
