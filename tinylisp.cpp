#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <functional>

// Prototipos de funciones
void load_script(const char* filename, bool show_script = false);
void load_script();

typedef std::vector<std::string> Tokens;
std::unordered_map<std::string, double> variables;
std::unordered_map<std::string, std::pair<int, std::function<double(const std::vector<double>&)>>> functions;

// Funcion para dividir una entrada en tokens separados
Tokens tokenize(const std::string& input) {
    Tokens tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Funcion para evaluar una expresion en notacion postfija
// Usa una pila (stack) para almacenar operandos y realizar operaciones
// La funcion admite variables locales para evaluar funciones personalizadas
double evaluate(Tokens& tokens, std::unordered_map<std::string, double> localVariables = {}) {
    std::stack<double> stack;

    while (!tokens.empty()) {
        std::string token = tokens.front();
        tokens.erase(tokens.begin());

        if (token == "+") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la suma");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a + b);
        } else if (token == "-") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la resta");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a - b);
        } else if (token == "*") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la multiplicacion");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a * b);
        } else if (token == "/") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la division");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            if (b == 0) {
                throw std::runtime_error("Division por cero");
            }
            stack.push(a / b);
        } else if (token == "^") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la potenciacion");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(pow(a, b));
        } else if (token == "sqrt") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la raiz cuadrada");
            }
            double a = stack.top(); stack.pop();
            if (a < 0) {
                throw std::runtime_error("La raiz cuadrada de un numero negativo no esta definida");
            }
            stack.push(sqrt(a));
        } else if (token == "%") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para el modulo");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            if (b == 0) {
                throw std::runtime_error("Division por cero");
            }
            stack.push(fmod(a, b));
        } else if (token == "abs") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el valor absoluto");
            }
            double a = stack.top(); stack.pop();
            stack.push(std::abs(a));
        } else if (token == "neg") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la negacion");
            }
            double a = stack.top(); stack.pop();
            stack.push(-a);
        } else if (token == "sin") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el seno");
            }
            double a = stack.top(); stack.pop();
            stack.push(std::sin(a));
        } else if (token == "cos") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el coseno");
            }
            double a = stack.top(); stack.pop();
            stack.push(std::cos(a));
        } else if (token == "tan") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la tangente");
            }
            double a = stack.top(); stack.pop();
            stack.push(std::tan(a));
        } else if (token == "asin") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el arcoseno");
            }
            double a = stack.top(); stack.pop();
            if (a < -1 || a > 1) {
                throw std::runtime_error("El arcoseno esta fuera del rango valido");
            }
            stack.push(asin(a));
        } else if (token == "acos") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el arcocoseno");
            }
            double a = stack.top(); stack.pop();
            if (a < -1 || a > 1) {
                throw std::runtime_error("El arcocoseno esta fuera del rango valido");
            }
            stack.push(acos(a));
        } else if (token == "atan") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el arcotangente");
            }
            double a = stack.top(); stack.pop();
            stack.push(atan(a));
        } else if (token == "atan2") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para atan2");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(atan2(a, b));
        } else if (token == "hypot") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la hipotenusa");
            }
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(hypot(a, b));
        } else if (token == "ln") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el logaritmo natural");
            }
            double a = stack.top(); stack.pop();
            if (a <= 0) {
                throw std::runtime_error("El logaritmo natural de un numero no positivo no esta definido");
            }
            stack.push(log(a));
        } else if (token == "log10") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para el logaritmo base 10");
            }
            double a = stack.top(); stack.pop();
            if (a <= 0) {
                throw std::runtime_error("El logaritmo base 10 de un numero no positivo no esta definido");
            }
            stack.push(log10(a));
        } else if (token == "exp") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la exponencial");
            }
            double a = stack.top(); stack.pop();
            stack.push(exp(a));
        } else if (token == "min") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la funcion min");
            }
            double a = stack.top(); stack.pop();
            double b = stack.top(); stack.pop();
            stack.push(std::min(a, b));
        } else if (token == "max") {
            if (stack.size() < 2) {
                throw std::runtime_error("Operandos insuficientes para la funcion max");
            }
            double a = stack.top(); stack.pop();
            double b = stack.top(); stack.pop();
            stack.push(std::max(a, b));
        } else if (token == "floor") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la funcion floor");
            }
            double a = stack.top(); stack.pop();
            stack.push(floor(a));
        } else if (token == "ceil") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la funcion ceil");
            }
            double a = stack.top(); stack.pop();
            stack.push(ceil(a));
        } else if (token == "round") {
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la funcion round");
            }
            double a = stack.top(); stack.pop();
            stack.push(round(a));
        } else if (token == "=") {
            // Operador de asignacion, se espera un nombre de variable para almacenar el valor en la pila
            if (tokens.empty()) {
                throw std::runtime_error("Falta el nombre de la variable para la asignacion");
            }
            std::string varName = tokens.front();
            tokens.erase(tokens.begin());
            if (stack.empty()) {
                throw std::runtime_error("Operandos insuficientes para la asignacion");
            } else {
                double value = stack.top(); stack.pop();
                variables[varName] = value;
                std::cout << "El valor ha sido asignado." << std::endl;
            }
        } else if (token == "defun") {
            // Definicion de una funcion con nombre y parametros
            if (tokens.size() < 3) {
                throw std::runtime_error("Sintaxis incorrecta para la definicion de funcion");
            }
            std::string funcName = tokens.front(); tokens.erase(tokens.begin());
            int paramCount;
            try {
                paramCount = std::stoi(tokens.front()); tokens.erase(tokens.begin());
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("El numero de parametros debe ser un entero");
            }
            // Obtener nombres de los parametros
            Tokens paramNames(tokens.begin(), tokens.begin() + paramCount);
            tokens.erase(tokens.begin(), tokens.begin() + paramCount);
            Tokens bodyTokens(tokens);
            // Almacenar la funcion en el mapa de funciones
            functions[funcName] = {paramCount, [paramNames, bodyTokens](const std::vector<double>& args) -> double {
                if (args.size() != paramNames.size()) {
                    throw std::runtime_error("Numero incorrecto de argumentos para la funcion");
                }
                std::unordered_map<std::string, double> localVariables;
                for (size_t i = 0; i < paramNames.size(); ++i) {
                    localVariables[paramNames[i]] = args[i];
                }
                Tokens evalTokens = bodyTokens;
                return evaluate(evalTokens, localVariables);
            }};
            std::cout << "Funcion " << funcName << " definida con exito." << std::endl;
            return 0;
        } else {
            try {
                // Intentar convertir el token en un numero
                stack.push(std::stod(token));
            } catch (const std::invalid_argument&) {
                // Si no es un numero, verificar si es una variable, funcion o generar un error
                if (localVariables.find(token) != localVariables.end()) {
                    stack.push(localVariables[token]);
                } else if (variables.find(token) != variables.end()) {
                    stack.push(variables[token]);
                } else if (functions.find(token) != functions.end()) {
                    // Llamada a una funcion definida por el usuario
                    auto& funcPair = functions[token];
                    int argCount = funcPair.first;
                    std::vector<double> args;
                    for (int i = 0; i < argCount; ++i) {
                        if (stack.empty()) {
                            throw std::runtime_error("Operandos insuficientes para la funcion " + token);
                        }
                        args.push_back(stack.top());
                        stack.pop();
                    }
                    // Invertir los argumentos ya que se extrajeron en orden inverso
                    std::reverse(args.begin(), args.end());
                    stack.push(funcPair.second(args));
                } else {
                    throw std::runtime_error("Variable o funcion desconocida: " + token);
                }
            }
        }
    }

    if (stack.empty()) {
        throw std::runtime_error("No hay resultado disponible en la pila");
    }

    return stack.top();
}

