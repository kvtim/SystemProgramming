// lab8.cpp : Определяет точку входа для приложения.
//

#include "pch.h"
#include "framework.h"
#include "lab8.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HWND hList1, hList2;
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LPWSTR GetPriorityString(int);
void Reload();

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
    LoadStringW(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB8);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   hList1 = CreateWindowEx(WS_EX_CLIENTEDGE
       , L"LISTBOX", NULL
       , WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY
       , 10, 10, 300, 300
       , hWnd, (HMENU)1, hInstance, NULL);

   hList2 = CreateWindowEx(WS_EX_CLIENTEDGE
       , L"LISTBOX", NULL
       , WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL
       , 320, 10, 300, 300
       , hWnd, (HMENU)2, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   Reload();

   return TRUE;
}

LPWSTR GetPriorityString(int p)
{
    if (p == IDLE_PRIORITY_CLASS)
        return L"Idle";
    if (p == BELOW_NORMAL_PRIORITY_CLASS)
        return L"Below Normal";
    if (p == NORMAL_PRIORITY_CLASS)
        return L"Normal";
    if (p == ABOVE_NORMAL_PRIORITY_CLASS)
        return L"Above Normal";
    if (p == HIGH_PRIORITY_CLASS)
        return L"High";
    if (p == REALTIME_PRIORITY_CLASS)
        return L"Realtime";
    return L"Unknown";
}

void Reload() 
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32);

    Process32First(hProcessSnap, &pe32);

    SendMessage(hList1, LB_RESETCONTENT, 0, 0);

    do
    {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        dwPriorityClass = GetPriorityClass(hProcess);
        char text[1024];

        swprintf((wchar_t*)text, 100, L"[%i] %s (%s)", pe32.th32ProcessID, pe32.szExeFile, GetPriorityString(dwPriorityClass));
        int idx = SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)text);

        SendMessage(hList1, LB_SETITEMDATA, idx, (LPARAM)pe32.th32ProcessID);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
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
    case WM_CONTEXTMENU:
    {
        if (wParam == (LPARAM)hList1) {

            HMENU hPopupMenu = CreatePopupMenu();
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 10, L"Realtime");
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 11, L"High");
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 12, L"Above Normal");
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 13, L"Normal");
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 14, L"Below Normal");
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 15, L"Idle");
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 16, L"RELOAD");

            SetForegroundWindow(hWnd);

            POINT p;
            GetCursorPos(&p);
            TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hWnd, NULL);
        }
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            if (wmId >= 10 && wmId <= 15)
            {
                int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
                if (index == LB_ERR)
                    break;
                int pid = SendMessage(hList1, LB_GETITEMDATA, index, 0);

                int priority = 0;
                switch (wmId)
                {
                case 10:
                    priority = REALTIME_PRIORITY_CLASS;
                    break;
                case 11:
                    priority = HIGH_PRIORITY_CLASS;
                    break;
                case 12:
                    priority = ABOVE_NORMAL_PRIORITY_CLASS;
                    break;
                case 13:
                    priority = NORMAL_PRIORITY_CLASS;
                    break;
                case 14:
                    priority = BELOW_NORMAL_PRIORITY_CLASS;
                    break;
                case 15:
                    priority = IDLE_PRIORITY_CLASS;
                    break;
                }

                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

                if (!SetPriorityClass(hProcess, priority))
                    MessageBox(hWnd, L"Failed to set priority", L"", MB_ICONERROR);
                else
                    MessageBox(hWnd, L"Priority was succesfully setted!", L"", MB_ICONASTERISK);

                CloseHandle(hProcess);
                Reload();
            }

            // Разобрать выбор в меню:
            switch (wmId)
            {
            case 1:
            {
                if (wmEvent == NM_RCLICK) 
                {
                    int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
                }
                if (wmEvent != 0) 
                {
                    int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
                    if (index == LB_ERR)
                        break;
                    int pid = SendMessage(hList1, LB_GETITEMDATA, index, 0);

                    SendMessage(hList2, LB_RESETCONTENT, 0, 0);


                    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
                    MODULEENTRY32 me32;
                    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
                    me32.dwSize = sizeof(MODULEENTRY32);
                    Module32First(hModuleSnap, &me32);

                    do
                    {
                        int idx = SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)me32.szModule);

                        SendMessage(hList2, LB_SETITEMDATA, idx, (LPARAM)me32.szModule);

                    } while (Module32Next(hModuleSnap, &me32));

                    CloseHandle(hModuleSnap);
                }
                break;
            }
            case 16:
            {
                Reload();
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
