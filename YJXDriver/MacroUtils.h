#pragma once

#ifndef __MACROUTILS__H
#define __MACROUTILS__H


//
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





#endif
