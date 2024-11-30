#include "interaction.h"

Interaction::Interaction() {}

void Interaction::userInput() {
    wchar_t input;
    while(true){
//input
        std::wcin >> input;
        std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//exit
        if (input == 'e') emit exit();
//info
        else if (input == '?') emit hint();
        else if (input == 'm') emit main();
        else if (input == 'i') emit infoMessage();
//add base
        else if (input == 'b') {
            std::wstring path;
            std::wcout << "Enter the path to the file: ";
            std::wcin >> path;
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!path.empty()) {
                emit base(QString::fromStdWString(path));
            }
        }
//set the number of output responses
        else if (input == 'r') {
            std::wstring num;
            std::wcout << "specify the number of responses to be displayed, enter\033[1;33m 0 \033[0mif you want to make the current number constant: ";
            std::wcin >> num;
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            bool flagNum = true;
            if (num.empty()) flagNum = false;
            for (const auto &i : num) {
                if (!std::isdigit(i)) flagNum = false;
            }
            if (flagNum) emit response(std::stoi(num));
        }
//checking the search
        else if (input == 't') {
            std::wstring testReq;
            std::wcout << "Enter a test query: ";
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::wcin, testReq);
            if (!testReq.empty()) {
                emit test(QString::fromStdWString(testReq));
            }
        }
//blacklist
        else if (input == 'l') {
            emit list();
            std::wcin >> input;
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (input == 'a' || input == 'd') {
                std::wstring word;
                std::wcin >> word;
                std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (input == 'a') emit addWord(QString::fromStdWString(word));
                else emit deleteWord(QString::fromStdWString(word));
            }
        }
    }
}
