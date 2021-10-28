// SP_lab7.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_lab7.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SPLAB7, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB7));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB7));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPLAB7);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
HANDLE hHandle, hHandleEvent;
PAINTSTRUCT ps;
HDC hdc;

void f1(HWND hWnd)
{
	WaitForSingleObject(hHandleEvent, INFINITE);
	WaitForSingleObject(hHandle, INFINITE);

	POINT poly[8];
	poly[0] = { 100, 100 };
	poly[1] = { 100, 50 };
	poly[2] = { 50, 50 };
	poly[3] = { 50, 100 };
	poly[4] = { 100, 100 };
	poly[5] = { 100, 50 };
	poly[6] = { 75, 30 };
	poly[7] = { 50, 50 };
	Polyline(hdc, poly, 8);

	Sleep(500);

	ReleaseMutex(hHandle);
}

void f2(HWND hWnd)
{
	WaitForSingleObject(hHandleEvent, INFINITE);
	WaitForSingleObject(hHandle, INFINITE);

	Ellipse(hdc, 200 - 10 - 25, 50 - 25, 200 - 10 + 25, 50 + 25);
	MoveToEx(hdc, 200 - 10, 75, (LPPOINT)NULL);
	LineTo(hdc, 200 - 10, 120);

	Sleep(500);

	ReleaseMutex(hHandle);
}

void f3(HWND hWnd)
{
	WaitForSingleObject(hHandleEvent, INFINITE);
	WaitForSingleObject(hHandle, INFINITE);

	POINT poly2[5];
	poly2[0] = { 300, 100 };
	poly2[1] = { 275, 75 };
	poly2[2] = { 300, 50 };
	poly2[3] = { 325, 75 };
	poly2[4] = { 300, 100 };
	Polyline(hdc, poly2, 5);

	Sleep(500);

	ReleaseMutex(hHandle);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThreads[3];

	switch (message)
	{
	case WM_CREATE:
	{
		hHandle = CreateMutex(NULL, FALSE, L"Mutex");
		hHandleEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("Event"));

		hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f1, (LPVOID)hWnd, NULL, NULL);
		hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f2, (LPVOID)hWnd, NULL, NULL);
		hThreads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f3, (LPVOID)hWnd, NULL, NULL);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		SetEvent(hHandleEvent);
		break;
	}
	case WM_DESTROY:
		WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);
		CloseHandle(hHandle);
		CloseHandle(hHandleEvent);
		EndPaint(hWnd, &ps);
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
