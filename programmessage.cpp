#include <thread>
#include <chrono>

#include "programmessage.h"

int toMegabytes (1048576);

#define CHECK_POWERSHELL() \
    (system("powershell -Command \"exit\"") == 0)

int getTotalMem (MEMORYSTATUSEX &statusEx) {
    statusEx.dwLength = sizeof(statusEx);
    GlobalMemoryStatusEx(&statusEx);
    unsigned long long totalMem = statusEx.ullTotalPhys;

    return totalMem/toMegabytes;
}

int getUnusedMemory (MEMORYSTATUSEX &statusEx) {
    statusEx.dwLength = sizeof(statusEx);
    GlobalMemoryStatusEx(&statusEx);
    unsigned long long totalMem = statusEx.ullAvailPhys;
    return totalMem/toMegabytes;
}

int getAppMemUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    pmc.cb = sizeof(pmc);
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize/toMegabytes;
}

//it works in a parallel stream
void memoryInfo (QElapsedTimer& timer) {
    MEMORYSTATUSEX statusEx;
    std::wcout << L"\033[1;32m";
    std::wcout << L"██████░ " <<"RAM info: \n";
    std::wcout << L"██░  ██░" << "The total amount of memory \033[1;33m" << getTotalMem(statusEx) << "\033[1;32m MB\n";
    std::wcout << L"██████░ " << "The amount of free memory \033[1;33m" << getUnusedMemory(statusEx) << "\033[1;32m MB\n";
    std::wcout << L"██░  ██░" << "Memory used by the program \033[1;33m" << getAppMemUsage() << "\033[1;32m MB\n";
    std::wcout << L"██░  ██░" << "\033[1;32mProgram working time: \033[1;33m" << timer.elapsed()/1000 << "\033[1;32m s\033[0m\n";
}

//computer information
QString getCPUInfo() {
    QString cpu;
    QProcess process;
    if (CHECK_POWERSHELL()){
        process.start("powershell", QStringList() << "Get-CimInstance -ClassName Win32_Processor "
                                                     "| Select-Object -First 1 | Format-List Name");
    } else {
        process.start("wmic cpu get caption");
    }
    process.waitForFinished();
    cpu = process.readAllStandardOutput().trimmed();
    return cpu;
}

QString getRAMInfo() {
    QString ram;
    QProcess process;
    if (CHECK_POWERSHELL()){
       process.start("powershell", QStringList() << "Get-CimInstance -ClassName Win32_PhysicalMemory "
                                                     "| Select-Object DeviceLocator, @{Name='Capacity MB';Expression={$_.'Capacity' / 1MB}}, Manufacturer");
    } else {
        process.start("wmic memorychip get capacity");
    }
    process.waitForFinished();
    ram = process.readAllStandardOutput().trimmed();
    return ram;
}
//start
DisplayMessage::DisplayMessage () {
    timeStartProgramm = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8();
    timer.start();
    QFile programInfo("programInfo.txt");
    if (!programInfo.open(QIODevice::ReadOnly)) {
        errorLog("Error opening file information");
    }
    info = programInfo.readAll();
    cpu = getCPUInfo();
    ram = getRAMInfo();
    mainDisplayInfo();
}
//information about the program
void DisplayMessage::programInfoMessage () {
    std::wcout << "\033c";
    std::wcout << L"\033[1;32m" << info.toStdString().c_str() << "\033[0m\n";
}
//The main message
void DisplayMessage::mainDisplayInfo() {
    display = false;
    programInfoMessage();
    std::wcout << L"\033[1;32mThe program started \033[1;33m" << timeStartProgramm.toStdString().c_str() << "\033[0m\n";
    if (!cpu.toStdString().empty()) {
        std::wcout << L"\033[1;32mCPU: \033[1;33m\n" << cpu.toStdString().c_str() << "\033[0m\n";
    }
    if (!ram.toStdString().empty()) {
        std::wcout << L"\033[1;32mRAM: \033[1;33m\n" << ram.toStdString().c_str() << "\033[0m\n";
    }
    std::wcout << L"Enter \033[1;33m?\033[0m for more information\n";
}
//starting an independent stream to display resources.
void DisplayMessage::resources () {
    std::thread([this]() {
        while (true) {
            if (display) {
                programInfoMessage();
                memoryInfo(timer);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }).detach();
}
//enabling the resource display flag
void DisplayMessage::displayInfo() {
    display = true;
}
//hotkey hint
void DisplayMessage::hellpInfo() {
    display = false;
    programInfoMessage();
    std::wcout <<L"\033[1;32m██░  ██░ \033[1;33m?\033[1;32m :: help  | displays the available commands\n";
    std::wcout <<L"\033[1;32m██░  ██░ \033[1;33mi\033[1;32m :: info  | displays information about the resources used\n";
    std::wcout <<L"\033[1;32m███████░ \033[1;33mb\033[1;32m :: base  | add a file to the database\n";
    std::wcout <<L"\033[1;32m██░  ██░ \033[1;33mm\033[1;32m :: main  | Output of the initial information\n";
    std::wcout <<L"\033[1;32m██░  ██░ \033[1;33mt\033[1;32m :: test  | verification by request (displayed in the history)\033[0m\n";
    std::wcout <<L"\033[1;32m         \033[1;33mr\033[1;32m :: reply | set the maximum value of the output responses\033[0m\n";
    std::wcout <<L"\033[1;32m         \033[1;33ml\033[1;32m :: list  | view/edit the blacklist\033[0m\n";
    std::wcout <<L"\033[1;32m         \033[1;33me\033[1;32m :: exit  | exiting the program\033[0m\n";
}
//displaying the blacklist
void DisplayMessage::displayList() {
    std::int8_t length = 0;
    for (const auto &i : stopWord) {
        std::wcout << i.toStdWString() << " ";
        length++;
        if (!(length < 5)) {
            std::wcout << std::endl;
            length = 0;
        }
    }
    std::wcout << std::endl;
    std::wcout << "Select the desired action a/d (add/delete word): ";
}
//displaying program messages
void DisplayMessage::displayFunctionMessage(QString message) {
    std::wcout << L"\033[1;31m" << message.toStdWString() << "\033[0m";
}
//
DisplayMessage::~DisplayMessage(){}
