#pragma once

//ͷ�ļ����Զ��幦�ܺ����ĺ�������
#include<stdio.h>
#include<windows.h>
#include"api.h"

void ShellcodeStart();
void ShellcodeEntry();
void ShellcodeEnd();
void CreateShellcode();
void InitFunctions(PFUNCTIONS pFn);
void CreateConfigFile(PFUNCTIONS pFn);