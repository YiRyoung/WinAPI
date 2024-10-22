#include "EngineWindow.h"

HINSTANCE UEngineWindow::hInstance = nullptr;

void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    hInstance = _Instance;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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

int UEngineWindow::WindowMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // 단축키를 아예 안사용하므로 단축키를 처리한다는 일 자차게 없으므로 의미가 없는 코드가 되었다.
        if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

UEngineWindow::UEngineWindow()
{
    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    // wcex.hInstance = nullptr;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "DefaultWindow";
    wcex.hIconSm = nullptr;

    RegisterClassExA(&wcex);

    WindowHandle = CreateWindowA("DefaultWindow", "MainWindow", WS_OVERLAPPEDWINDOW,
        -2000, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!WindowHandle)
    {
        return;
    }
}

UEngineWindow::~UEngineWindow()
{
}

void UEngineWindow::Open()
{
    ShowWindow(WindowHandle, SW_SHOW);
    UpdateWindow(WindowHandle);
}