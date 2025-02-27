//This has to go into a seperate header because the C++ standard library (std::) and win32 have conflicting method/type names.
//They cannot be included in the same header or footer without it causing problems.
//However we can abstract the conflicting types away, leaving these C++ friendly methods.

#pragma once

#include <windows.h>

namespace win32 {
	inline int createInformationBox(const char * title, const char * text) {
		return MessageBoxA(static_cast<HWND>(nullptr), text, title, MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
	}

	inline int createQuestionBox(const char * title, const char * text) {
		return MessageBoxA(static_cast<HWND>(nullptr), text, title, MB_OK | MB_ICONQUESTION | MB_TASKMODAL);
	}

	inline int createWarningBox(const char * title, const char * text) {
		return MessageBoxA(static_cast<HWND>(nullptr), text, title, MB_OK | MB_ICONWARNING | MB_TASKMODAL);
	}

	inline int createErrorBox(const char * title, const char * text) {
		return MessageBoxA(static_cast<HWND>(nullptr), text, title, MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	inline int createCustomBox(const char * title, const char * text, unsigned int boxType) {
		return MessageBoxA(static_cast<HWND>(nullptr), text, title, boxType);
	}
}
