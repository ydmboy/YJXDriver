#include "SecurityAgent.h"


extern "C"
NTSTATUS ListProcessTypeCallbacks()
{
    OBJECT_TYPE* processObjectType = *PsProcessType;
    PLIST_ENTRY head = (PLIST_ENTRY)&(processObjectType->CallbackList);
    PLIST_ENTRY current = head->Flink;

    while (current != head)
    {
        CALLBACK_ENTRY_ITEM* item = (CALLBACK_ENTRY_ITEM*)current;
        CALLBACK_ENTRY* entry = item->CallbackEntry;

        if (entry)
        {
            // 处理或打印回调信息
            DbgPrint("Callback found: %ws,Address:%p\n", entry->AltitudeString,entry);
			DbgPrint("gsHande:%p",gs_handleCallback);
			DbgPrint("Func:%p",my_pre_callback);
        }

        current = current->Flink;
    }
    return STATUS_SUCCESS;
}

extern "C"
NTSTATUS UnistallAllProcessType()
{
	OBJECT_TYPE* pspt = *(POBJECT_TYPE*)PsProcessType;
	PLIST_ENTRY head = (PLIST_ENTRY)&(pspt->CallbackList);
	PLIST_ENTRY current = head->Blink;
	while (current != head)
	{
		CALLBACK_ENTRY_ITEM* item = (CALLBACK_ENTRY_ITEM*)current;
		CALLBACK_ENTRY* entry = item->CallbackEntry;
		if (entry)
		{
			ObUnRegisterCallbacks(entry);
		}
		current = current->Blink;
	}
	return STATUS_SUCCESS;

}


// func: my_pre_callback  callback-function
// name:




extern "C"
OB_PREOP_CALLBACK_STATUS bdgPrintPara_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	// 打印回调函数的参数和相关信息
	PrintCallbackInfo(RegistrationContext, OperationInformation);

	return OB_PREOP_SUCCESS;
}

extern "C"
void PrintCallbackInfo(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	PEPROCESS  peProcess = PsGetCurrentProcess();
	PCHAR peProcessName = PsGetProcessImageFileName(peProcess);
	//debug off
	//DbgPrint("ImageFileName:%s\n",peProcessName);
	// cheatengine-x8
	if (!strcmp("cheatengine-x8", peProcessName) || !strcmp("Calculator.exe",peProcessName))
	{
		
	}
	else
	{
		return;
	}

	DbgPrint("----------------------------------------------------------------------");
	// 打印 RegistrationContext
	DbgPrint("RegistrationContext: %p\n", RegistrationContext);
	DbgPrint("ImageFileName:%s\n",peProcessName);
	// 打印 OperationInformation 结构体中的详细信息
	DbgPrint("OperationInformation:\n");
	DbgPrint("  Operation: %u\n", OperationInformation->Operation);
	DbgPrint("  KernelHandle: %s\n", OperationInformation->KernelHandle ? "TRUE" : "FALSE");
	DbgPrint("  Object: %p\n", OperationInformation->Object);

	// 根据操作类型打印详细的访问权限信息
	if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
	{
		DbgPrint("  Handle Create Operation:\n");
		DbgPrint("    Original Desired Access: %08x\n", OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess);
		DbgPrint("    Desired Access: %08x\n", OperationInformation->Parameters->CreateHandleInformation.DesiredAccess);
		PrintAccessRights(OperationInformation->Parameters->CreateHandleInformation.DesiredAccess);
	}
	else if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
	{
		DbgPrint("  Handle Duplicate Operation:\n");
		DbgPrint("    Original Desired Access: %08x\n", OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess);
		DbgPrint("    Desired Access: %08x\n", OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess);
		PrintAccessRights(OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess);
		DbgPrint("    Source Process: %p\n", OperationInformation->Parameters->DuplicateHandleInformation.SourceProcess);
		DbgPrint("    Target Process: %p\n", OperationInformation->Parameters->DuplicateHandleInformation.TargetProcess);
	}
	DbgPrint("----------------------------------------------------------------------");
}

