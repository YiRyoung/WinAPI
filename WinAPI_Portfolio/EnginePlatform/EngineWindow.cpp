#include "PreCompile.h"
#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

// 전역 변수 초기화
HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEXA> UEngineWindow::WindowClasses;
int WindowCount = 0;    // 현재 생성된 윈도우 창의 개수

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:    // 윈도우 창을 닫을 시
        --WindowCount;  // 윈도우 창의 개수 감소
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    hInstance = _Instance;

    // 윈도우 기본 설정 등록
    WNDCLASSEXA wcex;
    // 구조체 값 초기화 코드
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;                               // 윈도우 아이콘 출력
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);      // 윈도우 아이콘 설정
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;                        // 윈도우 상단 메뉴 설정
    wcex.lpszClassName = "Default";
    wcex.hIconSm = nullptr;
    CreateWindowClass(wcex);
}

int UEngineWindow::WindowMessageLoop(EngineDelegate _StartFunction, EngineDelegate _FrameFunction)
{
    MSG msg = MSG();

    // 프로그램 시작 후 딱 한 번 실행할 것들
    _StartFunction();

    // 윈도우 창이 1개 이상일 때에만 작동하며 
    // 창이 전부 꺼지면 프로그램도 종료됨
    while ( 0 != WindowCount)
    {
        if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (true == _FrameFunction.IsBind())
        {
            _FrameFunction();
        }
    }
    return (int)msg.wParam;
}

void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& _Class)
{
    // 맵을 사용하여 윈도우 관리
    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasses.end();
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasses.find(std::string(_Class.lpszClassName));

    if (EndIter != FindIter)
    {
        MSGASSERT(std::string(_Class.lpszClassName) + "같은 이름의 윈도우 클래스를 2번 등록했습니다");
        return;
    }

    RegisterClassExA(&_Class);
    WindowClasses.insert(std::pair{ _Class.lpszClassName, _Class });
}

UEngineWindow::UEngineWindow()
{
  
}

UEngineWindow::~UEngineWindow()
{

}

// 타이틀 이름과 클래스 이름을 직접 작성하여 윈도우를 생성하는 함수
void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    // 맵에 해당 클래스가 존재하지 않을 경우 (클래스 등록 안됨)
    if (false == WindowClasses.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + "등록하지 않은 클래스로 윈도우창을 만들려고 했습니다");
        return;
    }

    // 해당 클래스와 타이틀 이름으로 윈도우 창 생성
    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW, 0, 0, 
        CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    // 생성이 제대로 되지 않았을 경우 에러 메세지 발생
    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + " 윈도우 생성에 실패했습니다.");
        return;
    }
}

void UEngineWindow::Open(std::string_view _TitleName)
{
    // 윈도우가 만들어지지 않는다면 만든다.
    if (0 == WindowHandle)
    {
        Create("Window");
    }

    if (0 == WindowHandle)
    {
        return;
    }

    ShowWindow(WindowHandle, SW_SHOW);  // 윈도우를 보이게끔 하는 함수
    UpdateWindow(WindowHandle);
    ++WindowCount;                      // 윈도우 창의 개수 증가 
}