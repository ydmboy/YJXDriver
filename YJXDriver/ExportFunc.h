#pragma once

#ifndef EXPORT_FUNC_H

#include "CommonHeader.h"

#define EXPORT_FUNC_H


extern "C"
{
	NTSYSAPI PCHAR PsGetProcessImageFileName(IN PEPROCESS pProcess);

	NTSYSAPI
	NTSTATUS
	NTAPI
	ZwQuerySystemInformation(
		IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
		OUT PVOID SystemInformation,
		IN ULONG SystemInformationLength,
		OUT PULONG ReturnLength OPTIONAL 
		);
}

#endif
