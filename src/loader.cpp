#include <iostream>
#include <Windows.h>


// Syntax in command line:
// loader.exe [dll path] [pid]
int main(int argc, char * argv[]) {

	if (argc != 3) {
		std::cout << "[-] Usage is wrong. Parameters required are 3.\n";
		return 1;
	}

	PCSTR dllPath = argv[1];

	HMODULE hK32 = GetModuleHandleA("Kernel32.dll");
	if (hK32 == nullptr) {
		std::cout << "[-] Could not get handle to Kernel32. Error -> " << GetLastError() << std::endl;
		return 1;
	}

	FARPROC loadLibAddress = GetProcAddress(hK32, "LoadLibraryA");
	if (loadLibAddress == nullptr) {
		std::cout << "[-] Could not get 'LoadLibraryA' virtual address. Error -> " << GetLastError() << std::endl;
		return 1;
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, atoi(argv[2]));
	if (hProc == nullptr) {
		std::cout << "[-] Could not get handle to the process\n";
		return 1;
	}

	LPVOID reservedMemory = VirtualAllocEx(hProc, nullptr, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (reservedMemory == nullptr) {
		std::cout << "[-] Could not allocated memory in the process\n";
		CloseHandle(hProc);
		return 1;
	}

	BOOL bWriteDllInProcessMemory = WriteProcessMemory(hProc, reservedMemory, (LPCVOID)dllPath, strlen(dllPath) + 1, nullptr);
	if (!bWriteDllInProcessMemory) {
		std::cout << "[-] Could not write dll in the process memory\n";
		VirtualFreeEx(hProc, reservedMemory, strlen(dllPath) + 1, MEM_DECOMMIT);
		CloseHandle(hProc);
		return 1;
	}

	HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibAddress, reservedMemory, 0, 0);
	if (hThread == nullptr) {
		std::cout << "[-] Could not create remote thread to execute the dll\n";
		VirtualFreeEx(hProc, reservedMemory, strlen(dllPath) + 1, MEM_DECOMMIT);
		CloseHandle(hProc);
		return 1;
	}

	WaitForSingleObject(hThread, INFINITE);

	VirtualFreeEx(hProc, reservedMemory, strlen(dllPath) + 1, MEM_DECOMMIT);
	CloseHandle(hProc);
	CloseHandle(hThread);

	return 1;
}
