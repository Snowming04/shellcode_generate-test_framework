#pragma once

//shellcode所用到系统函数的函数指针，以及一个结构体里面包含了这些函数指针
#include<windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")

typedef FARPROC(WINAPI* FN_GetProcAddress)(
	__in HMODULE hModule,
	__in LPCSTR lpProcName
	);

typedef HMODULE(WINAPI* FN_LoadLibraryA)(
	__in LPCSTR lpLibFileName
	);


typedef HINTERNET(WINAPI* FN_InternetOpenA)(
	_In_opt_ LPCSTR lpszAgent,
	__in     DWORD  dwAccessType,
	_In_opt_ LPCSTR lpszProxy,
	_In_opt_ LPCSTR lpszProxyBypass,
	__in     DWORD  dwFlags
	);

typedef HINTERNET(WINAPI* FN_InternetOpenUrlA)(
	__in     HINTERNET hInternet,
	__in     LPCSTR    lpszUrl,
	__in     LPCSTR    lpszHeaders,
	__in     DWORD     dwHeadersLength,
	__in     DWORD     dwFlags,
	__in_opt DWORD_PTR dwContext
	);

typedef errno_t(WINAPI* FN_fopen_s)(
	__in FILE** pFile,
	__in const char* filename,
	__in const char* mode
	);

typedef BOOL(WINAPI* FN_InternetCloseHandle)(
	__in HINTERNET hInternet
	);

typedef BOOL(WINAPI* FN_InternetReadFile)(
	__in HINTERNET hFile,
	__in LPVOID    lpBuffer,
	__in DWORD     dwNumberOfBytesToRead,
	__in LPDWORD   lpdwNumberOfBytesRead
	);

typedef size_t(WINAPI* FN_fwrite)(
	__in const void* buffer,
	__in size_t size,
	__in size_t count,
	__in FILE* stream
	);

typedef void* (*FN_malloc)(
	__in size_t _Size
	);

typedef void* (*FN_memset)(
	_Out_opt_bytecapcount_(_Size) void* _Dst,
	_In_ int _Val, 
	_In_ size_t _Size
	);

typedef void* (*FN_free)(
	_Inout_opt_ void* _Memory
	);

typedef int (*FN_fclose)(
	_In_ FILE* stream
);


typedef struct _FUNCTIONS
{
	FN_GetProcAddress      fn_GetProcAddress;
	FN_LoadLibraryA        fn_LoadLibraryA;
	FN_InternetOpenA       fn_InternetOpenA;
	FN_InternetOpenUrlA    fn_InternetOpenUrlA;
	FN_fopen_s             fn_fopen_s;
	FN_InternetCloseHandle fn_InternetCloseHandle;
	FN_InternetReadFile    fn_InternetReadFile;
	FN_fwrite              fn_fwrite;
	FN_malloc              fn_malloc;
	FN_memset              fn_memset;
	FN_free                fn_free;
	FN_fclose              fn_fclose;
}FUNCTIONS, * PFUNCTIONS;