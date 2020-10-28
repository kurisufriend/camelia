#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int main()
{
    TCHAR fQPath[MAX_PATH];
    GetModuleFileNameA(NULL, fQPath, MAX_PATH);
    std::string path = fQPath;
    replace(path, "Catachresis.exe", "camelia.dll");
    std::cout << path;
    DWORD procId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);
        if (Process32First(snapshot, &procEntry))
        {
            do
            {
                if (!_stricmp(procEntry.szExeFile, "csgo.exe"))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &procEntry));
        }
    }
    CloseHandle(snapshot);

    if (!procId)
        return -1;
    std::cout << "process (hopefully) found" << std::endl;
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    std::cout << "got handle" << std::endl;
    if (!hProc)
        return -1;
    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        std::cout << "allocated " << MAX_PATH << " at " << std::hex << loc << std::endl;

        WriteProcessMemory(hProc, loc, path.c_str(), path.length() + 1, 0);

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (!hThread)
            return -1;
        std::cout << "created thread " << std::hex << hThread << std::endl;

        CloseHandle(hThread);
    }
    CloseHandle(hProc);
    std::cout << "injected ;^)" << std::endl;
    std::cin.ignore();
    return 0;
}