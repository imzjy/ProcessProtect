// protect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "..\ProcessProtect\ProcessProtectCommon.h"


int Error(const char* msg) {
	printf("%s (Error: %d)\n", msg, ::GetLastError());
	return 1;
}

int PrintUsage() {
	printf("protect [add|remove|clear] [pid1] [pid2]...\n");
	return 0;
}

std::vector<DWORD> ParsePids(const wchar_t* buffer[], int count) {
	std::vector<DWORD> pids;
	for (int i = 0; i < count; i++)
	{
		pids.push_back(::_wtoi(buffer[i]));
	}

	return pids;
}

int wmain(int argc, const wchar_t* argv[])
{
	if (argc < 2)
	{
		return PrintUsage();
	}

	enum class Options {
		Unknown,
		Add, Remove, Clear, Query
	};

	Options option;
	if (::_wcsicmp(argv[1], L"add") == 0)
	{
		option = Options::Add;
	}
	else if (::_wcsicmp(argv[1], L"remove") == 0)
	{
		option = Options::Remove;
	}
	else if (::_wcsicmp(argv[1], L"clear") == 0)
	{
		option = Options::Clear;
	}
	else if (::_wcsicmp(argv[1], L"query") == 0)
	{
		option = Options::Query;
	}
	else 
	{
		printf("Unknown option. Option: %S\n", argv[1]);
		return PrintUsage();
	}

	HANDLE hFile = ::CreateFile(L"\\\\.\\" PROCESS_PROTECT_NAME, GENERIC_WRITE | GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return Error("Failed to open device");
	}

	std::vector<DWORD> pids;
	BOOL success = false;
	DWORD bytes;
	switch (option) {
	case Options::Add:
	{
		pids = ParsePids(argv + 2, argc - 2);
		success = ::DeviceIoControl(hFile, IOCTL_PROCESS_PROTECT_BY_ID,
			pids.data(), static_cast<DWORD>(pids.size()) * sizeof(DWORD),
			nullptr, 0, &bytes, nullptr
		);
		break;
	}
	case Options::Remove:
	{
		pids = ParsePids(argv + 2, argc - 2);
		success = ::DeviceIoControl(hFile, IOCTL_PROCESS_UNPROTECT_BY_ID,
			pids.data(), static_cast<DWORD>(pids.size()) * sizeof(DWORD),
			nullptr, 0, &bytes, nullptr
		);
		break;
	}
	case Options::Clear:
	{
		pids = ParsePids(argv + 2, argc - 2);
		success = ::DeviceIoControl(hFile, IOCTL_PROCESS_PROTECT_CLEAR,
			pids.data(), static_cast<DWORD>(pids.size()) * sizeof(DWORD),
			nullptr, 0, &bytes, nullptr
		);
		break;
	}
	case Options::Query:
	{
		ULONG pids[MAX_PID];
		::memset(pids, 0, sizeof(pids));
		success = ::DeviceIoControl(hFile, IOCTL_PROCESS_PROTECT_QUERY,
			nullptr, 0,
			pids, MAX_PID * sizeof(ULONG),
			&bytes, nullptr
		);

		for (int i = 0; i < MAX_PID; i++)
		{
			if (pids[i] != 0)
			{
				printf("%u ", pids[i]);
			}
		}
		printf("\n");

		break;
	}
	}

	if (!success)
	{
		return Error("Failed in DeviceIoControl");
	}

	printf("Operation succeeded.\n");
	::CloseHandle(hFile);

	return 0;
}

