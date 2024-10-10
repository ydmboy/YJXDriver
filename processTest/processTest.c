#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
	HWND hWnd = NULL;
	HANDLE hProcess = NULL;
	UINT uiPid;

	SetConsoleTitle("B");

	cout << "请输入进程句柄值: ";
	cin >> uiPid;
	hProcess = (HANDLE)uiPid;
	TerminateProcess(hProcess, 0);

	system("pause");

	return(0);
}

