// lab4.cpp : Определяет точку входа для приложения.
//

#include "pch.h"
#include "framework.h"
#include "lab4.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HBITMAP hImages[4];

WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
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

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB4));

	MSG msg;

	// Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	hImages[0] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	hImages[1] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
	hImages[2] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
	hImages[3] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	MoveWindow(hWnd, 100, 100, 640, 480, FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetTimer(hWnd, 1, 50, NULL);
	return TRUE;
}

HGDIOBJ brushWhite = CreateSolidBrush(RGB(255, 255, 255));
int imageIdx = 0;

void PaintImage(HWND hWnd, HBITMAP image, HDC hDc, int x, int y)
{
	HDC hCDC = CreateCompatibleDC(hDc);
	SelectObject(hCDC, image);
	StretchBlt(hDc, x, y, 64, 64, hCDC, 0, 0, 200, 200, SRCCOPY);
	DeleteDC(hCDC);
}

int x = 200, y = 100, xt = x, yt = y;

void DoPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(hWnd, &ps);
	RECT rect;
	GetClientRect(hWnd, &rect);
	HDC hCDC = CreateCompatibleDC(hDc);
	HBITMAP buffer = CreateCompatibleBitmap(hDc, 640, 480);

	SelectObject(hCDC, buffer);
	SelectObject(hCDC, brushWhite);


	Rectangle(hCDC, -1, -1, 640, 480);


	PaintImage(hWnd, hImages[imageIdx], hCDC, x - 32, y - 32);

	BitBlt(hDc, 0, 0, 640, 480, hCDC, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);
	DeleteDC(hCDC);
	DeleteObject(buffer);
}

void DoTimer(HWND hWnd)
{
	int newIdx = imageIdx;
	bool moving = FALSE;

	if (x != xt)
	{
		x -= (x - xt) / abs(x - xt) * 10;
		moving = TRUE;
	}

	if (y != yt)
	{
		y -= (y - yt) / abs(y - yt) * 10;
		moving = TRUE;
	}

	while (newIdx == imageIdx && moving)
		newIdx = rand() % 4;

	imageIdx = newIdx;
	InvalidateRect(hWnd, NULL, FALSE);
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
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
		DoPaint(hWnd);
		break;
	case WM_TIMER:
		DoTimer(hWnd);
		break;
	case WM_LBUTTONUP:
	{
		xt = GET_X_LPARAM(lParam) / 10 * 10;
		yt = GET_Y_LPARAM(lParam) / 10 * 10;
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
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
