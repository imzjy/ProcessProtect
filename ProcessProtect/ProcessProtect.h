#define DRIVER_PREFIX "ProcessProtect: "
#define PROCESS_TERMINATE 1

#include "pch.h"
#include "ProcessProtectCommon.h"
#include "FastMutex.h"

const int MaxPids = MAX_PID;

struct Globals {
	int PidsCount;			//currect protected process count
	ULONG Pids[MaxPids];	//protected PIDS
	FastMutex Lock;
	PVOID RegHandle;		//object registraction cookie

	void Init() {
		Lock.Init();
	}
};