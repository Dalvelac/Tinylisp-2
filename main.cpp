#include "src/calculator.h"
#include <iostream>
int main() {
    std::string input;
    try {
        while (true) {
            std::cout << ">> ";
            std::getline(std::cin, input);
            if (input == "Salir") break;
            if (input.rfind("load ", 0) == 0) {
                std::string filename = input.substr(5);
                load_script(filename.c_str(), true);
            } else {
                try {
                    Tokens tokens = tokenize(input);
                    std::cout << evaluate(tokens) << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error crítico: " << e.what() << std::endl;
    }
    return 0;
}