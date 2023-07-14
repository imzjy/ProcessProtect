#define DRIVER_PREFIX "ProcessProtect: "
#define PROCESS_TERMINATE 1

#include "pch.h"
#include "FastMutex.h"

const int MaxPids = 256;

struct Globals {
	int PidsCount;			//currect protected process count
	ULONG Pids[MaxPids];	//protected PIDS
	FastMutex Lock;
	PVOID RegHandle;		//object registraction cookie

	void Init() {
		Lock.Init();
	}
};