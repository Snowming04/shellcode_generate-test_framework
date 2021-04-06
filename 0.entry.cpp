#include "header.h"
//框架的入口，创建最后生成的shellcode文件

//修改程序入口点为 EntryMain
#pragma comment(linker, "/entry:EntryMain")

int EntryMain() 
{
	//创建 shellcode文件的函数
	CreateShellcode();
	return 0;
}

void CreateShellcode()
{
	//因为修改了程序的入口点，所以C形式的 printf 函数就不能用了，必须使用动态调用形式
	//下面是把 printf 函数改为动态调用形式
	HMODULE hMsvcrt = LoadLibraryA("msvcrt.dll");
	typedef int(__cdecl* fn_printf)(char const* const _Format, ...);
	fn_printf xprintf = (fn_printf)GetProcAddress(hMsvcrt, "printf");

	//创建sc.bin文件 用来存放生成的shellcode
	HANDLE hBin = CreateFileA("shellcode.bin", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hBin == INVALID_HANDLE_VALUE)
	{
		xprintf("create file error:%d\n");
		return;
	}

	//计算生成的 shellcode 大小
	DWORD dwSize = (DWORD)ShellcodeEnd - (DWORD)ShellcodeStart;
	DWORD dwWriten;

	//写入生成的shellcode
	WriteFile(hBin, ShellcodeStart, dwSize, &dwWriten, NULL);
	CloseHandle(hBin);
}