extern "C"
void PrintAccessRights(ACCESS_MASK DesiredAccess)
{
	DbgPrint("    Access Rights:\n");

	if (DesiredAccess & PROCESS_TERMINATE)
		DbgPrint("      - PROCESS_TERMINATE\n");

	if (DesiredAccess & PROCESS_CREATE_THREAD)
		DbgPrint("      - PROCESS_CREATE_THREAD\n");

	if (DesiredAccess & PROCESS_SET_SESSIONID)
		DbgPrint("      - PROCESS_SET_SESSIONID\n");

	if (DesiredAccess & PROCESS_VM_OPERATION)
		DbgPrint("      - PROCESS_VM_OPERATION\n");

	if (DesiredAccess & PROCESS_VM_READ)
		DbgPrint("      - PROCESS_VM_READ\n");

	if (DesiredAccess & PROCESS_VM_WRITE)
		DbgPrint("      - PROCESS_VM_WRITE\n");

	if (DesiredAccess & PROCESS_DUP_HANDLE)
		DbgPrint("      - PROCESS_DUP_HANDLE\n");

	if (DesiredAccess & PROCESS_CREATE_PROCESS)
		DbgPrint("      - PROCESS_CREATE_PROCESS\n");

	if (DesiredAccess & PROCESS_SET_QUOTA)
		DbgPrint("      - PROCESS_SET_QUOTA\n");

	if (DesiredAccess & PROCESS_SET_INFORMATION)
		DbgPrint("      - PROCESS_SET_INFORMATION\n");

	if (DesiredAccess & PROCESS_QUERY_INFORMATION)
		DbgPrint("      - PROCESS_QUERY_INFORMATION\n");

	if (DesiredAccess & PROCESS_SUSPEND_RESUME)
		DbgPrint("      - PROCESS_SUSPEND_RESUME\n");

	if (DesiredAccess & PROCESS_QUERY_LIMITED_INFORMATION)
		DbgPrint("      - PROCESS_QUERY_LIMITED_INFORMATION\n");

	if (DesiredAccess & PROCESS_SET_LIMITED_INFORMATION)
		DbgPrint("      - PROCESS_SET_LIMITED_INFORMATION\n");

	if (DesiredAccess & SYNCHRONIZE)
		DbgPrint("      - SYNCHRONIZE\n");

	if (DesiredAccess & STANDARD_RIGHTS_REQUIRED)
		DbgPrint("      - STANDARD_RIGHTS_REQUIRED\n");

	if (DesiredAccess & DELETE)
		DbgPrint("      - DELETE\n");

	if (DesiredAccess & READ_CONTROL)
		DbgPrint("      - READ_CONTROL\n");

	if (DesiredAccess & WRITE_DAC)
		DbgPrint("      - WRITE_DAC\n");

	if (DesiredAccess & WRITE_OWNER)
		DbgPrint("      - WRITE_OWNER\n");

	if (DesiredAccess & PROCESS_ALL_ACCESS)
		DbgPrint("      - PROCESS_ALL_ACCESS\n");
}

