#include <string>
#include <iostream>
#include <stdlib.h>

#include "main.h"
#include "Timer.h"

#define WM_CREATE_WORKER_WINDOW 0x052C
 


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
	initVariables();
	setupWindows(hInstance, iCmdShow);

	Graphics gfx = Graphics(hMainWindowHandle);

	getProgMan();
	createWorkerWindow();
	findWorkerWindow();

	int exitCode = runMessageLoop(&gfx);

	return exitCode;
}

int runMessageLoop(Graphics* gfx) {
	MSG msg;
	int frameCount = 0;
	while (true) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				return msg.wParam;
			}
		}	

		gfx->ClearBuffer(1, 0, 0);
		gfx->Draw();
		gfx->EndFrame();

		//paintDesktop();

		if (gfx->timer.peek() > 1 || frameCount == 1) {
			if (frameCount == 0) {
				gfx->timer.mark();
				frameCount = 1;
			}
			else if (frameCount == 1) {
				float timeSinceLastFrame = gfx->timer.peek();
				float fps = (1.0f / timeSinceLastFrame);
				SetWindowTextA(hMainWindowHandle, ((std::string)("FPS: " + std::to_string(fps))).c_str());
				frameCount = 0;
			}

		}
	}
}

LRESULT CALLBACK mainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(69);
		break;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void registerMainWindowClass() {
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = NULL;
	wndClass.hInstance = NULL;
	wndClass.hIcon = NULL;
	wndClass.lpfnWndProc = mainWindowProc;
	wndClass.lpszClassName = wndClassName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_OWNDC;

	RegisterClass(&wndClass);
}

void initVariables() {
	registerMainWindowClass();
}

void setupWindows(HINSTANCE hInstance, int iCmdShow) {
	hMainWindowHandle = CreateWindow(wndClassName, TEXT("My Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, // position
		400, 400, // dimensions
		NULL, NULL, hInstance, NULL);

	ShowWindow(hMainWindowHandle, iCmdShow);
	UpdateWindow(hMainWindowHandle);
}

void getProgMan() {
	hProgMan = FindWindow(TEXT("Progman"), NULL);
}

void createWorkerWindow() {
	int* result = 0;
	SendMessageTimeout(hProgMan,
		WM_CREATE_WORKER_WINDOW,
		NULL,
		NULL,
		SMTO_NORMAL,
		1000,
		(PDWORD_PTR)(&result));
}

void findWorkerWindow() {
	EnumWindows(enumChildWindowProc, NULL);
}

BOOL CALLBACK enumChildWindowProc(HWND hTopWindow, LPARAM lParam)
{
	HWND p = FindWindowEx(hTopWindow,
		NULL,
		TEXT("SHELLDLL_DefView"),
		NULL);

	if (p != NULL) {
		workerw = FindWindowEx(NULL,
			hTopWindow,
			TEXT("WorkerW"),
			NULL);
	}

	return TRUE;
};

POINT cursorPos = { 0, 0 };
void paintDesktop() {
	HDC hdc = GetDCEx(workerw, NULL, 0x403);

	if (hdc != NULL) {

		HPEN pen = CreatePen(PS_DASHDOT,
			9,
			RGB(rand() % 255, rand() % 255, rand() % 255)
		);

		SelectObject(hdc, pen);

		
		MoveToEx(hdc, cursorPos.x, cursorPos.y, NULL);
		GetCursorPos(&cursorPos);
		LineTo(hdc, cursorPos.x, cursorPos.y);


		ReleaseDC(workerw, hdc);

		DeleteObject(pen);
	}
}