#include <iostream>
#include <stack>
#include <vector>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <string>

using namespace std;

// Function to define operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3; // Exponentiation (right-associative)
    return 0;
}

// Function to apply an operator to two operands
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
        case '^': return pow(a, b); // Handle exponentiation
        default: throw invalid_argument("Invalid operator");
    }
}

// Function to check if a token is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// Function to check if a string represents a number
bool isNumber(const string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[i] == '-') i++; // Allow negative sign
    
    bool hasDecimal = false;
    bool hasDigit = false; // Ensures we have at least one digit
    
    for (; i < s.size(); i++) {
        if (s[i] == '.') {
            if (hasDecimal) return false; // Only one decimal point allowed
            hasDecimal = true;
        } else if (isdigit(s[i])) {
            hasDigit = true;
        } else {
            return false;
        }
    }
    
    // Must have a minumum of one digit to be a valid number 
    return hasDigit;
}


// Function to check if parentheses are balanced
bool areParenthesesBalanced(const string& expr) {
    stack<char> stk;
    for (char c : expr) {
        if (c == '(') stk.push(c);
        else if (c == ')') {
            if (stk.empty()) return false;
            stk.pop();
        }
    }
    return stk.empty();
}

// Function to tokenize the input string
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

            if (c == '-') {
                // Check for spaces around the minus
                if (i + 1 < expr.size() && isdigit(expr[i + 1]) && (i == 0 || expr[i - 1] == '(' || isOperator(expr[i - 1]))) {
                    // Treat as part of a negative number if no space after '-'
                    num += c;
                } else {
                    tokens.push_back(string(1, c)); // Treat as subtraction operator
                }
            } else if (isOperator(c) || c == '(' || c == ')') {
                tokens.push_back(string(1, c)); // Add the operator or parenthesis
            } else if (!isspace(c)) {
                throw runtime_error("Invalid character in expression");
            }
        }
    }

    if (!num.empty()) tokens.push_back(num); // Add remaining numbers
    return tokens;
}

// Function to convert infix to postfix 
vector<string> infixToPostfix(const vector<string>& tokens) {
    vector<string> postfix;
    stack<string> operators;

    for (const string& token : tokens) {
        if (isNumber(token)) {
            postfix.push_back(token); // Numbers go to the output
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                postfix.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Remove '('
        } else if (isOperator(token[0])) { // Operator
            while (!operators.empty() && precedence(operators.top()[0]) >= precedence(token[0]) && token[0] != '^') {
                postfix.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        postfix.push_back(operators.top());
        operators.pop();
    }

    return postfix;
}

// Function to evaluate a postfix expression
double evaluatePostfix(const vector<string>& postfix) {
    stack<double> operands;

    for (const string& token : postfix) {
        if (isNumber(token)) {
            operands.push(stod(token)); // Convert number to double
        } else { // Operator
            if (operands.size() < 2) throw runtime_error("Invalid expression: missing operands");
            double b = operands.top(); operands.pop();
            double a = operands.top(); operands.pop();
            operands.push(applyOperator(a, b, token[0]));
        }
    }

    if (operands.size() != 1) throw runtime_error("Invalid expression: leftover operands");
    return operands.top();
}

// Main function to parse and evaluate expressions
void parseAndEvaluate(const string& expr) {
    try {
        if (!areParenthesesBalanced(expr)) {
            throw runtime_error("Mismatched parentheses");
        }
        vector<string> tokens = tokenize(expr);
        vector<string> postfix = infixToPostfix(tokens);
        double result = evaluatePostfix(postfix);
        cout << "Result: " << result << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}
// Handles unary operators
double evaluateUnary(const string& token, double value) {
    if (token == "+") return value;
    if (token == "-") return -value;
    
}

// Main driver function
int main() {
    string expression;
    cout << "Enter an arithmetic expression (or type 'exit' to quit):" << endl;
    while (true) {
        cout << "> ";
        getline(cin, expression);
        if (expression == "exit") break;
        parseAndEvaluate(expression);
    }
    return 0;
}
