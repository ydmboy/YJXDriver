#pragma once
#include "CommonHeader.h"

extern "C"
NTSTATUS DispatchRoutineBuffer(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);

extern "C"
NTSTATUS DispatchRoutineDirect(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
