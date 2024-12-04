#include <iostream>
#include <string>
#include <sstream>

int addition(int num1, int num2) {
    return num1 + num2;
}

int subtraction(int num1, int num2) {
    return num1 - num2;
}

int main() {
    std::string input;
    int num1, num2, output;
    char op;

    std::cout << "Enter an equation (e.g., 2 + 2 or 2+2): ";
    std::getline(std::cin, input);  

    input.erase(remove(input.begin(), input.end(), ' '), input.end());

  
    std::stringstream ss(input);

    ss >> num1 >> op >> num2;  

    if (ss.fail()) {
        std::cout << "Invalid input format." << std::endl;
        return 1;
    }

    switch (op) {
        case '+':
            output = addition(num1, num2);
            break;
        case '-':
            output = subtraction(num1, num2);
            break;
        default:
            std::cout << "Unsupported operator." << std::endl;
            return 1;
    }

    std::cout << "Result: " << output << std::endl;
    return 0;
}
