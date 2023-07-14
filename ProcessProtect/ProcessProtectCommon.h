#pragma once

#define PROCESS_PROTECT_NAME L"ProcessProtect"

#define IOCTL_PROCESS_PROTECT_BY_ID CTL_CODE(0X8000, 0X800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROCESS_UNPROTECT_BY_ID CTL_CODE(0X8000, 0X801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROCESS_PROTECT_CLEAR CTL_CODE(0X8000, 0X802, METHOD_BUFFERED, FILE_ANY_ACCESS)