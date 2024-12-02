#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// Tipos de datos utilizados
typedef std::vector<std::string> Tokens;

// Variables globales
extern std::unordered_map<std::string, double> variables;
extern std::unordered_map<std::string, std::pair<int, std::function<double(const std::vector<double>&)>>> functions;

// Declaraciones de funciones
void load_script(const char* filename, bool show_script = false);
void load_script();
typedef std::vector<std::string> Tokens;
Tokens tokenize(const std::string& input);
double evaluate(Tokens& tokens, std::unordered_map<std::string, double> localVariables = {});

#endif // CALCULATOR_H
