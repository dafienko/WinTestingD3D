#include "ErrorHandler.h"
#include "lightWin.h"
#include <d3d11.h>
#include <exception>
#include <string>

void CheckHRError(HRESULT hResult, int lineNumber, const char* fileName) {
	LPSTR errorText = NULL;
	if (hResult < 0) {
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			hResult,
			NULL,
			(LPSTR)&errorText,
			0,
			NULL);

		int msgBoxID = MessageBoxA(NULL, 
			(LPCSTR)(("" + std::to_string(lineNumber) + ": " + fileName + ":\n\t " + errorText).c_str()),
			(LPCSTR)("Error"),
			MB_OK | MB_ICONERROR);
	}

}