#include "header.h"
//��ܵ���ڣ�����������ɵ�shellcode�ļ�

//�޸ĳ�����ڵ�Ϊ EntryMain
#pragma comment(linker, "/entry:EntryMain")

int EntryMain() 
{
	//���� shellcode�ļ��ĺ���
	CreateShellcode();
	return 0;
}

void CreateShellcode()
{
	//��Ϊ�޸��˳������ڵ㣬����C��ʽ�� printf �����Ͳ������ˣ�����ʹ�ö�̬������ʽ
	//�����ǰ� printf ������Ϊ��̬������ʽ
	HMODULE hMsvcrt = LoadLibraryA("msvcrt.dll");
	typedef int(__cdecl* fn_printf)(char const* const _Format, ...);
	fn_printf xprintf = (fn_printf)GetProcAddress(hMsvcrt, "printf");

	//����sc.bin�ļ� ����������ɵ�shellcode
	HANDLE hBin = CreateFileA("shellcode.bin", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hBin == INVALID_HANDLE_VALUE)
	{
		xprintf("create file error:%d\n");
		return;
	}

	//�������ɵ� shellcode ��С
	DWORD dwSize = (DWORD)ShellcodeEnd - (DWORD)ShellcodeStart;
	DWORD dwWriten;

	//д�����ɵ�shellcode
	WriteFile(hBin, ShellcodeStart, dwSize, &dwWriten, NULL);
	CloseHandle(hBin);
}