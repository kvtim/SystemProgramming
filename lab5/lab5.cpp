// lab5.cpp : Определяет точку входа для приложения.
//

#include "pch.h"
#include "framework.h"
#include "lab5.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
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
	LoadStringW(hInstance, IDC_LAB5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB5));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB5));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB5);
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

UINT messageId;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	messageId = RegisterWindowMessage(L"Draw");

	return TRUE;
}

HBRUSH bRed = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH bGreen = CreateSolidBrush(RGB(0, 255, 0));
HBRUSH bBlue = CreateSolidBrush(RGB(0, 0, 255));



UINT shapeCount = 0;

UINT shapeX[1024];
UINT shapeY[1024];
UINT shapeColor[1024];
UINT shapeType[1024];

UINT settings;
void DoPaint(HDC hdc)
{

	for (int i = 0; i < shapeCount; i++)
	{
		UINT color = shapeColor[i];
		UINT shape = shapeType[i];
		UINT mouseX = shapeX[i];
		UINT mouseY = shapeY[i];

		SelectObject(hdc, (color == 0) ? bRed : ((color == 1) ? bGreen : bBlue));

		if (shape == 0)
		{
			POINT points[4];
			points[0].x = mouseX;
			points[0].y = mouseY - 10;
			points[1].x = mouseX + 10;
			points[1].y = mouseY;
			points[2].x = mouseX;
			points[2].y = mouseY + 10;
			points[3].x = mouseX - 10;
			points[3].y = mouseY;
			Polygon(hdc, points, 4);
		}

		if (shape == 1)
		{
			Rectangle(hdc, mouseX - 10, mouseY - 10, mouseX + 10, mouseY + 10);
		}

		if (shape == 2)
		{
			POINT points[10];
			points[0].x = mouseX;
			points[0].y = mouseY - 10;

			points[1].x = mouseX + 3;
			points[1].y = mouseY - 3;

			points[2].x = mouseX + 10;
			points[2].y = mouseY - 3;

			points[3].x = mouseX + 3;
			points[3].y = mouseY;

			points[4].x = mouseX + 5;
			points[4].y = mouseY + 10;

			points[5].x = mouseX;
			points[5].y = mouseY + 3;

			points[6].x = mouseX - 5;
			points[6].y = mouseY + 10;

			points[7].x = mouseX - 3;
			points[7].y = mouseY;

			points[8].x = mouseX - 10;
			points[8].y = mouseY - 3;

			points[9].x = mouseX - 3;
			points[9].y = mouseY - 3;

			Polygon(hdc, points, 10);
		}

		if (shape == 3)
		{
			Ellipse(hdc, mouseX - 10, mouseY - 10, mouseX + 10, mouseY + 10);
		}
	}
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
	if (message == messageId)
	{
		settings = wParam;
	}

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
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		DoPaint(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (settings & 16) {
			shapeX[shapeCount] = GET_X_LPARAM(lParam);
			shapeY[shapeCount] = GET_Y_LPARAM(lParam);
			shapeType[shapeCount] = (settings >> 2) & 3;
			shapeColor[shapeCount] = settings & 3;
			shapeCount++;
		}
		InvalidateRect(hWnd, NULL, TRUE);
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
