#include <thread>


#include "programmessage.h"

//operating system checks
#if defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS
#elif defined(__linux__)
#else
#define OS_UNKNOWN
#endif


//windows
#if defined(OS_WINDOWS)
#pragma comment( lib, "psapi.lib" )
#include <windows.h>
#include <psapi.h>
int toMegabytes (1048576);

#define CHECK_POWERSHELL() \
    (system("powershell -Command \"exit\"") == 0)

int getTotalMem () {
    MEMORYSTATUSEX statusEx;
    statusEx.dwLength = sizeof(statusEx);
    GlobalMemoryStatusEx(&statusEx);
    unsigned long long totalMem = statusEx.ullTotalPhys;

    return totalMem/toMegabytes;
}

int getUnusedMemory () {
    MEMORYSTATUSEX statusEx;
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

#endif

//linux
#if defined(OS_LINUX)

#include <fstream>
int getTotalMem() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            unsigned long long memKb;
            sscanf(line.c_str(), "MemTotal: %llu kB", &memKb);
            return memKb / 1024;
        }
    }
    return -1;
}

int getAppMemUsage() {
    std::ifstream statusFile("/proc/self/status");
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.find("VmRSS:") == 0) {
            unsigned long long memKb;
            sscanf(line.c_str(), "VmRSS: %llu kB", &memKb);
            return memKb / 1024;
        }
    }
    return -1;
}

int getUnusedMemory() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        return -1;
    }

    std::string line;
    unsigned long long freeMemKb = 0;

    // Считываем строки из /proc/meminfo
    while (std::getline(meminfo, line)) {
        if (line.find("MemAvailable:") == 0) {
            std::istringstream iss(line);
            std::string key;
            iss >> key >> freeMemKb;
            break;
        }
    }

    meminfo.close();

    const unsigned int toMegabytes = 1024;
    return static_cast<int>(freeMemKb / toMegabytes);
}

#endif

#if defined(OS_UNKNOWN)
int getTotalMem (MEMORYSTATUSEX &statusEx) {
    return -1;
}

int getUnusedMemory (MEMORYSTATUSEX &statusEx) {
    return -1;
}

int getAppMemUsage() {
    return -1;
}
#endif

//it works in a parallel stream
void memoryInfo (QElapsedTimer& timer) {
    std::wcout << L"\033[1;32m";
    std::wcout << L"██████░ " <<"RAM info: \n";
    std::wcout << L"██░  ██░" << "The total amount of memory \033[1;33m" << getTotalMem() << "\033[1;32m MB\n";
    std::wcout << L"██████░ " << "The amount of free memory \033[1;33m" << getUnusedMemory() << "\033[1;32m MB\n";
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
    std::wcout <<L"\033[1;32m██░  ██░ \033[1;33mm\033[1;32m :: main  | output of the initial information\n";
    std::wcout <<L"\033[1;32m██░  ██░ \033[1;33mt\033[1;32m :: test  | verification by request (not displayed in the history)\033[0m\n";
    std::wcout <<L"\033[1;32m         \033[1;33ml\033[1;32m :: list  | view/edit the blacklist\033[0m\n";
    std::wcout <<L"\033[1;32m         \033[1;33mr\033[1;32m :: reply | set the maximum value of the output responses\033[0m\n";
    std::wcout <<L"\033[1;32m         \033[1;33mf\033[1;32m :: fault | view the error list\033[0m\n";
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
    std::wcout << L"\033[1;31m" << message.toStdWString() << "\033[0m\n";
}

//display error log
void DisplayMessage::displayErrorLog(){
    QFile log ("ErrorLog.txt");
    QString message;

    if (!log.open(QIODevice::ReadOnly)) {
        message = "The file could not be read ErrorLog.txt";
        displayFunctionMessage(message);
        return;
    }

    if (log.size() == 0) {
        message = "The error log is empty";
        log.close();
        displayFunctionMessage(message);
        return;
    }
    message = log.readAll();
    log.close();
    displayFunctionMessage(message);
    return;
}

//
DisplayMessage::~DisplayMessage(){}
