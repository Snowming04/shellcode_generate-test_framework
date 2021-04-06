#include"header.h"
#include"api.h"


//标记shellcode的开始位置，用来进行shellcode编写前的前置操作和所用到函数的初始化

__declspec(naked) void ShellcodeStart()
{
	__asm
	{
		jmp ShellcodeEntry
	}

}

//获取系统中kernel32.dll基址的方法
__declspec(naked) DWORD getKernel32()
{
	__asm
	{
		mov eax, fs: [030h] ;
		test eax, eax;
		js finished;
		mov eax, [eax + 0ch];
		mov eax, [eax + 14h];
		mov eax, [eax];
		mov eax, [eax]
		mov eax, [eax + 10h]
		finished:
		ret
	}
}

//获取GetProcAddress函数地址
FARPROC getProcAddress(HMODULE hModuleBase)
{
	FARPROC pRet = NULL;
	PIMAGE_DOS_HEADER lpDosHeader;
	PIMAGE_NT_HEADERS32 lpNtHeaders;
	PIMAGE_EXPORT_DIRECTORY lpExports;
	PWORD lpwOrd;
	PDWORD lpdwFunName;
	PDWORD lpdwFunAddr;
	DWORD dwLoop;

	lpDosHeader = (PIMAGE_DOS_HEADER)hModuleBase;
	lpNtHeaders = (PIMAGE_NT_HEADERS)((DWORD)hModuleBase + lpDosHeader->e_lfanew);
	if (!lpNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size)
	{
		return pRet;
	}
	if (!lpNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
	{
		return pRet;
	}
	lpExports = (PIMAGE_EXPORT_DIRECTORY)((DWORD)hModuleBase + (DWORD)lpNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	if (!lpExports->NumberOfNames)
	{
		return pRet;
	}
	lpdwFunName = (PDWORD)((DWORD)hModuleBase + (DWORD)lpExports->AddressOfNames);
	lpwOrd = (PWORD)((DWORD)hModuleBase + (DWORD)lpExports->AddressOfNameOrdinals);
	lpdwFunAddr = (PDWORD)((DWORD)hModuleBase + (DWORD)lpExports->AddressOfFunctions);
	for (dwLoop = 0; dwLoop <= lpExports->NumberOfNames - 1; dwLoop++)
	{
		char* pszFunction = (char*)(lpdwFunName[dwLoop] + (DWORD)hModuleBase);
		if (pszFunction[0] == 'G'
			&& pszFunction[1] == 'e'
			&& pszFunction[2] == 't'
			&& pszFunction[3] == 'P'
			&& pszFunction[4] == 'r'
			&& pszFunction[5] == 'o'
			&& pszFunction[6] == 'c'
			&& pszFunction[7] == 'A'
			&& pszFunction[8] == 'd'
			&& pszFunction[9] == 'd'
			&& pszFunction[10] == 'r'
			&& pszFunction[11] == 'e'
			&& pszFunction[12] == 's'
			&& pszFunction[13] == 's')
		{
			pRet = (FARPROC)(lpdwFunAddr[lpwOrd[dwLoop]] + (DWORD)hModuleBase);
			break;
		}
	}
	return pRet;
}

void InitFunctions(PFUNCTIONS pFn)
{
	//获取LoadLibraryA函数的地址
	pFn->fn_GetProcAddress = (FN_GetProcAddress)getProcAddress((HMODULE)getKernel32());

	char szLoadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A',0 };
	pFn->fn_LoadLibraryA = (FN_LoadLibraryA)pFn->fn_GetProcAddress((HMODULE)getKernel32(), szLoadLibraryA);

	//使用LoadLibrary函数载入User32.dll，然后在里面搜寻MessageBoxA函数的地址
	char szUser32[] = { 'U','s','e','r','3','2','.','d','l','l',0 };
	char szMessageBoxA[] = { 'M','e','s','s','a','g','e','B','o','x','A',0 };
	pFn->fn_MessageBoxA = (FN_MessageBoxA)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szUser32), szMessageBoxA);

	//在Kernel32.dll中找到CreateFileA函数的地址
	char szCreateFileA[] = { 'C','r','e','a','t','e','F','i','l','e','A',0 };
	pFn->fn_CreateFileA = (FN_CreateFileA)pFn->fn_GetProcAddress((HMODULE)getKernel32(), szCreateFileA);
}

void ShellcodeEntry()
{
	FUNCTIONS fn;
	InitFunctions(&fn);
	CreateConfigFile(&fn);
}