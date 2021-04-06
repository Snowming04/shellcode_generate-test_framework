#include"header.h"
#include"api.h"

//定义想要实现的功能函数
void CreateConfigFile(PFUNCTIONS pFn)
{
	char szHello[] = { 'H','e','l','l','o','!',0 };
	char szTip[] = { 'T','i','p',0 };
	pFn->fn_MessageBoxA(NULL, szHello, szTip, MB_OK);
	char szName[] = { '1','.','t','x','t',0 };
	pFn->fn_CreateFileA(szName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

}
