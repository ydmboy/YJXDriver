#pragma once

#ifndef SECURITY_AGENT_H
#define SECURITY_AGENT_H
#include "CommonHeader.h"
#include "GlobalVariables.h"

	
	NTSTATUS ListProcessTypeCallbacks();

	 NTSTATUS UnistallAllProcessType();
	 OB_PREOP_CALLBACK_STATUS bdgPrintPara_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
	
		void PrintAccessRights(ACCESS_MASK DesiredAccess);
	
		void PrintCallbackInfo(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
	 OB_PREOP_CALLBACK_STATUS my_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
	 void setMemoryProtect();

#endif

