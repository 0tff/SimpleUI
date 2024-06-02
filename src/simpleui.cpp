// This project is by  0tff and toasty! Good luck 
// Have a look into the wiki!

#define _CRT_SECURE_NO_WARNINGS 
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

using namespace std;

void init();
void ShowWindowForm();

void Print(HWND hWndEdit, std::string pszText)
{
    int nLength = GetWindowTextLength(hWndEdit);
    SendMessage(hWndEdit, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
    SendMessage(hWndEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)pszText.c_str());
}

#define INPUT_CHAR_LIMIT            500

#define ALX_CONSOLE_WINDOW     (WM_APP + 500)
#define ALX_INPUT_FIELD        (WM_APP + 501)

#define MALX_EXIT              (WM_APP + 600)
#define MALX_RESTART           (WM_APP + 601)
#define MALX_ABOUT             (WM_APP + 602)
#define MALX_CREDITS           (WM_APP + 603)
#define MALX_COMMANDS          (WM_APP + 604)

HWND ParentWindow = NULL;
HWND MainWindow = NULL;
HMENU WindowMenu = NULL;
HMODULE HInstance = NULL;

HWND InputField = NULL;
HWND txtbox = NULL;

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CTLCOLORSTATIC:
    {
        HDC hEdit = (HDC)wParam;
        SetTextColor(hEdit, RGB(255, 255, 255));
        SetBkColor(hEdit, RGB(34, 34, 34)); // Dark background color
        return (LRESULT)GetStockObject(BLACK_BRUSH);
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case MALX_EXIT:
            if (MessageBox(hwnd, "Are you sure you want to exit?", "Exit", MB_YESNOCANCEL) == IDYES)
                ExitThread(0);
            break;
        }
        break;
    case WM_DESTROY:
        ExitThread(0);
        break;
    case WM_QUIT:
        ExitThread(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

BOOL RegisterWindowClass(const char* wClassName) {
    WNDCLASSEX nClass;

    nClass.cbSize = sizeof(WNDCLASSEX);
    nClass.style = CS_DBLCLKS;
    nClass.lpfnWndProc = DLLWindowProc;
    nClass.cbClsExtra = 0;
    nClass.cbWndExtra = 0;
    nClass.hInstance = GetModuleHandle(NULL);
    nClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
    nClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    nClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    nClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Set background to black
    nClass.lpszMenuName = "what";
    nClass.lpszClassName = wClassName;

    if (!RegisterClassEx(&nClass))
        return 0;

    return 1;
}

BOOL StartMessageLoop() {
    MSG msg;
    BOOL bRet;

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == 0) {
            return 0;
        }
        else if (bRet == -1)
        {
            return 0;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

BOOL CreateSubwindows() {
    txtbox = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 1, 20, 450, 234, MainWindow, (HMENU)ALX_CONSOLE_WINDOW, HInstance, 0);
    InputField = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 1, 284, 420, 100, MainWindow, (HMENU)ALX_INPUT_FIELD, HInstance, 0);
    HWND execute = CreateWindowEx(NULL, "button", "Execute", WS_CHILD | WS_VISIBLE | WS_BORDER, 1, 253, 100, 32, MainWindow, NULL, NULL, NULL);
    HWND openfile = CreateWindowEx(NULL, "button", "Open file", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 253, 220, 32, MainWindow, NULL, NULL, NULL);
    HWND clear = CreateWindowEx(NULL, "button", "Clear", WS_CHILD | WS_VISIBLE | WS_BORDER, 320, 253, 100, 32, MainWindow, NULL, NULL, NULL);

    SendMessage(InputField, EM_SETLIMITTEXT, INPUT_CHAR_LIMIT, NULL);

    HFONT textFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

    SendMessage(txtbox, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
    SendMessage(InputField, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));

    UpdateWindow(MainWindow);

    return 1;
}

BOOL CreateWindowMenu() {
    WindowMenu = CreateMenu();
    if (!WindowMenu)
        return 0;

    HMENU mainDropdown = CreatePopupMenu();
    AppendMenu(mainDropdown, MF_STRING, MALX_EXIT, "Exit");
    AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)mainDropdown, "Sevenv2");

    HMENU aboutDropdown = CreatePopupMenu();
    AppendMenu(aboutDropdown, MF_STRING, MALX_CREDITS, "Credits");
    AppendMenu(aboutDropdown, MF_STRING, MALX_ABOUT, "Large script window");
    AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)aboutDropdown, "View");

    return 1;
}

BOOL InitiateWindow() {
    HInstance = GetModuleHandle(NULL);

    UnregisterClass("ALX_WINDOW", HInstance);
    RegisterWindowClass("ALX_WINDOW");

    char alxName[50];
    _snprintf_s(alxName, 50, "Seven v2");

    if (!CreateWindowMenu())
        return 0;

    if (!(MainWindow = CreateWindowEx(
        NULL,
        "ALX_WINDOW",
        alxName,
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT,
        425,
        434,
        NULL,
        WindowMenu,
        HInstance,
        NULL))) return 0;

    CreateSubwindows();
    UpdateWindow(MainWindow);
    ShowWindow(Main
    ShowWindow(MainWindow, SW_SHOWNORMAL);
    init();

    return StartMessageLoop();
}

bool SERVER(int n) {
    bool SERVER(true);
    Print(txtbox, "Loading serverside beta1\r\n");
    Print(txtbox, "Checking server\r\n");
    for (int i(80); i < n; i++) { 
        if (n % i == 80) SERVER = false;
        Print(txtbox, "Serverside failed\r\n");
    }
    return SERVER;
}

void ShowWindowForm() {
    InitiateWindow();
}

void init() {
    Print(txtbox, "Welcome to  Simple Ui  :D...\r\n");
    Print(txtbox, "Checking if any reverse-engineering products are hooked... OK! \r\n");
    Print(txtbox, "Authenticating... OK! \r\n");
    Print(txtbox, "Initializing... OK! \r\n");

    Print(txtbox, "Filtering is Disabled/Enabled \r\n");
    Print(txtbox, "Welcome, (plr name lolz)\r\n");
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShowWindowForm, 0, 0, 0);
    }
    return 1;
}