// Implementacion de la funcion load_script para cargar un script desde un archivo
void load_script(const char* filename, bool show_script) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (show_script) {
            std::cout << line << std::endl;
        }
        try {
            // Evaluar cada linea del archivo
            Tokens tokens = tokenize(line);
            evaluate(tokens);
        } catch (const std::exception& e) {
            std::cerr << "Error al evaluar el script: " << e.what() << std::endl;
        }
    }

    if (file.bad()) {
        std::cerr << "Error: Se produjo un error durante la lectura del archivo " << filename << std::endl;
    }

    file.close();
}

// Implementacion de la funcion load_script que solicita el nombre del archivo al usuario
void load_script() {
    std::string filename;
    std::cout << "Ingrese el nombre del archivo: ";
    std::getline(std::cin, filename);

    if (filename.empty()) {
        std::cerr << "Error: No se proporciono un nombre de archivo." << std::endl;
        return;
    }

    load_script(filename.c_str(), true);
}

// Funcion principal para interactuar con el usuario
int main() {
    std::string input;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);
        if (input == "Salir") break;
        if (input.rfind("load ", 0) == 0) {
            // Cargar un script si el comando comienza con "load "
            std::string filename = input.substr(5);
            load_script(filename.c_str(), true);
        } else {
            try {
                // Evaluar la expresion ingresada por el usuario
                Tokens tokens = tokenize(input);
                std::cout << evaluate(tokens) << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    return 0;
}
