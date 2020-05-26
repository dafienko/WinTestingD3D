#pragma once
#include "lightWin.h"
#include "Graphics.h"

LPCWSTR wndClassName = TEXT("bruh");
HWND hMainWindowHandle;
WNDCLASS wndClass = { 0 };

LRESULT CALLBACK mainWindowProc(HWND, UINT, WPARAM, LPARAM);

void initVariables();
void setupWindows(HINSTANCE hInstance, int iCmdShow);
void registerMainWindowClass();
int runMessageLoop(Graphics* gfx);

HWND workerw;
HWND hProgMan;

void createWorkerWindow();
void getProgMan();
BOOL CALLBACK enumChildWindowProc(HWND, LPARAM);
void findWorkerWindow();
void paintDesktop();