#include <windows.h>
#include <cstdio>

#define CALCPID 227028

UINT GetPidByHwnd(LPCSTR lpcszWindowName)
{
	HWND hWnd = NULL;
	DWORD dwPid = 0;

	if (NULL == lpcszWindowName)
	{
		return(0);
	}
	hWnd = FindWindow(NULL, lpcszWindowName);
	if (NULL == hWnd)
	{
		return(0);
	}
	GetWindowThreadProcessId(hWnd, &dwPid);

	return(dwPid);
}

int main()
{
	HANDLE hCalc = NULL;
	HANDLE hA = NULL;
	HANDLE hB = NULL;
	HANDLE hDup = NULL;
	UINT uiPid = 0;
	BOOL fOk = FALSE;

	SetConsoleTitle("A");
	hCalc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CALCPID);
	if (NULL == hCalc)
	{
		printf("OpenProcess failed... %d\r\n", GetLastError());
		return(-1);
	}
	// 显示被拷贝的进程句柄
	printf("被拷贝的进程句柄为: %I64X\r\n", hCalc);

	// 获取窗口B的PID
	uiPid = GetPidByHwnd("B");
	if (!uiPid)
	{
		return(-1);
	}
	// 打开B进程的窗口句柄
	hB = OpenProcess(PROCESS_ALL_ACCESS, FALSE, uiPid);
	if (NULL == hB)
	{
		return(-1);
	}
	// 把进程Calc的句柄复制过去
	fOk = DuplicateHandle(GetCurrentProcess(), hCalc, hB, &hDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
	if (!fOk)
	{
		return(-1);
	}
	printf("复制过去的句柄值: %I64u\r\n", hDup);

	if (NULL != hCalc)
	{
		CloseHandle(hCalc);
		hCalc = NULL;
	}
	if (NULL != hB)
	{
		CloseHandle(hB);
		hA = NULL;
	}
	if (NULL != hDup)
	{
		CloseHandle(hDup);
		hDup = NULL;
	}

	system("pause");

	return(0);
}
