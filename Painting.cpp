// Painting.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Painting.h"

#define MAX_LOADSTRING 100

#define CM_TOOLS_PENCIL 1001
#define CM_TOOLS_LINE 1002

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int prevX, prevY, pointX, pointY;
bool isClicked, isPencil, isLine;
HDC tempHdc;
HBITMAP hBitmap;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAINTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINTING));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINTING));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAINTING);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

void czCreateMenu(HWND hwnd)
{
            HMENU MainMenu = CreateMenu();
            HMENU hPopupMenu = CreatePopupMenu();
 
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"&Инструменты"); 
            {
                AppendMenu(hPopupMenu, MF_STRING, CM_TOOLS_PENCIL, L"Карандаш");
                AppendMenu(hPopupMenu, MF_STRING, CM_TOOLS_LINE, L"Прямая");
                AppendMenu(hPopupMenu, MF_STRING, 1000, L"Сохранить как...");
                AppendMenu(hPopupMenu, MF_STRING, 1000, L"Авторизация");
                AppendMenu(hPopupMenu, MF_STRING | MF_CHECKED, 1000, L"Печать");
                AppendMenu(hPopupMenu, MF_STRING, 1000, L"Выход");
            }
 
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Правка");  
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Окна");    
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Настройки");
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&?");   
 
            
            
            SetMenu(hwnd, MainMenu);
			//------------
			RECT rc;
			GetClientRect(hwnd, &rc);

			HDC hdc = GetDC(hwnd);

			tempHdc = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(tempHdc, rc.right - rc.left, rc.bottom);
			SelectObject(tempHdc, hBitmap);

			InvertRect(tempHdc, &rc);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   czCreateMenu(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	int x, y;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case CM_TOOLS_PENCIL:
			isPencil = true;
			isLine = false;
			break;
		case CM_TOOLS_LINE:
			isLine = true;
			isPencil = false;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (isPencil && isClicked)
		{
			MoveToEx(hdc, prevX, prevY, 0);
			LineTo(hdc, x, y);
			prevX = x;
			prevY = y;
		}
		if (isLine && isClicked)
		{
		    RECT rc;
			GetClientRect(hWnd, &rc);
			
			BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom, tempHdc, rc.left, rc.top, SRCCOPY);			

			MoveToEx(hdc, pointX, pointY, 0);	
			LineTo(hdc, x, y);

		}
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		isClicked = true;
		if (isPencil)
		{
			prevX = LOWORD(lParam);
			prevY = HIWORD(lParam);
		}
		if (isLine)
		{
			pointX = LOWORD(lParam);
			pointY = HIWORD(lParam);
			RECT rc;
			GetClientRect(hWnd, &rc);
			/*tempHdc = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(tempHdc, rc.right - rc.left, rc.bottom);
			hDefaultBitmap = (HBITMAP) SelectObject(tempHdc, hBitmap);

			InvertRect(tempHdc, &rc);*/
			
		}		
		break;
	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		isClicked = false;
		RECT rc;
	    GetClientRect(hWnd, &rc);
		BitBlt(tempHdc, rc.left, rc.top, rc.right - rc.left, rc.bottom, hdc, rc.left, rc.top, SRCCOPY);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


