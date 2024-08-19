#pragma once

#ifndef __MACROUTILS__H
#define __MACROUTILS__H


#define DRIVER_ERROR(fmt, ...) DbgPrint("[File: %s, Line: %d] " fmt, __FILE__, __LINE__, __VA_ARGS__)


#define CHECK_STATUS(status, exception)                     \
	__try {                                                \
		status = (exception);                               \
		if (!NT_SUCCESS(status)) {                           \
			DRIVER_ERROR("Error: %08x\n FILE:", status);            \
			goto CLEANUP;                                  \
		}                                                 \
	} __except(EXCEPTION_EXECUTE_HANDLER) {                                          \
		DRIVER_ERROR("Error: %08x\n", GetExceptionCode());  \
		goto CLEANUP;                                     \
	}

#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define PROCESS_TERMINATE (0x0001)
#define PROCESS_CREATE_THREAD (0x0002)
#define PROCESS_SET_SESSIONID (0x0004)
#define PROCESS_VM_OPERATION (0x0008)
#define PROCESS_VM_READ (0x0010)
#define PROCESS_VM_WRITE (0x0020)
#define PROCESS_DUP_HANDLE (0x0040)
#define PROCESS_CREATE_PROCESS (0x0080)
#define PROCESS_SET_QUOTA (0x0100)
#define PROCESS_SET_INFORMATION (0x0200)
#define PROCESS_QUERY_INFORMATION (0x0400)
#define PROCESS_SUSPEND_RESUME (0x0800)
#define PROCESS_QUERY_LIMITED_INFORMATION (0x1000)
#define PROCESS_SET_LIMITED_INFORMATION (0x2000)




#define EX_PUSH_LOCK ULONG_PTR

typedef struct _CALLBACK_ENTRY CALLBACK_ENTRY;
typedef struct _CALLBACK_ENTRY_ITEM CALLBACK_ENTRY_ITEM;

typedef struct _KLDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	ULONG64 __Undefined1;
	ULONG64 __Undefined2;
	ULONG64 __Undefined3;
	ULONG64 NonPagedDebugInfo;
	ULONG64 DllBase; //.sys's dllbase
	ULONG64 EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG   Flags;
	USHORT  LoadCount;
	USHORT  __Undefined5;
	ULONG64 __Undefined6;
	ULONG   CheckSum;
	ULONG   __padding1;
	ULONG   TimeDateStamp;
	ULONG   __padding2;
}KLDR_DATA_TABLE_ENTRY,LDR_DATA_TABLE_ENTRY,*PKLDR_DATA_TABLE_ENTRY;


typedef struct _OBJECT_TYPE {
	LIST_ENTRY TypeList;
	UNICODE_STRING Name;
	void* DefaultObject;
	unsigned __int8 Index;
	unsigned int TotalNumberOfObjects;
	unsigned int TotalNumberOfHandles;
	unsigned int HighWaterNumberOfObjects;
	unsigned int HighWaterNumberOfHandles;
	//_OBJECT_TYPE_INITIALIZER TypeInfo;
	unsigned __int8 Placeholder[0x78];
	EX_PUSH_LOCK TypeLock;
	unsigned int Key;
	LIST_ENTRY CallbackList;
}OBJECT_TYPE;
typedef struct _CALLBACK_ENTRY_ITEM {
	LIST_ENTRY EntryItemList;
	OB_OPERATION Operations;
	CALLBACK_ENTRY* CallbackEntry;
	POBJECT_TYPE ObjectType;
	POB_PRE_OPERATION_CALLBACK PreOperation;
	POB_POST_OPERATION_CALLBACK PostOperation;
	__int64 unk;
}CALLBACK_ENTRY_ITEM;

typedef struct _CALLBACK_ENTRY {
	__int16 Version;
	char buffer1[6];
	POB_OPERATION_REGISTRATION RegistrationContext;
	__int16 AltitudeLength1;
	__int16 AltitudeLength2;
	char buffer2[4];
	WCHAR* AltitudeString;
	CALLBACK_ENTRY_ITEM Items;
}CALLBACK_ENTRY;


typedef struct _CALL_BACK_INFO
{
	ULONG64 Unknow;
	ULONG64 Unknow1;
	UNICODE_STRING AltitudeString;
	LIST_ENTRY NexEntryItemList;
	ULONG64 Operations;
	PVOID ObHandle;
	PVOID ObjectType;
	ULONG64 PreCallbackAddr;
	ULONG64 PostCallbackAddr;
}CALL_BACK_INFO,*PCALL_BACK_INFO;

typedef struct _OB_CALLBACK
{
	LIST_ENTRY ListEntry;
	ULONG64 Operations;
	PCALL_BACK_INFO ObHandle;
	ULONG64 ObjTypeAddr;
	ULONG64 PreCall;
	ULONG64 PostCall;

}OB_CALLBACK,*POB_CALLBACK;




typedef struct _OBJECT_TYPE_FLAGS {
	UCHAR CaseInsensitive : 1;
	UCHAR UnnamedObjectsOnly : 1;
	UCHAR UseDefaultObject : 1;
	UCHAR SecurityRequired : 1;
	UCHAR MaintainHandleCount : 1;
	UCHAR MaintainTypeList : 1;
	UCHAR SupportsObjectCallbacks : 1;
	UCHAR CacheAligned : 1;
}OBJECT_TYPE_FLAGS, * P_OBJECT_TYPE_FLAGS;
 
 
typedef struct _OBJECT_TYPE_INITIALIZER {
	USHORT				wLength;
	OBJECT_TYPE_FLAGS	ObjectTypeFlags;
	ULONG				ObjcetTypeCode;
	ULONG				InvalidAttributes;
	GENERIC_MAPPING		GenericMapping;
	ULONG				ValidAccessMask;
	ULONG				RetainAccess;
	ULONG				PoolType;
	ULONG				DefaultPagedPoolCharge;
	ULONG				DefaultNonPagedPoolCharge;
	PVOID				DumpProcedure;
	PVOID				OpenProcedure;
	PVOID				CloseProcedure;
	PVOID				DeleteProcedure;
	PVOID				ParseProcedure;
	PVOID				SecurityProcedure;
	PVOID				QueryNameProcedure;
	PVOID				OkayToCloseProcedure;
}OBJECT_TYPE_INITIALIZER, * POBJECT_TYPE_INITIALIZER;
 
typedef struct _OBJECT_TYPE_EX {
	LIST_ENTRY					TypeList;
	UNICODE_STRING				Name;
	ULONGLONG					DefaultObject;
	ULONG						Index;
	ULONG						TotalNumberOfObjects;
	ULONG						TotalNumberOfHandles;
	ULONG						HighWaterNumberOfObjects;
	ULONG						HighWaterNumberOfHandles;
	OBJECT_TYPE_INITIALIZER		TypeInfo;
	ULONGLONG					TypeLock;
	ULONG						Key;
	LIST_ENTRY					CallbackList;
}OBJECT_TYPE_EX, * POBJECT_TYPE_EX;

#endif
