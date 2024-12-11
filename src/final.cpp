#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <stdexcept>
#include <cctype>

using namespace std;

// Check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

// Check if a string is a valid number
bool isNumber(const string& s) {
    try {
        stod(s);
        return true;
    } catch (...) {
        return false;
    }
}

// Define operator precedence
int precedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/':
        case '%': return 2;
        case '^': return 3;
        default: return 0;
    }
}

// Tokenize the input expression into components (numbers, operators, parentheses)
vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    string num = "";

    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];

        if (isdigit(c) || c == '.') {
            num += c; // Accumulate number
        } else {
            if (!num.empty()) {
                tokens.push_back(num); // Add the number
                num = "";
            }

            // Handle unary operators (+ or -) explicitly
            if ((c == '+' || c == '-') && (i == 0 || expr[i - 1] == '(' || isOperator(expr[i - 1]))) {
                // This is a unary operator, accumulate it into the number string
                num += c;
            } else if (isOperator(c) || c == '(' || c == ')') {
                tokens.push_back(string(1, c)); // Add operator or parenthesis
            } else if (!isspace(c)) {
                throw runtime_error("Invalid character in expression");
            }
        }
    }

    if (!num.empty()) tokens.push_back(num); // Add remaining number
    return tokens;
}

// Convert infix expression tokens to postfix notation
vector<string> infixToPostfix(const vector<string>& tokens) {
    vector<string> postfix;
    stack<string> operators;

    for (const string& token : tokens) {
        if (isNumber(token)) {
            postfix.push_back(token);
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                postfix.push_back(operators.top());
                operators.pop();
            }
            if (operators.empty() || operators.top() != "(") {
                throw runtime_error("Mismatched parentheses");
            }
            operators.pop();
        } else if (isOperator(token[0])) {
            while (!operators.empty() && precedence(operators.top()[0]) >= precedence(token[0]) && token[0] != '^') {
                postfix.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        if (operators.top() == "(" || operators.top() == ")") {
            throw runtime_error("Mismatched parentheses");
        }
        postfix.push_back(operators.top());
        operators.pop();
    }

    return postfix;
}

// Apply an operator to two operands
double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
        case '%':
            if (b == 0) throw runtime_error("Modulo by zero");
            return fmod(a, b);
        case '^': return pow(a, b);
        default: throw invalid_argument("Invalid operator");
    }
}

// Evaluate the postfix expression
// Handles both unary and binary operators
double evaluatePostfix(const vector<string>& postfix) {
    stack<double> operands;

    for (const string& token : postfix) {
        if (isNumber(token)) {
            operands.push(stod(token));
        } else if (isOperator(token[0])) { // Handle operators
            if (token == "-" && operands.size() == 1) { // Unary minus
                double value = operands.top();
                operands.pop();
                operands.push(-value);
            } else { // Binary operators
                if (operands.size() < 2) throw runtime_error("Invalid expression: missing operands");
                double b = operands.top(); operands.pop();
                double a = operands.top(); operands.pop();
                operands.push(applyOperator(a, b, token[0]));
            }
        }
    }

    if (operands.size() != 1) throw runtime_error("Invalid expression: leftover operands");
    return operands.top();
}

// Evaluate the entire expression from input string
// Combines tokenization, postfix conversion, and evaluation
double evaluateExpression(const string& expr) {
    vector<string> tokens = tokenize(expr);
    vector<string> postfix = infixToPostfix(tokens);
    return evaluatePostfix(postfix);
}

int main() {
    string expression;
    cout << "Enter an expression: ";
    getline(cin, expression);

    try {
        double result = evaluateExpression(expression);
        cout << "Result: " << result << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
