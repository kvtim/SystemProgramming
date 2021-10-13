// lab5Menu.cpp : Определяет точку входа для приложения.
//

#include "pch.h"
#include "framework.h"
#include "lab5Menu.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hRdColors[3];
HWND hRdShape[4];
HWND hChkDraw;

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
	LoadStringW(hInstance, IDC_LAB5MENU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB5MENU));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB5MENU));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB5MENU);
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

UINT  messageId;

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


	hRdColors[0] = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Red"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP
		, 0, 0, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);

	hRdColors[1] = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Green"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 0, 25, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);

	hRdColors[2] = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Blue"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 0, 50, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);


	hRdShape[0] = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Spade"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP
		, 0, 75, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);
	hRdShape[1] = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Square"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 0, 100, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);
	hRdShape[2] = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Star"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 0, 125, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);
	hRdShape[3] = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Circle"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 0, 150, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);

	hChkDraw = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", L"Draw"
		, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX
		, 0, 175, 200, 25
		, hWnd, (HMENU)1, hInstance, NULL);

	messageId = RegisterWindowMessage(L"Draw");

	MoveWindow(hWnd, 20, 20, 200, 270, FALSE);
	SendMessage(hRdColors[0], BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(hRdShape[0], BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(hChkDraw, BM_SETCHECK, BST_CHECKED, 0);

	PostMessage(hWnd, WM_COMMAND, 0, 0);
	return TRUE;
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

		// Send the settings
		UINT color = 0;
		if (SendMessage(hRdColors[0], BM_GETCHECK, 0, 0) == BST_CHECKED) color = 0;
		if (SendMessage(hRdColors[1], BM_GETCHECK, 0, 0) == BST_CHECKED) color = 1;
		if (SendMessage(hRdColors[2], BM_GETCHECK, 0, 0) == BST_CHECKED) color = 2;

		UINT shape = 0;
		if (SendMessage(hRdShape[0], BM_GETCHECK, 0, 0) == BST_CHECKED) shape = 0;
		if (SendMessage(hRdShape[1], BM_GETCHECK, 0, 0) == BST_CHECKED) shape = 1;
		if (SendMessage(hRdShape[2], BM_GETCHECK, 0, 0) == BST_CHECKED) shape = 2;
		if (SendMessage(hRdShape[3], BM_GETCHECK, 0, 0) == BST_CHECKED) shape = 3;

		UINT draw = SendMessage(hChkDraw, BM_GETCHECK, 0, 0) == BST_CHECKED ? 1 : 0;

		UINT settings = shape * 4 + color + 16 * draw;
		PostMessage(HWND_BROADCAST, messageId, settings, 0);

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
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
	}
	break;
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
