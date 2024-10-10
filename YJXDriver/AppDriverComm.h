#pragma once

#ifndef APPDRIVERCOMM_H
#include "CommonHeader.h"

//struct _OBJECT_HOOK_INFO
//{
//	UNICODE_STRING func_name;
//	PVOID pre_func; // hook function  Callback_ENTRY_ITEM PreOperation
//	UNICODE_STRING hook;
//	PVOID orig_func; //HOOKED FUNCTION
//	UNICODE_STRING object_type;  
//	PVOID ob_address; // Callback_ENTRY_ITEM:ObjectType
//	PVOID func_modle;
//	POB_PRE_OPERATION_CALLBACK Operation;
//	POBJECT_TYPE  ob_handle;  
//} OBJECT_HOOK_INFO,*POBJECT_HOOK_INFO;
//


NTSTATUS DispatchRoutineBuffer(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);

NTSTATUS DispatchRoutineDirect(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);


#endif


