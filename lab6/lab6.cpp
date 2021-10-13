// lab6.cpp : Определяет точку входа для приложения.
//

#include "pch.h"
#include "framework.h"
#include "lab6.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

LPWSTR letters[5];
UINT letterX[5];
INT letterY[5];
UINT letterSpeed[5];
HANDLE threads[5];

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
    LoadStringW(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB6));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

bool running = TRUE;


DWORD WINAPI ThreadProc(LPVOID param)
{
    int idx = (int)param;
    while (1) 
    {
        Sleep(10);
        letterY[idx] += letterSpeed[idx];
        if (letterY[idx] > 500)
            letterY[idx] = -30;
    }
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

   if (!hWnd)
   {
      return FALSE;
   }

   MoveWindow(hWnd, 100, 100, 640, 480, FALSE);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   letters[0] = (LPWSTR)"A";
   letters[1] = (LPWSTR)"B";
   letters[2] = (LPWSTR)"C";
   letters[3] = (LPWSTR)"D";
   letters[4] = (LPWSTR)"E";
   letterSpeed[0] = 1;
   letterSpeed[1] = 2;
   letterSpeed[2] = 3;
   letterSpeed[3] = 4;
   letterSpeed[4] = 5;
   letterX[0] = 220;
   letterX[1] = 240;
   letterX[2] = 260;
   letterX[3] = 280;
   letterX[4] = 320;
   letterY[0] = 0;
   letterY[1] = 0;
   letterY[2] = 0;
   letterY[3] = 0;
   letterY[4] = 0;

   for (int i = 0; i < 5; i++)
       threads[i] = CreateThread(NULL, 2000, &ThreadProc, (LPVOID)i, 0, NULL);

   SetTimer(hWnd, 1, 3, NULL);

   return TRUE;
}

void DoStart(HWND hWnd) 
{
    if (!running)
    {
        for (int i = 0; i < 5; i++)
            ResumeThread(threads[i]);
    }
    running = TRUE;
}

void DoStop(HWND hWnd) 
{
    if (running) 
    {
        for (int i = 0; i < 5; i++)
            SuspendThread(threads[i]);
    }
    running = FALSE;
}

void DoTimer(HWND hWnd) 
{
    InvalidateRect(hWnd, NULL, TRUE);
}

void DoPaint(HDC hdc) 
{
    RECT rect;
    SetTextColor(hdc, 0x00000000);
    SetBkMode(hdc, TRANSPARENT);

    for (int i = 0; i < 5; i++) 
    {
        rect.left = letterX[i];
        rect.top = letterY[i];
        rect.right = letterX[i] + 100;
        rect.bottom = letterY[i] + 100;
        DrawText(hdc, letters[i], -1, &rect, DT_SINGLELINE | DT_NOCLIP);
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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case 1:
                DoStart(hWnd);
                break;
            case 2:
                DoStop(hWnd);
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
    case WM_TIMER:
        DoTimer(hWnd);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DoPaint(hdc);
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