extern "C"
OB_PREOP_CALLBACK_STATUS my_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{

	
	//LARGE_INTEGER delayTime = { 0 };
	//delayTime.QuadPart = -50000000;
	//KeDelayExecutionThread(KernelMode, FALSE, &delayTime);


	// 获取发出请求的进程
    PEPROCESS requestorProcess = PsGetCurrentProcess();
    HANDLE requestorProcessId = PsGetProcessId(requestorProcess);
    PCHAR requestorProcessName = PsGetProcessImageFileName(requestorProcess);

    // 获取目标进程对象和进程名称
    PEPROCESS targetProcess = (PEPROCESS)OperationInformation->Object;
    PCHAR targetProcessName = PsGetProcessImageFileName(targetProcess);

	//DbgPrint("File: %s, Line: %d - Requestor Process: %s (PID: %d)\n", __FILE__, __LINE__, requestorProcessName, requestorProcessId);
	//DbgPrint("File: %s, Line: %d - Target Process: %s\n", __FILE__, __LINE__, targetProcessName);



    //DbgPrint("Requestor Process: %s (PID: %d)\n", requestorProcessName, requestorProcessId);
    //DbgPrint("Target Process: %s\n", targetProcessName);

	//DbgPrint("yjx:sys eEPROCESS=%p",OperationInformation->Object);
	PEPROCESS process = (PEPROCESS)OperationInformation->Object;
	//PEPROCESS process = IoThreadToProcess((PETHREAD)OperationInformation->Object);
	PCHAR processName = PsGetProcessImageFileName(process);









	//int i = strlen(processName);
	//int j = strlen("calc.exe");
	//if (_stricmp(processName, "calculator.exe") != 0)
	//if(_stricmp(processName,"SecurityHealthService.exe")==0)
	//DbgPrint("%s",processName);
	    if (_stricmp(processName, "Calculator.exe") == 0)
    {
        // 检查并修改句柄的 DesiredAccess 字段
        if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
        {
            if (OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_READ)
            {
                OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_READ;
                DbgPrint("Blocked PROCESS_TERMINATE for calc.exe (handle create)\n");
            }
        }
        else if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
        {
            if (OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess & PROCESS_VM_READ)
            {
                OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_VM_READ;
                DbgPrint("Blocked PROCESS_TERMINATE for calc.exe (handle duplicate)\n");
            }
        }
    }

    return OB_PREOP_SUCCESS;










	///


	if(_stricmp(processName,"calc.exe")==0)
	{

		//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
		//OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
		//DbgPrint("ydmboy:TERMINATE");





		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess |= PROCESS_TERMINATE;
		OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess |= PROCESS_TERMINATE;
		DbgPrint("ydmboy:ALL_PROCESS");

		//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = PROCESS_ALL_ACCESS;
	
		//if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		//{

		//if (OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE)
		//{
			//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			//OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			//DbgPrint("calc Process 2\n");
		//}
		//if (OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess & PROCESS_TERMINATE)
		//{

		//	OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE; 
		//}

		//}
		//if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE) //{
		//	OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
		//}
		//else if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		//{
		//	OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
		//}
		//return STATUS_UNSUCCESSFUL;
	}


	//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
	return OB_PREOP_SUCCESS;


	if (OperationInformation->KernelHandle)
	{

	}
	else
	{

		ACCESS_MASK OriginalPermination = OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess;
		//ACCESS_MASK OriginalPerminationBackup = OriginalPermination;
		ACCESS_MASK NewPermination = OperationInformation->Parameters->CreateHandleInformation.DesiredAccess;
		//OriginalPermination &= ~PROCESS_TERMINATE;
		OriginalPermination &= ~PROCESS_VM_READ;
		OriginalPermination &= ~PROCESS_VM_WRITE;

		//OriginalPermination &= ~PROCESS_VM_WRITE;
		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = OriginalPermination;
		DbgPrint("yjx:原始权限 %x； 新的权限=%x",OriginalPermination,NewPermination);
	}
	return OB_PREOP_SUCCESS;
}

extern "C"
void setMemoryProtect()
{
	OB_CALLBACK_REGISTRATION ob1_callback_reg = { 0 };
	OB_OPERATION_REGISTRATION ob2_operation = { 0 };
	RtlInitUnicodeString(&ob1_callback_reg.Altitude, L"31001");
	ob1_callback_reg.RegistrationContext = NULL;
	ob1_callback_reg.Version = OB_FLT_REGISTRATION_VERSION;
	ob1_callback_reg.OperationRegistrationCount = 1; // 只注册一个回调
	ob1_callback_reg.OperationRegistration = &ob2_operation;

	ob2_operation.ObjectType = PsProcessType;
	ob2_operation.Operations |= OB_OPERATION_HANDLE_CREATE;
	//ob2_operation.Operations |= OB_OPERATION_HANDLE_DUPLICATE;

	ob2_operation.PostOperation = NULL;
	//ob2_operation.PreOperation = my_pre_callback;

	ob2_operation.PreOperation = my_pre_callback;
	NTSTATUS status = ObRegisterCallbacks(&ob1_callback_reg, &gs_handleCallback);


	
	switch (status)
	{
	case STATUS_SUCCESS:
	{
		DbgPrint("Success:YJX:sys SetMemoryProtecte gs_handleCallback=%p \n", gs_handleCallback);
		break;
	}
	case STATUS_INVALID_PARAMETER:
	{
		DbgPrint("Failed:STATUS_INVALID_PARAMETER");
		break;
	}
	case STATUS_FLT_INSTANCE_ALTITUDE_COLLISION
		:
	{
		DbgPrint("Failed:STATUS_FLT_INSTANCE_ALTITUDE_COLLISION");
		break;
	}
	case STATUS_ACCESS_DENIED:
	{
		DbgPrint("Failed:STATUS_ACCESS_DENIED");
		break;
	}
	case STATUS_INSUFFICIENT_RESOURCES:
	{
		DbgPrint("Failed:STATUS_INSUFFICIENT_RESOURCES");
		break;
	}
	default:
		break;
	}
}