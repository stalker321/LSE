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


void memoryInfo (QElapsedTimer& timer) {
    MEMORYSTATUSEX statusEx;
    std::cout <<  "\033[1;32m";
    std::cout << "██████░ " <<"RAM info: \n";
    std::cout << "██░  ██░" << "The total amount of memory \033[1;33m" << getTotalMem(statusEx) << "\033[1;32m MB\n";
    std::cout << "██████░ " << "The amount of free memory \033[1;33m" << getUnusedMemory(statusEx) << "\033[1;32m MB\n";
    std::cout << "██░  ██░" << "Memory used by the program \033[1;33m" << getAppMemUsage() << "\033[1;32m MB\n";
    std::cout << "██░  ██░" << "\033[1;32mProgram working time: \033[1;33m" << timer.elapsed()/1000 << "\033[1;32m s\033[0m\n";
}


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

//Display message
DisplayMessage::DisplayMessage () {
    timeStartProgramm = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8();
    timer.start();
    system("chcp 65001 > nul");
    QFile programInfo("programInfo.txt");
    if (!programInfo.open(QIODevice::ReadOnly)) {
        errorLog("Error opening file information");
    }
    info = programInfo.readAll();
    cpu = getCPUInfo();
    ram = getRAMInfo();
    mainDisplayInfo();
}

void DisplayMessage::programInfoMessage () {
    printf("\033c");
    std::cout << "\033[1;32m" << info.toStdString() << "\033[0m\n";
}

void DisplayMessage::mainDisplayInfo() {
    display = false;
    programInfoMessage();
    std::cout << "\033[1;32mThe program started \033[1;33m" << timeStartProgramm.toStdString() << "\033[0m\n";
    if (!cpu.toStdString().empty()) {
        std::cout << "\033[1;32mCPU: \033[1;33m\n" << cpu.toStdString() << "\033[0m\n";
    }
    if (!ram.toStdString().empty()) {
        std::cout << "\033[1;32mRAM: \033[1;33m\n" << ram.toStdString() << "\033[0m\n";
    }
    std::cout << "Enter \033[1;33m?\033[0m for more information\n";
}

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

void DisplayMessage::displayInfo() {
    display = true;
}

void DisplayMessage::hellpInfo() {
    display = false;
    programInfoMessage();
    std::cout <<"\033[1;32m██░  ██░ \033[1;33m?\033[1;32m :: help | displays the available commands\n";
    std::cout <<"\033[1;32m██░  ██░ \033[1;33mi\033[1;32m :: info | displays information about the resources used\n";
    std::cout <<"\033[1;32m███████░ \033[1;33mb\033[1;32m :: base | add a file to the database\n";
    std::cout <<"\033[1;32m██░  ██░ \033[1;33mm\033[1;32m :: main | Output of the initial information\n";
    std::cout <<"\033[1;32m██░  ██░ \033[1;33me\033[1;32m :: exit | exiting the program\033[0m\n";
}

DisplayMessage::~DisplayMessage(){
}
