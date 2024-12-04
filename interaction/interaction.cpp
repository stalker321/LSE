#include "interaction.h"
//add base
std::wstring addBase() {
    std::wstring path;
    std::wcout << "Enter the path to the file: ";
    std::wcin >> path;
    return path;
}
//set the num responses
int inputNumResposes() {
    std::wstring num;
    std::wcout << "specify the number of responses to be displayed, enter\033[1;33m 0 \033[0mif you want to make the current number constant: ";
    std::wcin >> num;
    bool flagNum = true;
    if (num.empty()) flagNum = false;
    for (const auto &i : num) {
        if (!std::isdigit(i)) flagNum = false;
    }
    if (flagNum) return std::stoi(num);
    return -1;
}
//entering a test query
std::wstring testQuery(){
    std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::wstring testReq;
    std::wcout << "Enter a test query: ";
    std::getline(std::wcin, testReq);
    return testReq;
}
//blacklist
wchar_t action () {
    wchar_t input;
    std::wcin >> input;
    return input;
}
std::wstring word() {
    std::wstring input;
    std::wcin >> input;
    return input;
}

//interaction
Interaction::Interaction() {}

void Interaction::userInput() {
    wchar_t input;
    while(true){
//input
        std::wcin >> input;
        std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (input) {
//exit
        case 'e':
            emit exit();
            break;
//info
        case '?':
            emit hint();
            break;
        case 'm':
            emit main();
            break;
        case 'i':
            emit infoMessage();
            break;
//add base
        case 'b':
            emit base(QString::fromStdWString(addBase()));
            break;
//set the number of output responses
        case 'r':
            emit response(inputNumResposes());
            break;
//test the search
        case 't':
            emit test(QString::fromStdWString(testQuery()));
            break;
//blacklist
        case 'l':
            emit list();
            input = action();
            if (input == 'a'){
                std::wcout << "Enter the word you want to add: ";
                emit addWord(QString::fromStdWString(word()));
                break;
            } else if (input == 'd'){
                std::wcout << "Enter the word you want to delete: ";
                emit deleteWord(QString::fromStdWString(word()));
                break;
            }
            break;
        case 'f':
            emit infoErrorLog();
            break;
        default:
            break;
        }
    }
}
