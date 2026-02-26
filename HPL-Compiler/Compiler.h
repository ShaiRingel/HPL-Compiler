#pragma once
#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"

typedef struct {
	Lexer* lexer;
	Parser* parser;
	SemanticAnalyzer* analyzer;
	// CodeGenerator* generator;
	// ErrorHandler* handler;
} Compiler;

// Initialize Components
Compiler* initCompiler(char* filePath);

// Starts the compilation process
void startCompiler(Compiler* compiler);

// Frees all the compiler
void freeCompiler(Compiler* compiler);