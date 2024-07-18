#pragma once

#ifndef __MACROUTILS__H
#define __MACROUTILS__H


//
#define DRIVER_ERROR(x, ...) DbgPrint(x,__VA_ARGS__)
#define CHECK_STATUS(status,exception)       \
	__try                                    \
	{                                         \
		status = (exception);                                     \
		if (!NT_SUCCESS(status))           \
		{                                  \
			DRIVER_ERROR("Error: %08x\n", status); \
			goto CLEANUP;                     \
		}                     \
	}                                           \
	__except                                       \
	{                                       \
			DRIVER_ERROR("Error: %08x\n", GetExceptionCode()); \
			goto CLEANUP;                      	}





#endif
