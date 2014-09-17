// AStarPath.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AStarPath.h"
#include "Map.h"
#include "GameObject.h"
#include "PathFinder.h"
#include "PerformaceTimer.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HWND g_mainWnd;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int cxClient, cyClient;
Map g_map;
GameObject g_object;
int g_timerID;

void CALLBACK Update(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	int ret = g_object.UpdateObject( 0 );

	InvalidateRect(g_mainWnd, NULL, TRUE);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ASTARPATH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTARPATH));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTARPATH));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ASTARPATH);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

int InitGame()
{
	char strFilePath[MAX_PATH];
	char* strLastSlash;
	// Get the exe exe path
	GetModuleFileName( NULL, strFilePath, MAX_PATH );
	strFilePath[MAX_PATH-1]=0;

	strLastSlash = _tcsrchr( strFilePath, TEXT('\\') );
	if( strLastSlash )
	{
		// Chop the exe name from the exe path
		*strLastSlash = 0;
	}
	strcpy_s( strLastSlash, 7, "\\2.map" );
	g_map.InitMap( strFilePath );

	g_object.InitObject();

	g_timerID = SetTimer( NULL, 1, 25, Update );

	return 0;
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WINDOW_WIDTH+16, WINDOW_HEIGHT+58, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_mainWnd = hWnd;
   InitGame( );

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	static HPEN BluePen = CreatePen( PS_SOLID, 1, RGB(0, 0, 255) );
	static HPEN OldPen = NULL;

	static HBRUSH RedBrush = CreateSolidBrush( RGB(255, 0, 0) );
	static HBRUSH OldBrush = NULL;

	//static POINT verts[NUM_VERTS];
	//static int iNumVerts = NUM_VERTS;

	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;

	static unsigned int nFlag = 1;

	static HINSTANCE hInstance;

	static int timerID;

	switch (message)
	{
	case WM_CREATE:{
		RECT rect;
		GetClientRect( hWnd, &rect );
		cxClient = rect.right;
		cyClient = rect.bottom;

		//Create the back buffer
		hdcBackBuffer = CreateCompatibleDC( NULL );
		hdc = GetDC( hWnd );
		hBitmap = CreateCompatibleBitmap( hdc, cxClient, cyClient );
		hOldBitmap = ( HBITMAP )SelectObject( hdcBackBuffer, hBitmap );

		hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;  

		ReleaseDC( hWnd, hdc ); 
	}break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
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
		break;
	case WM_PAINT:{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);

		g_map.Draw( hdcBackBuffer );
		g_object.DrawObject( hdcBackBuffer );

		BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		}break;
	case WM_SIZE:{
		cxClient = LOWORD( lParam );
		cyClient = HIWORD( lParam );

		//Create a new bitmap
		SelectObject( hdcBackBuffer, hOldBitmap );
		DeleteObject( hBitmap );

		HDC hdc = GetDC( hWnd );
		hBitmap = CreateCompatibleBitmap( hdc, cxClient, cyClient );
		ReleaseDC( hWnd, hdc );

		SelectObject( hdcBackBuffer, hBitmap );
	}break;
	case WM_ERASEBKGND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDBLCLK:
		g_object.SetDestination( LOWORD(lParam) / GRID_WIDTH, HIWORD(lParam) / GRID_HEIGHT );
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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
