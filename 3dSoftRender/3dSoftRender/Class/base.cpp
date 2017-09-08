#define INITGUID
#define	WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <dinput.h>
#include "t3dlib1.h"
#include "t3dlib2.h"
#include "t3dlib3.h"

#define WINDOW_CLASS_NAME "WIND3DCLASS"
#define WINDOW_TITLE "T3D Graphics Console Ver 2.0"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define WINDOW_BPP 16
#define WINDOWED_APP 1

int Game_Init(void *parms = NULL);
int Game_Shutdown(void *parms = NULL);
int Game_Main(void *parms = NULL);

HWND main_window_handle = NULL;
HINSTANCE main_instance = NULL;
char buffer[256];


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_CREATE:
		{
			return 0;

		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
			return 0;
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	default:
		break;
	}
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASS winclass;
	HWND hwnd;
	MSG msg;
	HDC hdc;
	PAINTSTRUCT ps;

	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW |CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;

	if(!RegisterClass(&winclass))
		return 0;

	if(!(hwnd = CreateWindow(WINDOW_CLASS_NAME,
			WINDOW_TITLE,
			(WINDOWED_APP ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) :
				(WS_POPUP | WS_VISIBLE)),
			0, 0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			NULL, NULL,
			hinstance,
			NULL)))
			return 0;
	main_window_handle = hwnd;
	main_instance = hinstance;

	if (WINDOWED_APP)
	{
		RECT window_rect = {0, 0, WINDOW_WIDTH-1, WINDOW_HEIGHT-1};

		AdjustWindowRectEx(&window_rect,
			GetWindowStyle(main_window_handle),
			GetMenu(main_window_handle) != NULL,
			GetWindowExStyle(main_window_handle));

		window_client_x0 = -window_rect.left;
		window_client_y0 = -window_rect.top;

		MoveWindow(main_window_handle, 0, 0, window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top,
			FALSE);

		ShowWindow(main_window_handle, SW_SHOW);
	}
	Game_Init();

	SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, NULL, 0);

	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Game_Main();
	}
	Game_Shutdown();

	SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, NULL, 0);

	return (msg.wParam);
}

int Game_Init(void *parms)
{
	DDraw_Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOWED_APP);

	DInput_Init();

	DInput_Init_Keyboard();

	DSound_Init();
	DMusic_Init();
	ShowCursor(FALSE);
	srand(Start_Clock());

	return 1;
}

int Game_Shutdown(void *parms)
{
	DSound_Stop_All_Sounds();
	DSound_Delete_All_Sounds();
	DSound_Shutdown();

	DMusic_Delete_All_MIDI();
	DMusic_Shutdown();

	DInput_Release_Keyboard();
	DInput_Shutdown();

	DDraw_Shutdown();

	return 1;
}

int Game_Main(void *parms)
{
	int index;

	Start_Clock();

	DDraw_Fill_Surface(lpddsback, 0);

	DInput_Read_Keyboard();


	DDraw_Flip();
	Wait_Clock(30);

	if(KEY_DOWN(VK_ESCAPE) || keyboard_state[DIK_ESCAPE])
	{
		PostMessage(main_window_handle, WM_DESTROY, 0, 0);
	}
	return 1;
}