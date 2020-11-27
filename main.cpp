#include "memory.h"

DWORD process_id = 0;
DWORD64 gta_base = 0;
DWORD64 module_base = 0;
HANDLE game_handle = 0;

template<typename T>
T read(uintptr_t address)
{
    T buffer;
    ReadProcessMemory(game_handle, (LPCVOID)address, &buffer, sizeof(T), NULL);
    return buffer;
}

template<typename T>
void write(uintptr_t address, T buffer)
{
    WriteProcessMemory(game_handle, (LPVOID)address, &buffer, sizeof(buffer), NULL);
}

void bypass_menu()
{
    system("cls");

    uintptr_t attach_hook = read<uintptr_t>(module_base + 0x709B8); // cwde db[F1] in[al,-21]
    uintptr_t attach_status = read<BYTE>(attach_hook + 0x9C8); // [rcx],eax

    Sleep(1000);

    std::cout << "[>] reading memory..." << std::endl;

    Sleep(300);

    std::cout << "[+] status: " << attach_status << std::endl;

    std::cout << "[>] writing memory..." << std::endl;

    write<BYTE>(attach_hook + 0x9C8, true);
    attach_status = read<BYTE>(attach_hook + 0x9C8);

    Sleep(4000);

    std::cout << "[+] status: " << attach_status << std::endl;
    
    Sleep(10000);
    
}

int main()
{
    memory memory;

    process_id = memory.get_pid(L"GTA5.exe"); if (!process_id) return false;
    gta_base = memory.get_base(process_id, L"GTA5.exe"); if (!gta_base) return false;
    module_base = memory.get_base(process_id, L"Eternity.dll"); if (!module_base) return false;
    game_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id); if (!game_handle) return false;

    std::cout << "[ ======= MODULE ======= ]" << std::endl << std::endl;
    std::cout << " PID: " << process_id << std::endl;
    std::cout << std::hex << " Base: " << gta_base << std::endl;
    std::cout << std::hex << " Module Base: " << module_base << std::endl;
    std::cout << std::endl << "[ ====================== ]";

    Sleep(1500);

    bypass_menu();
}
