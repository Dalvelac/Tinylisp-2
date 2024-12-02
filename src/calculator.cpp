#include "calculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <vector>

typedef std::vector<std::string> Tokens;

// Variables globales
std::unordered_map<std::string, double> variables;
std::unordered_map<std::string, std::pair<int, std::function<double(const std::vector<double>&)>>> functions;

// Divide una entrada en tokens separados
Tokens tokenize(const std::string& input) {
    Tokens tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Evalúa una expresión en notación postfija
double evaluate(Tokens& tokens, std::unordered_map<std::string, double> localVariables) {
    std::stack<double> stack;

    while (!tokens.empty()) {
        std::string token = tokens.front();
        tokens.erase(tokens.begin());

        // Operaciones matemáticas básicas y funciones avanzadas
        if (token == "+") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la suma");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a + b);
        } else if (token == "-") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la resta");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a - b);
        } else if (token == "*") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la multiplicación");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a * b);
        } else if (token == "/") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la división");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            if (b == 0) throw std::runtime_error("División por cero");
            stack.push(a / b);
        } else if (token == "^") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la potenciación");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(pow(a, b));
        } else if (token == "sqrt") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la raíz cuadrada");
            double a = stack.top(); stack.pop();
            if (a < 0) throw std::runtime_error("La raíz cuadrada de un número negativo no está definida");
            stack.push(sqrt(a));
        } else if (token == "%") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para el módulo");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(fmod(a, b));
        } else if (token == "abs") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para el valor absoluto");
            double a = stack.top(); stack.pop();
            stack.push(std::abs(a));
        } else if (token == "neg") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la negación");
            double a = stack.top(); stack.pop();
            stack.push(-a);
        } else if (token == "sin") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para el seno");
            double a = stack.top(); stack.pop();
            stack.push(std::sin(a));
        } else if (token == "cos") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para el coseno");
            double a = stack.top(); stack.pop();
            stack.push(std::cos(a));
        } else if (token == "tan") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la tangente");
            double a = stack.top(); stack.pop();
            stack.push(std::tan(a));
        } else if (token == "log10") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para el logaritmo base 10");
            double a = stack.top(); stack.pop();
            if (a <= 0) throw std::runtime_error("El logaritmo base 10 de un número no positivo no está definido");
            stack.push(log10(a));
        } else if (token == "exp") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la exponencial");
            double a = stack.top(); stack.pop();
            stack.push(exp(a));
        } else if (token == "floor") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la función floor");
            double a = stack.top(); stack.pop();
            stack.push(floor(a));
        } else if (token == "ceil") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la función ceil");
            double a = stack.top(); stack.pop();
            stack.push(ceil(a));
        } else if (token == "round") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la función round");
            double a = stack.top(); stack.pop();
            stack.push(round(a));
        } else if (token == "min") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la función min");
            double a = stack.top(); stack.pop();
            double b = stack.top(); stack.pop();
            stack.push(std::min(a, b));
        } else if (token == "max") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la función max");
            double a = stack.top(); stack.pop();
            double b = stack.top(); stack.pop();
            stack.push(std::max(a, b));
        } else if (token == "push") {
            if (tokens.empty()) throw std::runtime_error("Falta un valor para push");
            std::string value = tokens.front(); tokens.erase(tokens.begin());
            stack.push(std::stod(value));
        } else if (token == "asin") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para el arcoseno");
            double a = stack.top(); stack.pop();
            if (a < -1 || a > 1) throw std::runtime_error("El arcoseno está fuera del rango válido");
            stack.push(asin(a));
        } else if (token == "=") {
            if (tokens.empty()) throw std::runtime_error("Falta el nombre de la variable para la asignación");
            std::string varName = tokens.front(); tokens.erase(tokens.begin());
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la asignación");
            double value = stack.top(); stack.pop();
            variables[varName] = value;
            std::cout << "El valor ha sido asignado." << std::endl;
        } else if (token == "acos") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para el arcocoseno");
            double a = stack.top(); stack.pop();
            if (a < -1 || a > 1) throw std::runtime_error("El arcocoseno está fuera del rango válido");
            stack.push(acos(a));
        } else if (token == "atan") {
            if (stack.empty()) throw std::runtime_error("Operandos insuficientes para el arcotangente");
            double a = stack.top(); stack.pop();
            stack.push(atan(a));
        } else if (token == "atan2") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para atan2");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(atan2(a, b));
        } else if (token == "hypot") {
            if (stack.size() < 2) throw std::runtime_error("Operandos insuficientes para la hipotenusa");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(hypot(a, b));

        } else if (token == "defun") {
            if (tokens.size() < 3) throw std::runtime_error("Sintaxis incorrecta para la definición de función");
            std::string funcName = tokens.front(); tokens.erase(tokens.begin());
            int paramCount = std::stoi(tokens.front()); tokens.erase(tokens.begin());
            Tokens paramNames(tokens.begin(), tokens.begin() + paramCount);
            tokens.erase(tokens.begin(), tokens.begin() + paramCount);
            Tokens bodyTokens(tokens);
            functions[funcName] = {paramCount, [paramNames, bodyTokens](const std::vector<double>& args) -> double {
                if (args.size() != paramNames.size()) throw std::runtime_error("Número incorrecto de argumentos para la función");
                std::unordered_map<std::string, double> localVars;
                for (size_t i = 0; i < paramNames.size(); ++i) {
                    localVars[paramNames[i]] = args[i];
                }
                Tokens evalTokens = bodyTokens;
                return evaluate(evalTokens, localVars);
            }};
            std::cout << "Función " << funcName << " definida con éxito." << std::endl;
        } else {
            try {
                stack.push(std::stod(token));
            } catch (const std::invalid_argument&) {
                if (localVariables.find(token) != localVariables.end()) {
                    stack.push(localVariables[token]);
                } else if (variables.find(token) != variables.end()) {
                    stack.push(variables[token]);
                } else if (functions.find(token) != functions.end()) {
                    auto& func = functions[token];
                    int argCount = func.first;
                    std::vector<double> args;
                    for (int i = 0; i < argCount; ++i) {
                        if (stack.empty()) throw std::runtime_error("Operandos insuficientes para la función " + token);
                        args.push_back(stack.top());
                        stack.pop();
                    }
                    std::reverse(args.begin(), args.end());
                    stack.push(func.second(args));
                } else {
                    throw std::runtime_error("Variable o función desconocida: " + token);
                }
            }
        }
    }

    if (stack.empty()) throw std::runtime_error("No hay resultado disponible en la pila");
    return stack.top();
}

// Carga un script desde un archivo y lo evalúa
void load_script() {
    std::string filename;
    std::cout << "Ingrese el nombre del archivo: ";
    std::getline(std::cin, filename);

    if (filename.empty()) {
        std::cerr << "Error: No se proporcionó un nombre de archivo." << std::endl;
        return;
    }

    load_script(filename.c_str(), true); // Llama a la versión con argumentos.
}

void load_script(const char* filename, bool show_script) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (show_script) std::cout << line << std::endl; // Muestra la línea del script si se requiere.

        try {
            Tokens tokens = tokenize(line); // Divide la línea en tokens.
            evaluate(tokens); // Evalúa los tokens.
        } catch (const std::exception& e) {
            std::cerr << "Error al evaluar el script: " << e.what() << std::endl;
        }
    }

    if (file.bad()) std::cerr << "Error durante la lectura del archivo " << filename << std::endl;
    file.close();
}
