#include "interaction.h"

Interaction::Interaction() {}

void Interaction::userInput() {
    char input;
    while(true){
        std::cin >> input;
        if (input == 'e') emit exit();
        if (input == '?') emit hint();
        if (input == 'm') emit main();
        if (input == 'b') {
            std::string path;
            std::cout << "Enter the path to the file: ";
            std::cin >> path;
                if (!path.empty()) {
                emit base(QString(path.c_str()));
            }
        }
        if (input == 'i') emit infoMessage();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
