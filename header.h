#pragma once

//头文件及自定义功能函数的函数声明
#include<stdio.h>
#include<windows.h>
#include"api.h"

void ShellcodeStart();
void ShellcodeEntry();
void ShellcodeEnd();
void CreateShellcode();
void InitFunctions(PFUNCTIONS pFn);
void CreateConfigFile(PFUNCTIONS pFn);