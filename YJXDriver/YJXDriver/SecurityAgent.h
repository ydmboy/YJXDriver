#pragma once

#ifndef SECURITY_AGENT_H
#define SECURITY_AGENT_H
#include "CommonHeader.h"
#include "GlobalVariables.h"

	extern "C"
	NTSTATUS ListProcessTypeCallbacks();

	extern "C" NTSTATUS UnistallAllProcessType();
	extern "C" OB_PREOP_CALLBACK_STATUS bdgPrintPara_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
	extern "C"
		void PrintAccessRights(ACCESS_MASK DesiredAccess);
	extern "C"
		void PrintCallbackInfo(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
	extern "C" OB_PREOP_CALLBACK_STATUS my_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
	extern "C" void setMemoryProtect();

#endif

