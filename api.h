#pragma once

//shellcode所用到系统函数的函数指针，以及一个结构体里面包含了这些函数指针
#include<windows.h>

typedef FARPROC(WINAPI* FN_GetProcAddress)(
	__in HMODULE hModule,
	__in LPCSTR lpProcName
	);

typedef HMODULE(WINAPI* FN_LoadLibraryA)(
	__in LPCSTR lpLibFileName
	);

typedef int(WINAPI* FN_MessageBoxA)(
	__in_opt HWND hWnd,
	__in_opt LPCSTR lpText,
	__in_opt LPCSTR lpCaption,
	__in UINT uType);

typedef HANDLE(WINAPI* FN_CreateFileA)(
	__in     LPCSTR lpFileName,
	__in     DWORD dwDesiredAccess,
	__in     DWORD dwShareMode,
	__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	__in     DWORD dwCreationDisposition,
	__in     DWORD dwFlagsAndAttributes,
	__in_opt HANDLE hTemplateFile
	);

typedef struct _FUNCTIONS
{
	FN_GetProcAddress  fn_GetProcAddress;
	FN_LoadLibraryA  fn_LoadLibraryA;
	FN_MessageBoxA  fn_MessageBoxA;
	FN_CreateFileA fn_CreateFileA;

}FUNCTIONS, * PFUNCTIONS;