// lab2.cpp : Определяет точку входа для приложения.
//

#include "pch.h"
#include "framework.h"
#include "lab2.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HWND hAddButton, hTextbox, hDeleteButton, hMoveButton, hClearButton, hList1, hList2;
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
	LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB2);
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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	hList1 = CreateWindowEx(WS_EX_CLIENTEDGE
		, L"LISTBOX", NULL
		, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_MULTIPLESEL
		, 10, 10, 300, 300
		, hWnd, NULL, hInstance, NULL);

	hList2 = CreateWindowEx(WS_EX_CLIENTEDGE
		, L"LISTBOX", NULL
		, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_MULTIPLESEL
		, 320, 10, 300, 300
		, hWnd, NULL, hInstance, NULL);

	hAddButton = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", NULL
		, WS_CHILD | WS_VISIBLE
		, 10, 320, 100, 40
		, hWnd, (HMENU)1, hInstance, NULL);
	SetWindowText(hAddButton, L"Add");

	hTextbox = CreateWindowEx(WS_EX_CLIENTEDGE
		, L"EDIT", NULL
		, WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL
		, 120, 320, 100, 40
		, hWnd, (HMENU)2, hInstance, NULL);

	hDeleteButton = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", NULL
		, WS_CHILD | WS_VISIBLE
		, 240, 320, 100, 40
		, hWnd, (HMENU)3, hInstance, NULL);
	SetWindowText(hDeleteButton, L"Delete");

	hClearButton = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", NULL
		, WS_CHILD | WS_VISIBLE
		, 360, 320, 100, 40
		, hWnd, (HMENU)4, hInstance, NULL);
	SetWindowText(hClearButton, L"Clear");

	hMoveButton = CreateWindowEx(WS_EX_APPWINDOW
		, L"BUTTON", NULL
		, WS_CHILD | WS_VISIBLE
		, 480, 320, 100, 40
		, hWnd, (HMENU)5, hInstance, NULL);
	SetWindowText(hMoveButton, L"To Right");

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void addToList1()
{
	char* text = new char[256];

	GetWindowTextA(hTextbox, (LPSTR)text, 256);
	SetWindowText(hTextbox, (LPWSTR)"");

	while (text[strlen(text) - 1] == 32)
	{
		text[strlen(text) - 1] = 0;
	}
	while (*text == 32)
		text++;

	bool match = FALSE;
	int count = SendMessageA(hList1, LB_GETCOUNT, 0, 0);

	for (int i = 0; i < count; i++) 
	{
		char* data = (char*)SendMessageA(hList1, LB_GETITEMDATA, (WPARAM)i, 0);
		if (strcmp(data, text) == 0)
			match = TRUE;
	}

	if (!match && strlen(text) > 0) 
	{
		int idx = SendMessageA(hList1, LB_ADDSTRING, 0, (LPARAM)text);
		SendMessageA(hList1, LB_SETITEMDATA, idx, (LPARAM)text);
	}
}

void MoveToList2()
{
	int count = SendMessageA(hList1, LB_GETSELCOUNT, 0, 0);
	int* buf = new int[count];

	SendMessageA(hList1, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);

	for (int i = 0; i < count; i++) 
	{
		int data = SendMessageA(hList1, LB_GETITEMDATA, (WPARAM)buf[i], 0);
		char* s = _strdup((char*)data);

		bool match = FALSE;
		int count = SendMessageA(hList2, LB_GETCOUNT, 0, 0);

		for (int i = 0; i < count; i++) 
		{
			char* data = (char*)SendMessageA(hList2, LB_GETITEMDATA, (WPARAM)i, 0);
			if (strcmp(data, s) == 0)
				match = TRUE;
		}

		if (!match && strlen(s) > 0) {
			int idx = SendMessageA(hList2, LB_ADDSTRING, 0, (LPARAM)s);
			SendMessageA(hList2, LB_SETITEMDATA, idx, (LPARAM)s);
		}
		break;
	}
	delete[] buf;
}

void DeleteString()
{
	int count = SendMessageA(hList1, LB_GETSELCOUNT, 0, 0);
	int* buf = new int[count];

	SendMessageA(hList1, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);

	for (int i = count - 1; i >= 0; i--) 
	{
		SendMessageA(hList1, LB_DELETESTRING, buf[i], 0);
	}
	delete[] buf;

	count = SendMessageA(hList2, LB_GETSELCOUNT, 0, 0);
	buf = new int[count];

	SendMessageA(hList2, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);

	for (int i = count - 1; i >= 0; i--) 
	{
		SendMessageA(hList2, LB_DELETESTRING, buf[i], 0);
	}
	delete[] buf;
}

void ClearLists()
{
	int count = SendMessageA(hList1, LB_GETCOUNT, 0, 0);
	for (int i = count - 1; i >= 0; i--)
		SendMessageA(hList1, LB_DELETESTRING, 0, 0);

	count = SendMessageA(hList2, LB_GETCOUNT, 0, 0);
	for (int i = count - 1; i >= 0; i--)
		SendMessageA(hList2, LB_DELETESTRING, 0, 0);
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
		case 1: {
			addToList1();
			break;
		}
		case 3: {
			DeleteString();
			break;
		}
		case 4: {
			ClearLists();
			break;
		}
		case 5: {
			MoveToList2();
			break;
		}
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
