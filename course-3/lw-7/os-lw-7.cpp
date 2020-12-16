#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <numeric>

#define WINDOW_MOVE_TIMER_ID 1039
#define CURSOR_MOVE_TIMER_ID 1038

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

bool captured = false;

HINSTANCE hInst;
RECT rc;
POINT startPoint;
POINT previousPoint;
POINT currentPoint;

int width = 100;
int height = 100;

int actionSet [3] = {0, 0, 0};
int missingActionCount = 0;

void dropActions()
{
    actionSet[LEFT] = 0;
    actionSet[RIGHT] = 0;
    actionSet[UP] = 0;
}

int accumulateActions()
{
    int accumulate = 0;
    for (int i = 0; i < DOWN; i++) {
        accumulate += actionSet[i];
    }

    return accumulate;
}

void changeAction(int action)
{
    int accumulate = accumulateActions();

    if (accumulate > UP) {
        dropActions();
        captured = false;
        return;
    } else if (accumulate - 1 == action) {
        return;
    } else if (accumulate == action) {
        actionSet[action] = 1;
        return;
    }

    if (missingActionCount > 3) {
        dropActions();
        missingActionCount = 0;
    } else {
        missingActionCount++;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE:
            SetTimer(hWnd, WINDOW_MOVE_TIMER_ID, 50, NULL);
            SetTimer(hWnd, CURSOR_MOVE_TIMER_ID, 50, NULL);
            
            CreateWindowEx(NULL, "SCROLLBAR", "", WS_CHILD | WS_VISIBLE | SBS_HORZ, 0, 0, width, height, hWnd, (HMENU) NULL, hInst, NULL);
            break;
        case WM_TIMER:
            GetCursorPos(&currentPoint);
            switch(wParam) {
                case WINDOW_MOVE_TIMER_ID:
                    if (captured) {
                        int dxStart = currentPoint.x - startPoint.x;
                        int dyStart = currentPoint.y - startPoint.y;

                        if (dxStart || dyStart) {
                            MoveWindow(hWnd, dxStart + rc.left, dyStart + rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
                        }
                    }
                    break;
                case CURSOR_MOVE_TIMER_ID:
                    if (captured) {
                        int dxCurrent = currentPoint.x - previousPoint.x;
                        int dyCurrent = currentPoint.y - previousPoint.y;

                        if (dxCurrent || dyCurrent) {
                            if (dxCurrent > dyCurrent) {
                                if (dxCurrent > 0) {
                                    changeAction(RIGHT);
                                } else if (dxCurrent < 0) {
                                    changeAction(UP);
                                }
                            } else if (dyCurrent > dxCurrent) {
                                if (dyCurrent > 0) {
                                    changeAction(DOWN);
                                } else if (dyCurrent < 0) {
                                    changeAction(LEFT);
                                }
                            }
                        }

                        previousPoint = currentPoint;
                    }
                    break;
            }
        case WM_HSCROLL:
            if (lParam != NULL && GetAsyncKeyState(VK_LBUTTON) & 1) {
                captured = true;
                if (captured) {
                    GetCursorPos(&startPoint);
                    GetWindowRect(hWnd, &rc);

                    previousPoint = startPoint;
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
            break;
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HWND hWnd;
    MSG msg;
    WNDCLASSEX wndClassEx;

    hInst = hInstance;

    memset(&wndClassEx, 0, sizeof(WNDCLASSEX));

    wndClassEx.cbSize = sizeof(WNDCLASSEX);
    wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
    wndClassEx.lpfnWndProc = WndProc;
    wndClassEx.hInstance = hInstance;
    wndClassEx.hbrBackground = (HBRUSH) (COLOR_WINDOW);
    wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClassEx.lpszClassName = "os-lw-7";
    RegisterClassEx(&wndClassEx);

    hWnd = CreateWindowEx(NULL,  "os-lw-7", "os-lw-7", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 30, 200, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
